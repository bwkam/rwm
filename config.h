#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>

static char *menucmd[] = {"dmenu_run", NULL};

static Key keys[] = {
    {XCB_MOD_MASK_1, 0x0071, close_wm, NULL}, /* 0x0071 = XK_q */
    {XCB_MOD_MASK_1, 0x0020, spawn, menucmd}};
