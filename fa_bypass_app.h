#pragma once

#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <notification/notification.h>

// Forward declaration
typedef struct FABypassMainView FABypassMainView;

typedef struct {
    Gui* gui;
    NotificationApp* notifications;
    FuriThread* update_thread;
    volatile bool running;
    char current_code[7];
    uint32_t last_update;
    FABypassMainView* main_view;
} FABypassApp;

/**
 * @brief Allocate and initialize the app
 * @return Pointer to FABypassApp, or NULL on failure
 */
FABypassApp* fa_bypass_app_alloc(void);

/**
 * @brief Free app resources
 */
void fa_bypass_app_free(FABypassApp* app);

/**
 * @brief Start the app
 */
int32_t fa_bypass_app_run(void* p);
