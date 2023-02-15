#include "rwm.h"
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>

static Key keys[] = {
    {XCB_MOD_MASK_1, 0x0071, close_wm, NULL} /* 0x0071 = XK_q */
};
