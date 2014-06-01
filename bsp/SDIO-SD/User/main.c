#include "stm32f10x.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_usart1.h"	

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


#define BLOCK_SIZE            512 /* Block Size in Bytes */

#define NUMBER_OF_BLOCKS      32  /* For Multi Blocks operation (Read/Write) */
#define MULTI_BUFFER_SIZE    (BLOCK_SIZE * NUMBER_OF_BLOCKS)   //��������С	 


uint8_t Buffer_Block_Tx[BLOCK_SIZE], Buffer_Block_Rx[BLOCK_SIZE];
uint8_t readbuff[BLOCK_SIZE];
uint8_t Buffer_MultiBlock_Tx[MULTI_BUFFER_SIZE], Buffer_MultiBlock_Rx[MULTI_BUFFER_SIZE];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;
extern SD_CardInfo SDCardInfo;	
int i;
 
void SD_EraseTest(void);
void SD_SingleBlockTest(void);
void SD_MultiBlockTest(void);
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength);

int main(void)
{									   
	/* �жϳ�ʼ�� */
	NVIC_Configuration();

	/* ���ڳ�ʼ�� */
	USART1_Config();	
	printf( "\r\n ����һ��SD�� 4bit dma ʵ��\r\n " );

	/* SD ����ʼ�� */
	Status = SD_Init();
	
	if(Status == SD_OK) 
	{    
		printf( " \r\n SD_Init ��ʼ���ɹ� \r\n " );		
	}
	else
	{
		printf("\r\n SD_Init ��ʼ��ʧ�� \r\n" );
		printf("\r\n ���ص�Status��ֵΪ�� %d \r\n",Status );
	}			  	

	printf( " \r\n CardType is ��%d ", SDCardInfo.CardType );
	printf( " \r\n CardCapacity is ��%d ", SDCardInfo.CardCapacity );
	printf( " \r\n CardBlockSize is ��%d ", SDCardInfo.CardBlockSize );
	printf( " \r\n RCA is ��%d ", SDCardInfo.RCA);
	printf( " \r\n ManufacturerID is ��%d \r\n", SDCardInfo.SD_cid.ManufacturerID );

	/* �������� */
	SD_EraseTest();
	
  /* �����д���� */
	SD_SingleBlockTest(); 
	
	/* ����д���� */	
	SD_MultiBlockTest();  

	while (1)
	{}
}


/*
 * ��������SD_EraseTest
 * ����  ���������ݲ���
 * ����  ����
 * ���  ����
 */
void SD_EraseTest(void)
{
  if (Status == SD_OK)
  {    
		/* ��һ������Ϊ������ʼ��ַ���ڶ�������Ϊ����������ַ */
    Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
  }

  if (Status == SD_OK)
  {	/* ��ȡ�ող��������� */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);

    /* ��ѯ�����Ƿ���� */
    Status = SD_WaitReadOperation(); 
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }
 
  if (Status == SD_OK)
  {	/* �Ѳ�������������Ա� */
    EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(EraseStatus == PASSED)
  	printf("\r\n �������Գɹ��� " );
 
  else	  
  	printf("\r\n ��������ʧ�ܣ� " );  
}

/*
 * ��������SD_SingleBlockTest
 * ����  ��	�������ݿ��д����
 * ����  ����
 * ���  ����
 */
void SD_SingleBlockTest(void)
{  
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

  if (Status == SD_OK)
  {
    /* Write block of 512 bytes on address 0 */
    Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
		
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();	  
    while(SD_GetStatus() != SD_TRANSFER_OK); 
  }

  if (Status == SD_OK)
  {
    /* Read block of 512 bytes from address 0 */
    Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);//��ȡ����
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus1 = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE);	//�Ƚ�
  }
  
  if(TransferStatus1 == PASSED)
    printf("\r\n �����д���Գɹ���" );
 
  else  
  	printf("\r\n �����д����ʧ�ܣ� " );  
}

/*
 * ��������SD_MultiBlockTest
 * ����  ��	�����ݿ��д����
 * ����  ����
 * ���  ����
 */
void SD_MultiBlockTest(void)
{
  /* Fill the buffer to send */
  Fill_Buffer(Buffer_MultiBlock_Tx, MULTI_BUFFER_SIZE, 0x0);

  if (Status == SD_OK)
  {
    /* Write multiple block of many bytes on address 0 */
    Status = SD_WriteMultiBlocks(Buffer_MultiBlock_Tx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitWriteOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  if (Status == SD_OK)
  {
    /* Read block of many bytes from address 0 */
    Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00, BLOCK_SIZE, NUMBER_OF_BLOCKS);
    /* Check if the Transfer is finished */
    Status = SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
  }

  /* Check the correctness of written data */
  if (Status == SD_OK)
  {
    TransferStatus2 = Buffercmp(Buffer_MultiBlock_Tx, Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
  }
  
  if(TransferStatus2 == PASSED)	  
  	printf("\r\n ����д���Գɹ��� " );

  else 
  	printf("\r\n ����д����ʧ�ܣ� " );  

}




/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1, -pBuffer2 : Ҫ�ȽϵĻ�������ָ��
 *         -BufferLength ����������
 * ���  ��-PASSED ���
 *         -FAILED ����
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}


/*
 * ��������Fill_Buffer
 * ����  ���ڻ���������д����
 * ����  ��-pBuffer Ҫ���Ļ�����
 *         -BufferLength Ҫ���Ĵ�С
 *         -Offset ���ڻ������ĵ�һ��ֵ
 * ���  ���� 
 */
void Fill_Buffer(uint8_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint16_t index = 0;

  /* Put in global buffer same values */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/*
 * ��������eBuffercmp
 * ����  ����黺�����������Ƿ�Ϊ0
 * ����  ��-pBuffer Ҫ�ȽϵĻ�����
 *         -BufferLength ����������        
 * ���  ��PASSED ������������ȫΪ0
 *         FAILED ������������������һ����Ϊ0 
 */
TestStatus eBuffercmp(uint8_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* In some SD Cards the erased state is 0xFF, in others it's 0x00 */
    if ((*pBuffer != 0xFF) && (*pBuffer != 0x00))//��������0xff��0x00
    {
      return FAILED;
    }

    pBuffer++;
  }

  return PASSED;
}

/***************************END OF FILE*******************************/
