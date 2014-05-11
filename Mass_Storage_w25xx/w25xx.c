/*
 * File      : w25xx.c
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-11-10     aozima       first implementation
 * 2010-11-30     aozima       port to W25x16/W25X32/W25X64
*/

#include <stdint.h>
#include "w25xx.h"

static uint32_t device_id = 0;

// bsp support:
// port_init()                         : init hardware GPIO.
// CS_HIGH()                           :
// CS_LOW()                            :
// spi_lock()                          : spi lock
// spi_unlock()                        : spi unlock
// spi_config()                        : config spi for w25xx
// uint8_t spi_readwrite(uint8_t data) : spi rw

/********* bsp **********/
// CS       PA2
// SPI <--> SPI1

#include "stm32f10x.h"
//#include "rtthread.h"

#define FLASH_TRACE(...)
//#define FLASH_TRACE  rt_kprintf

#define CS_HIGH()    GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define CS_LOW()     GPIO_ResetBits(GPIOA,GPIO_Pin_2)

#define spi_lock()
#define spi_unlock()

#define spi_config()

static void port_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    CS_HIGH();

    // spi config
    {
        SPI_InitTypeDef SPI_InitStructure;

        /* Enable SPI1 Periph clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
                               | RCC_APB2Periph_SPI1,
                               ENABLE);

        /* Configure SPI1 pins: PA5-SCK, PA6-MISO and PA7-MOSI */
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        /*------------------------ SPI1 configuration ------------------------*/
        SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI_Direction_1Line_Tx;
        SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
        SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStructure.SPI_NSS  = SPI_NSS_Soft;
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;/* 72M/64=1.125M */
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStructure.SPI_CRCPolynomial = 7;

        SPI_I2S_DeInit(SPI1);
        SPI_Init(SPI1, &SPI_InitStructure);

        /* Enable SPI_MASTER */
        SPI_Cmd(SPI1, ENABLE);
        SPI_CalculateCRC(SPI1, DISABLE);
    } // spi config
}

static uint8_t spi_readwrite(uint8_t data)
{
    //Wait until the transmit buffer is empty
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // Send the byte
    SPI_I2S_SendData(SPI1, data);

    //Wait until a data is received
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // Get the received data
    data = SPI_I2S_ReceiveData(SPI1);

    // Return the shifted data
    return data;
}
/********* bsp end **********/

// status register
// S7    S6   S5   S4   S3   S2   S1   S0
// SRP   (R)  TB  BP2  BP1  BP0  WEL  BUSY
static uint8_t w25xx_read_status(void)
{
    uint8_t tmp;

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_RDSR );
    tmp=spi_readwrite(0XFF);
    CS_HIGH();
    return tmp;
}

static void w25xx_wait_busy(void)
{
    while( w25xx_read_status() & (0x01));
}

/** \brief w25xx SPI flash init
 *
 * \param void
 * \return void
 *
 */
void w25xx_init(void)
{
    port_init();

    spi_lock();
    spi_config();
    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WRDI );
    CS_HIGH();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_JEDEC_ID );
    device_id  = spi_readwrite(0xFF);      // 0xEF M7-M0
    device_id |= spi_readwrite(0xFF)<<8;   // 0x30 memory type
    device_id |= spi_readwrite(0xFF)<<16;  // 0x15~0x17 capacity
    CS_HIGH();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WRSR );
    spi_readwrite( 0 );
    CS_HIGH();

    spi_unlock();
}

/** \brief get SPI FLASH DEVICE ID
 *
 * \param void
 * \return uint32_t DEVICE_ID
 *
 */
uint32_t w25xx_get_device_id(void)
{
    return device_id;
}

/** \brief read [size] byte from [offset] to [buffer]
 *
 * \param offset uint32_t unit : byte
 * \param buffer uint8_t*
 * \param size uint32_t   unit : byte
 * \return uint32_t byte for read
 *
 */
uint32_t w25xx_read(uint32_t offset,uint8_t * buffer,uint32_t size)
{
    uint32_t index;

    spi_lock();
    spi_config();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WRDI );
    CS_HIGH();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_READ);
    spi_readwrite(  offset>>16 );
    spi_readwrite(  offset>>8 );
    spi_readwrite(  offset );
    for(index=0; index<size; index++)
    {
        *buffer++ = spi_readwrite(0xFF);
    }
    CS_HIGH();

    spi_unlock();

    return size;
}

/** \brief write N page on [page]
 *
 * \param page uint32_t unit : byte (4096 * N,1 page = 4096byte)
 * \param buffer const uint8_t*
 * \param size uint32_t unit : byte ( 4096*N )
 * \return uint32_t
 *
 */
uint32_t w25xx_page_write(uint32_t page,const uint8_t * buffer,uint32_t size)
{
    uint32_t index;

    page &= ~0xFFF; // page = 4096byte

    spi_lock();
    spi_config();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WREN );//write en
    CS_HIGH();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_ERASE_4K );
    spi_readwrite( page >> 16 );
    spi_readwrite( page >> 8 );
    spi_readwrite( page  );
    CS_HIGH();

    w25xx_wait_busy(); // wait erase done.

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WREN );//write en
    CS_HIGH();

    for(index=0; index<size/256; index++)
    {
        uint32_t i;

        w25xx_wait_busy();

        CS_LOW();
        spi_readwrite( SPI_FLASH_CMD_WREN );//write en
        CS_HIGH();

        CS_LOW();
        spi_readwrite( SPI_FLASH_CMD_PAGE_PROG );
        spi_readwrite(  page>>16 );
        spi_readwrite(  page>>8 );
        spi_readwrite(  page );
        page += 256;

        for(i=0; i<256; i++)
        {
            spi_readwrite(*buffer++);
        }
        CS_HIGH();

        w25xx_wait_busy();
    }
    CS_HIGH();

    CS_LOW();
    spi_readwrite( SPI_FLASH_CMD_WRDI );
    CS_HIGH();

    spi_unlock();
    return size;
}
