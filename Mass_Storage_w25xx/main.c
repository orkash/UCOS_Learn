#include "stm32f10x.h"

extern void USB_cable(void);
int main(void)
{
    SystemInit();
    USB_cable();

    while(1);
}
