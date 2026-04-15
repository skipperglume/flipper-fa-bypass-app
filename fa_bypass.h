#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <furi.h>


typedef enum {
    FABypassNotifyRegister,
    FABypassNotifyAuth,
    FABypassNotifyAuthSuccess,
    FABypassNotifyWink,
    FABypassNotifyConnect,
    FABypassNotifyDisconnect,
    FABypassNotifyError,
} FABypassNotifyEvent; // Notify Event

typedef struct FABypassData FABypassData;

typedef void (*FABypassEvtCallback)(FABypassNotifyEvent evt, void* context);

FABypassData* fa_bypass_alloc(void);

bool fa_bypass_init(FABypassData* instance);

void fa_bypass_free(FABypassData* instance);

void fa_bypass_set_event_callback(FABypassData* instance, FABypassEvtCallback callback, void* context);

void fa_bypass_confirm_user_present(FABypassData* instance);

uint16_t fa_bypass_msg_parse(FABypassData* instance, uint8_t* buf, uint16_t len);
void fa_bypass_wink(FABypassData* instance);
void fa_bypass_set_state(FABypassData* instance, uint8_t state);

#ifdef __cplusplus
}
#endif