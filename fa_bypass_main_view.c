#include "fa_bypass_main_view.h"
#include "fa_bypass_config.h"
#include <gui/elements.h>
#include <furi_hal_rtc.h>

#define TAG "FABypassMainView"

typedef struct {
    FABypassApp* app;
    char current_code[7];
    uint32_t time_remaining;
} FABypassMainViewState;

struct FABypassMainView {
    View* view;
    FABypassMainViewState* state;
};

static void fa_bypass_main_view_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    
    if(context == NULL) {
        canvas_clear(canvas);
        return;
    }
    
    FABypassMainViewState* state = context;

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "FA Bypass");

    // Draw TOTP code - read from app if available
    const char* code_to_display = "------";
    if(state->app != NULL) {
        code_to_display = state->app->current_code;
    }
    
    canvas_set_font(canvas, FontBigNumbers);
    canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, code_to_display);

    // Draw time remaining
    canvas_set_font(canvas, FontSecondary);
    uint32_t time_remaining = TOTP_PERIOD_SECONDS;
    if(state->app != NULL) {
        uint32_t timestamp = furi_hal_rtc_get_timestamp();
        time_remaining = TOTP_PERIOD_SECONDS - (timestamp % TOTP_PERIOD_SECONDS);
    }
    
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "Expires in: %lu s", 
             (unsigned long)time_remaining);
    canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignCenter, time_str);

    // Draw instructions
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignCenter, 
                            "Press OK to send via HID");
}

static bool fa_bypass_main_view_input_callback(InputEvent* event, void* context) {
    furi_assert(event);
    
    if(context == NULL) {
        return false;
    }
    
    FABypassMainViewState* state = (FABypassMainViewState*)context;
    UNUSED(state);

    if(event->type == InputTypeShort && event->key == InputKeyOk) {
        // TODO: Implement HID typing with user confirmation
        // Show confirmation dialog, then enable USB HID
        return true;
    }

    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        // Return to previous view/exit
        return false;
    }

    return false;
}

FABypassMainView* fa_bypass_main_view_alloc(FABypassApp* app) {
    FABypassMainView* main_view = malloc(sizeof(FABypassMainView));
    if(main_view == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate main_view");
        return NULL;
    }
    
    main_view->state = malloc(sizeof(FABypassMainViewState));
    if(main_view->state == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate main_view state");
        free(main_view);
        return NULL;
    }
    
    main_view->state->app = app;
    snprintf(main_view->state->current_code, sizeof(main_view->state->current_code), "------");
    main_view->state->time_remaining = TOTP_PERIOD_SECONDS;

    main_view->view = view_alloc();
    if(main_view->view == NULL) {
        FURI_LOG_E(TAG, "Failed to allocate view");
        free(main_view->state);
        free(main_view);
        return NULL;
    }
    
    view_set_context(main_view->view, main_view->state);
    view_set_draw_callback(main_view->view, fa_bypass_main_view_draw_callback);
    view_set_input_callback(main_view->view, fa_bypass_main_view_input_callback);

    return main_view;
}

void fa_bypass_main_view_free(FABypassMainView* view) {
    furi_assert(view);
    view_free(view->view);
    free(view->state);
    free(view);
}

View* fa_bypass_main_view_get_view(FABypassMainView* view) {
    furi_assert(view);
    return view->view;
}

void fa_bypass_main_view_update_code(FABypassMainView* view, const char* code) {
    furi_assert(view);
    furi_assert(code);
    
    FABypassMainViewState* state = view->state;
    snprintf(state->current_code, sizeof(state->current_code), "%s", code);
    
    // Calculate time remaining in current period
    uint32_t timestamp = furi_hal_rtc_get_timestamp();
    state->time_remaining = TOTP_PERIOD_SECONDS - (timestamp % TOTP_PERIOD_SECONDS);
    
    // Don't call view_commit_model from the update thread - just update the state
    // The draw callback will read this updated state on the next draw cycle
}
