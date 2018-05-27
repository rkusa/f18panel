#ifndef USBpanel_mode3_h_
#define USBpanel_mode3_h_

#include "usb_desc.h"

#if defined(PANEL_MODE3_INTERFACE)

#include <inttypes.h>

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
  int usb_panel_mode3_send(void);
  extern uint8_t usb_panel_mode3_data[4];
#ifdef __cplusplus
}
#endif

// C++ interface
#ifdef __cplusplus
class usb_panel_mode3_class
{
public:
  void begin(void) { }
  void end(void) { }

  void button(unsigned int num, bool val) {
    if (--num >= 87) return;
    uint8_t *p = usb_panel_mode3_data + (num >> 5);
    num &= 0x1F;
    if (val) *p |= (1 << num);
    else *p &= ~(1 << num);
    if (!manual_mode) usb_panel_mode3_send();
  }

  void useManualSend(bool mode) {
    manual_mode = mode;
  }
  void send_now(void) {
    usb_panel_mode3_send();
  }

  void reset(void) {
    memset(usb_panel_mode3_data, 0, sizeof(usb_panel_mode3_data));
  }

private:
  static uint8_t manual_mode;
  static uint8_t interface;
};
extern usb_panel_mode3_class PanelMode3;

#endif // __cplusplus

#endif // PANEL_MODE3_INTERFACE

#endif // USBpanel_mode3_h_
