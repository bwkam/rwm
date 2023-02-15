#include "rwm.h"
#include "config.h"
#include <stdint.h>
#include <sys/wait.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>

static xcb_connection_t *conn;
static xcb_drawable_t *win;
static xcb_screen_t *screen;
static xcb_generic_event_t *ev;
static uint32_t values[3];

static void spawn(char **com) {
  if (fork() == 0) {
    setsid();
    if (fork() != 0) {
      _exit(0);
    }
    execvp((char *)com[0], (char **)com);
    _exit(0);
  }
  wait(NULL);
}

static xcb_keycode_t *xcb_get_keycodes(xcb_keysym_t keysym) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keycode_t *keycode;
  keycode = (!(keysyms) ? NULL : xcb_key_symbols_get_keycode(keysyms, keysym));
  xcb_key_symbols_free(keysyms);
  return keycode;
}

static xcb_keysym_t xcb_get_keysym(xcb_keycode_t keycode) {
  xcb_key_symbols_t *keysyms = xcb_key_symbols_alloc(conn);
  xcb_keysym_t keysym;
  keysym = (!(keysyms) ? 0 : xcb_key_symbols_get_keysym(keysyms, keycode, 0));
  xcb_key_symbols_free(keysyms);
  return keysym;
}

static void set_focus(xcb_drawable_t window) {
  if ((window != 0) && (window != screen->root)) {
    xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT, window,
                        XCB_CURRENT_TIME);
  }
}

static void close_wm(void) {
  if (conn != NULL) {
    xcb_disconnect(conn);
  }
}

static void init_wm(void) {
  conn = xcb_connect(NULL, NULL);
  screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

  values[0] =
      XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
      XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | XCB_EVENT_MASK_PROPERTY_CHANGE;

  xcb_change_window_attributes(conn, screen->root, XCB_CW_EVENT_MASK, values);

  xcb_ungrab_key(conn, XCB_GRAB_ANY, screen->root, XCB_MOD_MASK_ANY);

  int key_table_size = sizeof(keys) / sizeof(*keys);
  for (int i = 0; i < key_table_size; ++i) {
    xcb_keycode_t *keycode = xcb_get_keycodes(keys[i].keysym);
    if (keycode != NULL) {
      xcb_grab_key(conn, 1, screen->root, keys[i].mod, *keycode,
                   XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC);
    }
  }

  xcb_flush(conn);

  // bindings
  xcb_grab_button(conn, 0, screen->root,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, screen->root,
                  XCB_NONE, 1, XCB_MOD_MASK_1);
  xcb_grab_button(conn, 0, screen->root,
                  XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE,
                  XCB_GRAB_MODE_ASYNC, XCB_GRAB_MODE_ASYNC, screen->root,
                  XCB_NONE, 3, XCB_MOD_MASK_1);
  xcb_flush(conn);
}

int main(int argc, char *argv[]) {
  init_wm();
  for (;;) {
    ev = xcb_wait_for_event(conn);
    switch (ev->response_type & ~0x80) {
    case XCB_MAP_REQUEST: {
      xcb_map_request_event_t *e = (xcb_map_request_event_t *)ev;
      xcb_map_window(conn, e->window);
      uint32_t vals[4];
      vals[0] = (screen->width_in_pixels / 2) - (600 / 2);
      vals[1] = (screen->height_in_pixels / 2) - (400 / 2);
      vals[2] = 600;
      vals[3] = 400;

      xcb_configure_window(conn, e->window,
                           XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y |
                               XCB_CONFIG_WINDOW_WIDTH |
                               XCB_CONFIG_WINDOW_HEIGHT,
                           vals);
      xcb_flush(conn);

      values[0] = XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_FOCUS_CHANGE;

      xcb_change_window_attributes(conn, e->window, XCB_CW_EVENT_MASK, values);
      set_focus(e->window);
    } break;
    case XCB_KEY_PRESS: {
      xcb_key_press_event_t *e = (xcb_key_press_event_t *)ev;
      xcb_keysym_t keysym = xcb_get_keysym(e->detail);
      *win = e->child;
      int key_table_size = sizeof(keys) / sizeof(*keys);
      for (int i = 0; i < key_table_size; ++i) {
        if ((keys[i].keysym == keysym) && (keys[i].mod == e->state)) {
          keys[i].func(keys[i].com);
        }
      }
    } break;
      free(ev);
    }
  }
  return 0;
}
