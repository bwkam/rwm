#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

typedef struct {
  unsigned int mod;
  xcb_keysym_t keysym;
  void (*func)(char **com);
  char **com;
} Key;

static xcb_keycode_t *xcb_get_keycodes(xcb_keysym_t keysym);
static xcb_keysym_t xcb_get_keysym(xcb_keycode_t keycode);

static void init_wm(void);
static void set_focus(xcb_drawable_t window);
static void close_wm(void);
