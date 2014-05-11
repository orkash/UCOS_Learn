/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Virtual Com Port Demo main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
static vu32 TimingDelay = 0; 
extern u32 count_out;
extern u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{

#ifdef DEBUG
  debug();
#endif

  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();

  /* Configure the systick */    
  SysTick_Config();
  /* Initialize the LCD */
  STM3210E_LCD_Init();
  /* Clear the LCD */ 
  LCD_Clear(White);
  LCD_SetBackColor(Blue);
  /* Set the LCD Text Color */
  LCD_SetTextColor(White); 

  LCD_DisplayStringLine(Line0, "   EM_STM32E Demo   ");
  LCD_DisplayStringLine(Line1, "   Visual_Com_Test  ");
  LCD_DisplayStringLine(Line2, " welcome to Embest  ");
  LCD_DisplayStringLine(Line3, " www.embedinfo.com  ");
  LCD_SetBackColor(White);                        // Set the Back Color */
  LCD_SetTextColor(Blue);                         // Set the Text Color */
  LCD_DisplayStringLine(Line5, "        V_COM OK    ");
  LCD_DisplayStringLine(Line5, "  Check V_COM On PC ");

  while (1)
  {
    if ((count_out != 0) && (bDeviceState == CONFIGURED))
    {
      USB_To_USART_Send_Data(&buffer_out[0], count_out);
      count_out = 0;
    }
  }
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configure a SysTick Base time to 10 ms.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 void SysTick_Config(void)
{
  /* Configure HCLK clock as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
 
  /* SysTick interrupt each 100 Hz with HCLK equal to 72MHz */
  SysTick_SetReload(720000);

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length (time base 10 ms).
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);

  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Decrement_TimingDelay
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void Decrement_TimingDelay(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
