#pragma once

#include "fa_bypass_app.h"
#include <gui/gui.h>

// #include <assets_icons.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <dialogs/dialogs.h>
#include <notification/notification_messages.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/widget.h>

// #include "u2f_hid.h"
// #include "u2f.h"

// #include "views/u2f_view.h"
#include "scenes/fa_bypass_scene.h"

// APP_NAME: fa_bypass_app || FA_BypassApp
typedef enum {
    FA_BypassApp_ErrorNoFiles,
    FA_BypassApp_ErrorCloseRpc,
} FA_BypassApp_Error;


typedef enum {
    FA_BypassCustomEventNone,

    FA_BypassCustomEventConnect,
    FA_BypassCustomEventDisconnect,
    FA_BypassCustomEventDataError,

    FA_BypassCustomEventRegister,
    FA_BypassCustomEventAuth,
    FA_BypassCustomEventAuthSuccess,
    FA_BypassCustomEventWink,

    FA_BypassCustomEventTimeout,

    FA_BypassCustomEventConfirm,

    FA_BypassCustomEventErrorBack,

} GpioCustomEvent;


typedef enum {
    FA_BypassAppViewError,
    FA_BypassAppViewMain,
} FA_BypassAppView;


struct FA_BypassApp {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    NotificationApp* notifications;
    Widget* widget;
    FuriTimer* timer;
    GpioCustomEvent event_cur;
    bool u2f_ready;
    FA_BypassApp_Error error;
    
    // U2fHid* u2f_hid;
    // U2fView* u2f_view;
    // U2fData* u2f_instance;
};