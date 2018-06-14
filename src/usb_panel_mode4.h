#ifndef USBpanel_mode4_h_
#define USBpanel_mode4_h_

#include "usb_desc.h"

#if defined(PANEL_MODE4_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
  int usb_panel_mode4_send(void);
  extern uint32_t usb_panel_mode4_data[1];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_panel_mode4_class
{
public:
  void begin(void) { }
  void end(void) { }

  void button(unsigned int button, bool val) {
    if (--button >= 29) return;
    if (val) usb_panel_mode4_data[0] |= (1 << button);
    else usb_panel_mode4_data[0] &= ~(1 << button);
    if (!manual_mode) usb_panel_mode4_send();
  }

  void useManualSend(bool mode) {
    manual_mode = mode;
  }
  void send_now(void) {
    usb_panel_mode4_send();
  }

  void reset(void) {
    memset(usb_panel_mode4_data, 0, sizeof(usb_panel_mode4_data));
  }

private:
  static uint8_t manual_mode;
  static uint8_t interface;
};
extern usb_panel_mode4_class PanelMode4;

#endif // __cplusplus

#endif // PANEL_MODE4_INTERFACE

#endif // USBpanel_mode4_h_

