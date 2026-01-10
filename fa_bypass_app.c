#include "fa_bypass_app.h"
#include "fa_bypass_main_view.h"
#include "fa_bypass_crypto.h"
#include "fa_bypass_config.h"
#include <furi_hal_rtc.h>
#include <notification/notification_messages.h>
#include <storage/storage.h>

#include "gui/view_dispatcher.h"

#define TAG "FABypassApp"

/* Forward declarations */
static int32_t fa_bypass_update_thread(void* context);

FABypassApp* fa_bypass_app_alloc(void) {
    FABypassApp* app = malloc(sizeof(FABypassApp));
    
    app->gui = furi_record_open(RECORD_GUI);
    if(app->gui == NULL) {
        FURI_LOG_E(TAG, "Failed to open GUI record");
        free(app);
        return NULL;
    }
    
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    if(app->notifications == NULL) {
        FURI_LOG_E(TAG, "Failed to open Notification record");
        furi_record_close(RECORD_GUI);
        free(app);
        return NULL;
    }
    
    app->running = true;
    app->last_update = 0;
    app->main_view = NULL;
    snprintf(app->current_code, sizeof(app->current_code), "------");
    
    return app;
}

void fa_bypass_app_free(FABypassApp* app) {
    furi_assert(app);
    
    app->running = false;
    if(app->update_thread) {
        furi_thread_join(app->update_thread);
        furi_thread_free(app->update_thread);
    }
    
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    free(app);
}

static int32_t fa_bypass_update_thread(void* context) {
    furi_assert(context);
    FABypassApp* app = (FABypassApp*)context;
    
    FURI_LOG_I(TAG, "Update thread started");
    
    // TODO: Load TOTP secret from storage
    // For now, use a demo secret (base32 encoded)
    const char* demo_secret = "JBSWY3DPEBLW64TMMQ======";  // Example secret
    uint8_t secret[256];
    int secret_len = base32_decode(demo_secret, secret, sizeof(secret));
    
    if(secret_len <= 0) {
        FURI_LOG_E(TAG, "Failed to decode secret, len=%d", secret_len);
        return -1;
    }
    
    FURI_LOG_I(TAG, "Secret decoded, length=%d", secret_len);
    
    uint32_t last_log = 0;
    UNUSED(last_log);
    while(app->running) {
        uint32_t timestamp = furi_hal_rtc_get_timestamp();
        
        // Update TOTP code every REFRESH_MS
        if((timestamp - app->last_update) * 1000 >= REFRESH_MS) {
            if(totp_calculate(secret, secret_len, timestamp, app->current_code)) {
                FURI_LOG_I(TAG, "Generated TOTP: %s", app->current_code);
                app->last_update = timestamp;
            }
        }
        
        furi_delay_ms(100);
    }
    
    FURI_LOG_I(TAG, "Update thread exiting");
    return 0;
}

int32_t fa_bypass_app_run(void* p) {
    UNUSED(p);

    FURI_LOG_I(TAG, "Starting FA Bypass App");
    
    FABypassApp* app = fa_bypass_app_alloc();
    if(app == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate app");
        return -1;
    }
    
    FURI_LOG_I(TAG, "App allocated");
    
    // Create and start update thread
    app->update_thread = furi_thread_alloc();
    if(app->update_thread == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate update thread");
        fa_bypass_app_free(app);
        return -1;
    }
    
    furi_thread_set_name(app->update_thread, "FABypassUpdateThread");
    furi_thread_set_stack_size(app->update_thread, 2048);
    furi_thread_set_callback(app->update_thread, fa_bypass_update_thread);
    furi_thread_set_context(app->update_thread, app);
    
    // Delay before starting thread to ensure app is fully initialized
    furi_delay_ms(100);
    
    furi_thread_start(app->update_thread);
    FURI_LOG_I(TAG, "Update thread started");
    
    // Create main view
    FABypassMainView* main_view = fa_bypass_main_view_alloc(app);
    if(main_view == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate main view");
        fa_bypass_app_free(app);
        return -1;
    }
    
    FURI_LOG_I(TAG, "Main view allocated, state=%p, app=%p", main_view->state, app);
    
    // Store main_view reference so update thread can access it
    app->main_view = main_view;
    FURI_LOG_I(TAG, "main_view stored in app");
    
    // Create view dispatcher
    ViewDispatcher* view_dispatcher = view_dispatcher_alloc();
    if(view_dispatcher == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate view dispatcher");
        fa_bypass_main_view_free(main_view);
        fa_bypass_app_free(app);
        return -1;
    }
    
    FURI_LOG_I(TAG, "View dispatcher allocated");
    
    view_dispatcher_attach_to_gui(view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);
    FURI_LOG_I(TAG, "View dispatcher attached to GUI");
    
    FURI_LOG_I(TAG, "About to add view");
    view_dispatcher_add_view(view_dispatcher, 0, fa_bypass_main_view_get_view(main_view));
    FURI_LOG_I(TAG, "View added");
    
    FURI_LOG_I(TAG, "About to switch view");
    view_dispatcher_switch_to_view(view_dispatcher, 0);
    FURI_LOG_I(TAG, "View switched");
    
    FURI_LOG_I(TAG, "Starting event loop");
    // Main event loop
    view_dispatcher_run(view_dispatcher);
    
    FURI_LOG_I(TAG, "Exited event loop");
    
    // Cleanup
    view_dispatcher_remove_view(view_dispatcher, 0);
    view_dispatcher_free(view_dispatcher);
    fa_bypass_main_view_free(main_view);
    fa_bypass_app_free(app);
    
    FURI_LOG_I(TAG, "App cleanup complete");
    return 0;
}
