#include <furi.h>
#include "fa_bypass_app.h"

int32_t fa_bypass_app_entry(void* p) {
    return fa_bypass_app_run(p);
}
