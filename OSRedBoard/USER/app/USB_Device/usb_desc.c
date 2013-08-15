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

/* �豸������ */
const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
{
    0x12,                       /* ����Descriptor�ĳ��ȣ�18�ֽ�*/
    USB_DEVICE_DESCRIPTOR_TYPE, /* Descriptor�����Device Descriptor(0x01) */
    0x00,                       /* �豸����ѭ��USBЭ��İ汾�ţ�2.00 */
    0x02,
    0xff,                       /* �豸��ʵ�ֵ��ࣺ��ÿ���ӿ�������������ʵ�ֵ���*/
    0x00,                       /* �豸��ʵ�ֵ����ࣺ��ÿ���ӿ�����������*/
    0x00,                       /* �豸����ѭ��Э�������ÿ���ӿ�����������*/
    0x40,                       /* �˵�0��������ݰ����ȣ�64�ֽ�*/
    0xFF,                       /*idVendor (0x0483)  ���̱��*/
    0xDD,
    0x25,                       /*idProduct = 0x5750  ��Ʒ���*/
    0x30,
    0x00,                       /*bcdDevice rel. 2.00  �豸�������*/
    0x02,
    1,                          /*Index of string descriptor describing
                                              manufacturer */
    2,                          /*Index of string descriptor describing
                                             product*/
    3,                          /*Index of string descriptor describing the
                                             device serial number */
    0x01                        /* �豸��֧�ֵ�������Ŀ��1*/
}; 								/* CustomHID_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */

const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
  {
    /************����������*****************/
	0x09, /* �������ĳ��ȣ�9�ֽ�*/
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* ������������: 0x02 ����������(Configuration) */
    CUSTOMHID_SIZ_CONFIG_DESC,	 /* �����������������ӿ����������˵������������������ĳ���*/
    0x00,
    0x01,         /* ������֧�ֵĽӿ���Ŀ��1*/
    0x01,         /* ��SetConofiguration()ѡ������ã���ָ�������úţ�1*/
    0x00,         /* �������������õ��ַ��������������ţ�0 */
    0xC0,         /* �������ã�B7(1 ����), B6(�Թ���), B5(Զ�̻���), B4-B0(0 ����) */
    0x32,         /* ��󹦺ģ���2mAΪ��λ���㣺0x32��ʾ50��2 ��100mA */

    /************** �ӿ������� ****************/
    /* 09 */
    0x09,         /* �������ĳ��ȣ�9�ֽ�*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/* �����������ͣ�0x04�ӿ�������(Interface) */
    0x00,         /* ѡ��˽ӿڵ������ţ���0��ʼ���㣺0 */
    0x00,         /* ����ѡ������õ������ţ�0 */
    0x04,         /* ʵ�ִ˽ӿ���Ҫʹ�õĶ˵���Ŀ��4 */
    0x0a,         /* �˽ӿ�����ѭ���ࣺHID Class */
    0x00,         /* �˽ӿ�����ѭ�����ࣺ1=BOOT, 0=no boot: requiring BIOS support */
    0x00,         /* �˽ӿ���֧�ֵ�Э�飺0���Զ��塢1�����̡�2�����*/
    0,            /* ���������˽ӿڵ��ַ���������������*/
    /******************** �������� Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* ���������ȣ�9�ֽ�*/
    HID_DESCRIPTOR_TYPE, /* ���������ͣ�HID��������*/
    0x10,         /* ����ѭ��HIDЭ��汾��1.10 */
    0x01,
    0x00,         /* ���Ҵ��룺��*/
    0x01,         /* �����ඨ�壬��������Ҫ������������Ŀ��1*/
    0x22,         /* �����������������ͣ�����������*/
    CUSTOMHID_SIZ_REPORT_DESC,/* �������������ĳ��ȣ�*/
    0x00,
    /******************** �˵������� Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,         /* ���������ȣ�7�ֽ�*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* ���������ͣ��˵�������*/

    0x01,          /* �˵�����ԣ�B3-B0(�˵��), B6-B4(0), B7(1=IN, 0=OUT): 0x01��Endpoint1/ OUT*/
    0x03,          /* �˵�����ͣ�B1-B0(00=����01=ͬ��10=������11=�ж�): 0x03���ж϶˵�*/
    0x10,          /* �˶˵�������Ч���ݳ��ȣ�16�ֽ�*/
    0x00,
    0x20,          /* ������ѯ�˶˵����ݵļ��ʱ�䣺(1ms��125us��λ): 0x20��32 ms */
    /* 34 */
    	
    0x07,	/* ���������ȣ�7�ֽ�*/
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* ���������ͣ��˵�������*/
    0x81,	/* �˵�����ԣ�B3-B0(�˵��), B6-B4(0), B7(1=IN, 0=OUT): 0x81��Endpoint1/ IN */
    0x03,	/* �˵�����ͣ�B1-B0(00=����01=ͬ��10=������11=�ж�): 0x03���ж϶˵�*/
    0x10,	/* �˶˵�������Ч���ݳ��ȣ�16�ֽ�*/
    0x00,
    0x20,	/* ������ѯ�˶˵����ݵļ��ʱ�䣺(1ms��125us��λ): 0x20��32 ms */
    /* 41 */
	0x07,          /* ���������ȣ�7�ֽ�*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* ���������ͣ��˵�������*/

    0x02,          /* �˵�����ԣ�B3-B0(�˵��), B6-B4(0), B7(1=IN, 0=OUT): 0x02��Endpoint2/ OUT */
    0x02,          /* �˵�����ͣ�B1-B0(00=����01=ͬ��10=������11=�ж�): 0x02���������˵�*/
    0x40,          /* �˶˵�������Ч���ݳ��ȣ�64�ֽ�*/
    0x00,
    0x20,          /* ������ѯ�˶˵����ݵļ��ʱ�䣺(1ms��125us��λ): 0x20��32 ms */
	/* 48 */
	0x07,          /* ���������ȣ�7�ֽ�*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* ���������ͣ��˵�������*/

    0x82,          /* �˵�����ԣ�B3-B0(�˵��), B6-B4(0), B7(1=IN, 0=OUT): 0x82��Endpoint2/ IN */
    0x02,          /* �˵�����ͣ�B1-B0(00=����01=ͬ��10=������11=�ж�): 0x03���ж϶˵�*/
    0x40,          /* �˶˵�������Ч���ݳ��ȣ�64�ֽ�*/
    0x00,
    0x20,          /* ������ѯ�˶˵����ݵļ��ʱ�䣺(1ms��125us��λ): 0x20��32 ms */
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

