#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"
#include "usb_pwr.h"

void USB_cable(void)
{
    /* usb msc up*/
    Set_System();
    Set_USBClock();
    Led_Config();
    USB_Interrupts_Config();
    USB_Init();
}
