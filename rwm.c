// we should include this in everything "xcb"
#include <xcb/xcb.h>
#include <xcb/xproto.h>

// main function
int main(int argc, char *argv[]) {
  // we first initialize a few global variables that will be commonly
  // used throughout the code, but that's optional.

  // as we will see later, this one is used to fill the values of value masks
  // for specific xcb functions, that's how xcb/xlib works :/
  // And uint32 just means that its a 32 bit integer and its unsigned (can't be
  // a negative value iirc)
  uint32_t values[3];

  // for every xcb (and xlib) program we have to establish a connection to the X
  // server
  xcb_connection_t *connection;

  // we also need a screen (that's not a window)
  xcb_screen_t *screen;

  // now that's the window, and its logically a drawable thing since we will map
  // and unmap it..etc, so that's why we give it that type
  xcb_drawable_t win;

  // a root window is the parent of all windows, its initial size is the
  // dimensions of your monitor
  xcb_drawable_t root;

  // in the event loop, we will receieve events that we have to respond to,
  // these events are of type xcb_generic_event_t, we just initialize a variable
  // here for simplicity
  xcb_generic_event_t *ev;

  // we open the connection to the X server using that function, simple and
  // straightforward.
  connection = xcb_connect(NULL, NULL);

  // if anything's wrong with the connection just return an exit error of 1
  if (xcb_connection_has_error(connection))
    return 1;

  // here we tell xcb to send us events any moment the keybinds we set up below
  // are triggered

  return 0;
}
