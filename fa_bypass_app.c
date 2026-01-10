#include "fa_bypass_app_i.h"
// #include "u2f_data.h"
#include <furi.h>
#include <furi_hal.h>


static bool fa_bypass_app_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    FA_BypassApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool fa_bypass_app_back_event_callback(void* context) {
    furi_assert(context);
    FA_BypassApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void fa_bypass_app_tick_event_callback(void* context) {
    furi_assert(context);
    FA_BypassApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}


FA_BypassApp* fa_bypass_app_alloc(void) {
    FA_BypassApp* app = malloc(sizeof(FA_BypassApp));

    app->gui = furi_record_open(RECORD_GUI); // 
    app->notifications = furi_record_open(RECORD_NOTIFICATION); // 

    app->view_dispatcher = view_dispatcher_alloc();
    
    app->scene_manager = scene_manager_alloc(&fa_bypass_scene_handlers, app);



    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_tick_event_callback(
        app->view_dispatcher, fa_bypass_app_tick_event_callback, 500);

    view_dispatcher_set_custom_event_callback(app->view_dispatcher, fa_bypass_app_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, fa_bypass_app_back_event_callback);

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Custom Widget
    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, FA_BypassAppViewError, widget_get_view(app->widget));

    // app->u2f_view = u2f_view_alloc();
    // view_dispatcher_add_view(
    //     app->view_dispatcher, U2fAppViewMain, u2f_view_get_view(app->u2f_view));

    // if(furi_hal_usb_is_locked()) {
    //     app->error = U2fAppErrorCloseRpc;
    //     scene_manager_next_scene(app->scene_manager, U2fSceneError);
    // } else {
    //     if(u2f_data_check(true)) {
    //         scene_manager_next_scene(app->scene_manager, U2fSceneMain);
    //     } else {
    //         app->error = U2fAppErrorNoFiles;
    //         scene_manager_next_scene(app->scene_manager, U2fSceneError);
    //     }
    // }

    return app;
}


void fa_bypass_app_free(FA_BypassApp* app) {
    furi_assert(app);

    // Views
    view_dispatcher_remove_view(app->view_dispatcher, FA_BypassAppViewMain);
    // fa_bypass_view_free(app->u2f_view);

    // Custom Widget
    view_dispatcher_remove_view(app->view_dispatcher, FA_BypassAppViewError);
    widget_free(app->widget);

    // View dispatcher
    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    // Close records
    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);

    free(app);
}

// APP_NAME: fa_bypass_app
int32_t fa_bypass_app(void* p) {
    UNUSED(p);
    FA_BypassApp* fa_bypass_app = fa_bypass_app_alloc();

    view_dispatcher_run(fa_bypass_app->view_dispatcher);

    fa_bypass_app_free(fa_bypass_app);

    return 0;
}