#ifndef USBpanel_mode1_h_
#define USBpanel_mode1_h_

#include "usb_desc.h"

#if defined(PANEL_MODE1_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
  int usb_panel_mode1_send(void);
  extern uint32_t usb_panel_mode1_data[3];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_panel_mode1_class
{
public:
  void begin(void) { }
  void end(void) { }

  void button(unsigned int button, bool val) {
    if (--button >= PANEL_MODE1_MAX_BUTTON) return;

    // there are 32bit per array entry
    size_t ix = 0;
    while (button >= 32) {
      button -= 32;
      ix++;
    }

    if (ix > 2) {
      return;
    }

    if (val) usb_panel_mode1_data[ix] |= (1 << button);
    else usb_panel_mode1_data[ix] &= ~(1 << button);
    if (!manual_mode) usb_panel_mode1_send();
  }

  void useManualSend(bool mode) {
    manual_mode = mode;
  }
  void send_now(void) {
    usb_panel_mode1_send();
  }

  void reset(void) {
    memset(usb_panel_mode1_data, 0, sizeof(usb_panel_mode1_data));
  }

private:
  static uint8_t manual_mode;
  static uint8_t interface;
};
extern usb_panel_mode1_class PanelMode1;

#endif // __cplusplus

#endif // PANEL_MODE1_INTERFACE

#endif // USBpanel_mode1_h_

