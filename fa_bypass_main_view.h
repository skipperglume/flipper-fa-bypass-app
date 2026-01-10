#pragma once

#include <gui/view.h>
#include "fa_bypass_app.h"

typedef struct FABypassMainView FABypassMainView;

/**
 * @brief Allocate main view
 */
FABypassMainView* fa_bypass_main_view_alloc(FABypassApp* app);

/**
 * @brief Free main view
 */
void fa_bypass_main_view_free(FABypassMainView* view);

/**
 * @brief Get view handle
 */
View* fa_bypass_main_view_get_view(FABypassMainView* view);

/**
 * @brief Update displayed TOTP code
 */
void fa_bypass_main_view_update_code(FABypassMainView* view, const char* code);
