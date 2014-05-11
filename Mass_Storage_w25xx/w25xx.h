/*
 * File      : sst25vfxx.h
 *
 * Change Logs:
 * Date           Author       Notes
 * 2010-11-10     aozima       first implementation
*/

#ifndef W25XX_H_INCLUDED
#define W25XX_H_INCLUDED

#include <stdint.h>

/* function list */
extern void w25xx_init(void);
extern uint32_t w25xx_get_device_id(void);
extern uint32_t w25xx_read(uint32_t offset,uint8_t * buffer,uint32_t size);
extern uint32_t w25xx_page_write(uint32_t page,const uint8_t * buffer,uint32_t size);

/* command list */
#define SPI_FLASH_CMD_RDSR                    0x05  /* ¶Á×´Ì¬¼Ä´æÆ÷     */
#define SPI_FLASH_CMD_WRSR                    0x01  /* Ð´×´Ì¬¼Ä´æÆ÷     */
#define SPI_FLASH_CMD_WRDI                    0x04  /* ¹Ø±ÕÐ´Ê¹ÄÜ       */
#define SPI_FLASH_CMD_WREN                    0x06  /* ´ò¿ªÐ´Ê¹ÄÜ       */
#define SPI_FLASH_CMD_READ                    0x03  /* ¶ÁÊý¾Ý           */
#define SPI_FLASH_CMD_FAST_READ               0x0B  /* ¿ìËÙ¶Á           */
#define SPI_FLASH_CMD_PAGE_PROG               0x02  /* Ò³±à³Ì  256Byte  */
#define SPI_FLASH_CMD_ERASE_4K                0x20  /* ÉÈÇø²Á³ý:4K      */
#define SPI_FLASH_CMD_ERASE_64K               0xD8  /* ÉÈÇø²Á³ý:64K     */
#define SPI_FLASH_CMD_ERASE_full              0xC7  /* È«Æ¬²Á³ý         */
#define SPI_FLASH_CMD_JEDEC_ID                0x9F  /* ¶Á JEDEC_ID      */
#define SPI_FLASH_CMD_POWER_DOWN              0xB9  /* */
#define SPI_FLASH_CMD_RES_DEVICE_ID           0xAB  /* release power    */
#define SPI_FLASH_CMD_MANUFACTURER_ID         0x90  /* */

/* device id define */
enum _w25xx_id
{
    unknow     = 0,
    W25X16 = 0x001530EF,
    W25X32 = 0x001630EF,
    W25X64 = 0x001730EF,
};

#endif // SST25VFXX_H_INCLUDED
