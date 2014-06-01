/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_istr.h"
/*
#include "memory.h"
#include "user_type.h"
#include "user_cfg.h"
#include "fm25v10.h"
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


//USBCmd RcvCmd;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_OUT_Callback(void)
{
    uint8_t Receive_Buffer[8];
    PMAToUserBufferCopy(Receive_Buffer, ENDP1_RXADDR, 2);
    SetEPRxStatus(ENDP1, EP_RX_VALID);
}

//Uint16 count_out = 0;
/*******************************************************************************
* Function Name  : EP2_OUT_Callback.
* Description    : EP2 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
/*    USBCmd SendCmd;
//	count_out = GetEPRxCount(ENDP2);

	PMAToUserBufferCopy(&RcvCmd.Cmd, ENDP2_RXADDR, 8);

	SendCmd.CmdPar[0] = ~RcvCmd.CmdPar[0];
	SendCmd.CmdPar[1] = ~RcvCmd.CmdPar[1];
	SendCmd.CmdPar[2] = ~RcvCmd.CmdPar[2];
	SendCmd.CmdPar[3] = ~RcvCmd.CmdPar[3];
	SendCmd.CmdPar[4] = ~RcvCmd.CmdPar[4];
	SendCmd.CmdPar[5] = ~RcvCmd.CmdPar[5];
	SendCmd.CmdPar[6] = ~RcvCmd.CmdPar[6];
    
    if(SysDat.Mode == RemoteControl_Mode){
        SendCmd.CmdPar[1] = 0xaa;
        SendCmd.CmdPar[2] = 0xaa;
    }
    
	UserToPMABufferCopy(&SendCmd.Cmd, ENDP2_TXADDR, 8);
	SetEPTxCount(ENDP2, 8);
  	SetEPTxValid(ENDP2);
    
	SetEPRxStatus(ENDP2, EP_RX_VALID);
    count_out = 0;  //init 
	return;
    */

}

/*******************************************************************************
* Function Name  : EP1_IN_Callback.
* Description    : EP1 IN Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
	return;	
}

/*******************************************************************************
* Function Name  : EP2_IN_Callback.
* Description    : EP2 IN Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_IN_Callback(void)
{
    /*
    USBSendDat SendDat;
	switch(RcvCmd.Cmd){
        case USB_PAR_CMD:
            UserToPMABufferCopy(&SaveSYSPar.SYSCheckFlag1, ENDP2_TXADDR, sizeof(SaveSYSParameter));
            SetEPTxCount(ENDP2, sizeof(SaveSYSParameter));
            break;
        case USB_DATA_CMD:
            FM25V10_DISK_Read(MeasureParameterAddr + (count_out << 6),(uint8_t *)SendDat.USBSendBuf,sizeof(USBSendDat));
            UserToPMABufferCopy((uint8_t *)SendDat.USBSendBuf, ENDP2_TXADDR, sizeof(USBSendDat));
            SetEPTxCount(ENDP2, sizeof(USBSendDat));
            count_out ++;
            break;
        default : break;
    }       
  	SetEPTxValid(ENDP2);
	return;	
    */
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

