/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Descriptors for Custom HID Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* 设备描述符 */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
{
    0x12,                       /* 整个Descriptor的长度：18字节*/
    USB_DEVICE_DESCRIPTOR_TYPE, /* Descriptor的类别：Device Descriptor(0x01) */
    0x00,                       /* 设备所遵循的USB协议的版本号：2.00 */
    0x02,
    0xff,                       /* 设备所实现的类：由每个接口描述符描述所实现的类*/
    0x00,                       /* 设备所实现的子类：由每个接口描述符描述*/
    0x00,                       /* 设备所遵循的协议类别：由每个接口描述符描述*/
    0x40,                       /* 端点0的最大数据包长度：64字节*/
    0xFF,                       /*idVendor (0x0483)  厂商编号*/
    0xDD,
    0x25,                       /*idProduct = 0x5750  产品编号*/
    0x30,
    0x00,                       /*bcdDevice rel. 2.00  设备出厂编号*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /* 设备所支持的配置数目：1*/
}; 								/* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */

const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    /************配置描述符*****************/
	0x09, /* 描述符的长度：9字节*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* 描述符的类型: 0x02 配置描述符(Configuration) */
    CUSTOMHID_SIZ_CONFIG_DESC,	 /* 完整的描述符包括接口描述符、端点描述符和类描述符的长度*/
    0x00,
    0x01,         /* 配置所支持的接口数目：1*/
    0x01,         /* 用SetConofiguration()选择此配置，所指定的配置号：1*/
    0x00,         /* 用于描述此配置的字符描述符的索引号：0 */
    0xC0,         /* 供电配置：B7(1 保留), B6(自供电), B5(远程唤醒), B4-B0(0 保留) */
    0x32,         /* 最大功耗，以2mA为单位计算：0x32表示50×2 ＝100mA */

    /************** 接口描述符 ****************/
    /* 09 */
    0x09,         /* 描述符的长度：9字节*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/* 描述符的类型：0x04接口描述符(Interface) */
    0x00,         /* 选择此接口的索引号，从0开始计算：0 */
    0x00,         /* 用于选择此设置的索引号：0 */
    0x04,         /* 实现此接口需要使用的端点数目：4 */
    0x0a,         /* 此接口所遵循的类：HID Class */
    0x00,         /* 此接口所遵循的子类：1=BOOT, 0=no boot: requiring BIOS support */
    0x00,         /* 此接口所支持的协议：0：自定义、1：键盘、2：鼠标*/
    0,            /* 用于描述此接口的字符描述符的索引号*/
    /******************** 类描述符 Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* 描述符长度：9字节*/
    HID_DESCRIPTOR_TYPE, /* 描述符类型：HID类描述符*/
    0x10,         /* 所遵循的HID协议版本：1.10 */
    0x01,
    0x00,         /* 国家代码：无*/
    0x01,         /* 按照类定义，后续所需要的描述符的数目：1*/
    0x22,         /* 后续的描述符的类型：报告描述符*/
    CUSTOMHID_SIZ_REPORT_DESC,/* 后续的描述符的长度：*/
    0x00,
    /******************** 端点描述符 Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,         /* 描述符长度：7字节*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* 描述符类型：端点描述符*/

    0x01,          /* 端点的特性：B3-B0(端点号), B6-B4(0), B7(1=IN, 0=OUT): 0x01：Endpoint1/ OUT*/
    0x03,          /* 端点的类型：B1-B0(00=控制01=同步10=大容量11=中断): 0x03：中断端点*/
    0x10,          /* 此端点的最大有效数据长度：16字节*/
    0x00,
    0x20,          /* 主机查询此端点数据的间隔时间：(1ms或125us单位): 0x20：32 ms */
    /* 34 */
    	
    0x07,	/* 描述符长度：7字节*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* 描述符类型：端点描述符*/
    0x81,	/* 端点的特性：B3-B0(端点号), B6-B4(0), B7(1=IN, 0=OUT): 0x81：Endpoint1/ IN */
    0x03,	/* 端点的类型：B1-B0(00=控制01=同步10=大容量11=中断): 0x03：中断端点*/
    0x10,	/* 此端点的最大有效数据长度：16字节*/
    0x00,
    0x20,	/* 主机查询此端点数据的间隔时间：(1ms或125us单位): 0x20：32 ms */
    /* 41 */
	0x07,          /* 描述符长度：7字节*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* 描述符类型：端点描述符*/

    0x02,          /* 端点的特性：B3-B0(端点号), B6-B4(0), B7(1=IN, 0=OUT): 0x02：Endpoint2/ OUT */
    0x02,          /* 端点的类型：B1-B0(00=控制01=同步10=大容量11=中断): 0x02：大容量端点*/
    0x40,          /* 此端点的最大有效数据长度：64字节*/
    0x00,
    0x20,          /* 主机查询此端点数据的间隔时间：(1ms或125us单位): 0x20：32 ms */
	/* 48 */
	0x07,          /* 描述符长度：7字节*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* 描述符类型：端点描述符*/

    0x82,          /* 端点的特性：B3-B0(端点号), B6-B4(0), B7(1=IN, 0=OUT): 0x82：Endpoint2/ IN */
    0x02,          /* 端点的类型：B1-B0(00=控制01=同步10=大容量11=中断): 0x03：中断端点*/
    0x40,          /* 此端点的最大有效数据长度：64字节*/
    0x00,
    0x20,          /* 主机查询此端点数据的间隔时间：(1ms或125us单位): 0x20：32 ms */
	/* 55 */
  }
  ; /* CustomHID_ConfigDescriptor */
const uint8_t CustomHID_ReportDescriptor[CUSTOMHID_SIZ_REPORT_DESC] =
  {    
    0x05, 0x8c,            /* USAGE_PAGE (ST Page)           */                   
    0x09, 0x01,            /* USAGE (Demo Kit)               */    
    0xa1, 0x01,            /* COLLECTION (Application)       */            
    /* 6 */
    
    /* Led 1 */        
    0x85, 0x01,            /*     REPORT_ID (1)		     */
    0x09, 0x01,            /*     USAGE (LED 1)	             */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
    0x75, 0x08,            /*     REPORT_SIZE (8)            */        
    0x95, 0x01,            /*     REPORT_COUNT (1)           */       
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     

    0x85, 0x01,            /*     REPORT_ID (1)              */
    0x09, 0x01,            /*     USAGE (LED 1)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 26 */
    
    /* Led 2 */
    0x85, 0x02,            /*     REPORT_ID 2		     */
    0x09, 0x02,            /*     USAGE (LED 2)	             */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
    0x75, 0x08,            /*     REPORT_SIZE (8)            */        
    0x95, 0x01,            /*     REPORT_COUNT (1)           */       
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     

    0x85, 0x02,            /*     REPORT_ID (2)              */
    0x09, 0x02,            /*     USAGE (LED 2)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 46 */
    
    /* Led 3 */        
    0x85, 0x03,            /*     REPORT_ID (3)		     */
    0x09, 0x03,            /*     USAGE (LED 3)	             */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
    0x75, 0x08,            /*     REPORT_SIZE (8)            */        
    0x95, 0x01,            /*     REPORT_COUNT (1)           */       
    0xB1, 0x82,             /*    FEATURE (Data,Var,Abs,Vol) */     

    0x85, 0x03,            /*     REPORT_ID (3)              */
    0x09, 0x03,            /*     USAGE (LED 3)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 66 */
    
    /* Led 4 */
    0x85, 0x04,            /*     REPORT_ID 4)		     */
    0x09, 0x04,            /*     USAGE (LED 4)	             */
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */          
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */           
    0x75, 0x08,            /*     REPORT_SIZE (8)            */        
    0x95, 0x01,            /*     REPORT_COUNT (1)           */       
    0xB1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */     

    0x85, 0x04,            /*     REPORT_ID (4)              */
    0x09, 0x04,            /*     USAGE (LED 4)              */
    0x91, 0x82,            /*     OUTPUT (Data,Var,Abs,Vol)  */
    /* 86 */
    
    /* key Push Button */  
    0x85, 0x05,            /*     REPORT_ID (5)              */
    0x09, 0x05,            /*     USAGE (Push Button)        */      
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */      
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */      
    0x75, 0x01,            /*     REPORT_SIZE (1)            */  
    0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */   
    
    0x09, 0x05,            /*     USAGE (Push Button)        */               
    0x75, 0x01,            /*     REPORT_SIZE (1)            */           
    0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */  
         
    0x75, 0x07,            /*     REPORT_SIZE (7)            */           
    0x81, 0x83,            /*     INPUT (Cnst,Var,Abs,Vol)   */                    
    0x85, 0x05,            /*     REPORT_ID (2)              */         
                    
    0x75, 0x07,            /*     REPORT_SIZE (7)            */           
    0xb1, 0x83,            /*     FEATURE (Cnst,Var,Abs,Vol) */                      
    /* 114 */

    /* Tamper Push Button */  
    0x85, 0x06,            /*     REPORT_ID (6)              */
    0x09, 0x06,            /*     USAGE (Tamper Push Button) */      
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */      
    0x25, 0x01,            /*     LOGICAL_MAXIMUM (1)        */      
    0x75, 0x01,            /*     REPORT_SIZE (1)            */  
    0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */   
    
    0x09, 0x06,            /*     USAGE (Tamper Push Button) */               
    0x75, 0x01,            /*     REPORT_SIZE (1)            */           
    0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */  
         
    0x75, 0x07,            /*     REPORT_SIZE (7)            */           
    0x81, 0x83,            /*     INPUT (Cnst,Var,Abs,Vol)   */                    
    0x85, 0x06,            /*     REPORT_ID (6)              */         
                    
    0x75, 0x07,            /*     REPORT_SIZE (7)            */           
    0xb1, 0x83,            /*     FEATURE (Cnst,Var,Abs,Vol) */  
    /* 142 */
    
    /* ADC IN */
    0x85, 0x07,            /*     REPORT_ID (7)              */         
    0x09, 0x07,            /*     USAGE (ADC IN)             */          
    0x15, 0x00,            /*     LOGICAL_MINIMUM (0)        */               
    0x26, 0xff, 0x00,      /*     LOGICAL_MAXIMUM (255)      */                 
    0x75, 0x08,            /*     REPORT_SIZE (8)            */           
    0x81, 0x82,            /*     INPUT (Data,Var,Abs,Vol)   */                    
    0x85, 0x07,            /*     REPORT_ID (7)              */                 
    0x09, 0x07,            /*     USAGE (ADC in)             */                     
    0xb1, 0x82,            /*     FEATURE (Data,Var,Abs,Vol) */                                 
    /* 161 */

    0xc0 	          /*     END_COLLECTION	             */
  }; /* CustomHID_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
  {
    CUSTOMHID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
  {
    CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'F', 0, 'D', 0, '3', 0, '0', 0, '2', 0, '5', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
  {
    CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'H', 0, 'D', 0, 'K', 0, 'J', 0, '-', 0,
    'H', 0, 'D', 0, '3', 0, '0', 0, '2', 0, '5', 0
  };
uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
  {
    CUSTOMHID_SIZ_STRING_SERIAL,            /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'F', 0, 'D', 0, '3', 0,'3', 0,'2', 0, '1', 0, '0', 0
  };

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

