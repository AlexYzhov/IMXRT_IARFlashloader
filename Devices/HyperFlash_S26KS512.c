/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : Flash.c
 *    Description : iMXRT1050_EVK HyperFlash flash loader
 *
 *    History :
 *    1. Date        : Octomber, 2017
 *       Author      : Stoyan Choynev
 *       Description : Initial revision
 *
 *
 *    $Revision: 5068 $
 **************************************************************************/
/** include files **/
#include <intrinsics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "flash_loader.h"       // The flash loader framework API declarations.
#include "flash_loader_extra.h"
#include "device.h"
#include "lut.h"
//#include <stddef.h>

#include "HyperFlash_S26KS512.h"

/** local definitions **/
typedef uint16_t flashunit;

#pragma pack(1)

typedef struct
{
  unsigned short BlockNumb;
  unsigned short BlockSize;
} SectorInfo_t, *pSectorInfo_t;

typedef struct
{
  unsigned long TypWordProgTO;
  unsigned long TypBlockEraseTO;
  unsigned long MaxWordProgTO;
  unsigned long MaxBlockEraseTO;
  unsigned long NumbOfBlock;
  unsigned long TotalNumbOfSectors;
  unsigned long TotalDevSize;
} FlashDevInfo_t, *pFlashDevInfo_t;
#pragma pack()

/*Device definitions*/
#define AHB_READ_SEQUENCE           LUT_SEQUENCE_0
#define AHB_WRITE_SEQUENCE          LUT_SEQUENCE_1

#define AHB_READ_LUT_INDEX          (AHB_READ_SEQUENCE<<3)
#define AHB_WRITE_LUT_INDEX         (AHB_WRITE_SEQUENCE<<3)

/*Hyper Flash memory definitions*/
#define FLASH_A_SEQ_ADD1    (0x555*sizeof(flashunit))
#define FLASH_A_SEQ_ADD2    (0x2AA*sizeof(flashunit))

/*Flash codes big endian*/
#define FLASH_RESET       (0xF000)
#define FLASH_CODE1       (0xAA00)
#define FLASH_CODE2       (0x5500)
#define ID_IN             (0x9000)
#define READ_STATUS       (0x7000)
#define CLEAR_STATUS      (0x7100)
#define WRITE_CODE        (0xA000)
#define WRITE_BUFFER      (0x2500)
#define PROGRAM_BUFFER    (0x2900)
#define ERASE_CODE        (0x8000)
#define ERASE_SECTOR      (0x3000)
#define ERASE_CHIP        (0x1000)
#define QUERY_CFI         (0x9800)

#define STATUS_BIT_DRB      (1<<15)
#define STATUS_BIT_ESSB     (1<<14)
#define STATUS_BIT_ESB      (1<<13)
#define STATUS_BIT_PSB      (1<<12)
#define STATUS_BIT_WBASB    (1<<11)
#define STATUS_BIT_PSSB     (1<<10)
#define STATUS_BIT_SLSB     (1<<9)
#define STATUS_BIT_ESTAT    (1<<8)

#define BLOCK_SIZE 512

/** default settings **/

/** external functions **/
#if USE_ARGC_ARGV
extern const char* FlFindOption(char* option, int with_value, int argc, char const* argv[]);
#endif /*USE_ARGC_ARGV*/

/** external data **/

/** internal functions **/
#if USE_ARGC_ARGV
static uint32_t init(void *base_of_flash,int argc, char const *argv[]);
#else
static uint32_t init(void *base_of_flash);
#endif /* USE_ARGC_ARGV */
static uint32_t write(uint32_t addr,uint32_t count,char const *buffer);
static uint32_t erase(void *block_start);
static uint32_t erase_chip(void);
static void InitLUT(void);
static uint32_t CFI_Query (char * Buffer);
static void FlashCleanup(void);
static flashunit ReadStatus(void);
static void ClearStatus(void);
static uint32_t CmdSend(uint32_t addr, flashunit cmd);
static uint32_t ReadTransfer(uint32_t addr, uint64_t * data, uint16_t size);
static uint32_t WriteTransfer(uint32_t addr, uint64_t const * data, uint16_t size);
/** public data **/
const device_t HyperFlash_S26KS512 = 
{
  .init = init,
  .write = write,
  .erase = erase,
  .erase_chip = erase_chip,
  .signoff = NULL
};
/** private data **/
static FlashDevInfo_t FlashDevInfo;
/** public functions **/

/** private functions **/
/*************************************************************************
 * Function Name: init
 * Parameters: Flash Base Address
 *
 * Return:  0 - Init Successful
 *          1 - Init Fail
 * Description: init FlexSPI interface for HyperFlash. Read device size
 *              and build flash layout
 *
 *************************************************************************/
#if USE_ARGC_ARGV
static uint32_t init(void *base_of_flash,int argc, char const *argv[])
#else
static uint32_t init(void *base_of_flash)
#endif /* USE_ARGC_ARGV */
{
uint32_t result = (RESULT_ERROR);
  /**/
  FLEXSPI_MCR0 = 0xFFFFA032;   
  FLEXSPI_MCR1 = 0xFFFFFFFF; 
  FLEXSPI_MCR2 = 0x200841F7;
  FLEXSPI_AHBCR = 0x00000078;
  FLEXSPI_AHBRXBUF0CR0 = 0x00000000;
  FLEXSPI_AHBRXBUF1CR0 = 0x00000000;
  FLEXSPI_AHBRXBUF2CR0 = 0x00000000;
  //FLEXSPI_FLSHA1CR0 = 0x00010000;
  FLEXSPI_FLSHA1CR1 = 0x00001c00;
  FLEXSPI_FLSHA1CR2 = (AHB_WRITE_SEQUENCE<<8) | (AHB_READ_SEQUENCE);
  FLEXSPI_FLSHCR4 = 0x00000000; 
  FLEXSPI_DLLACR = 0x00001D00;
  FLEXSPI_DLLBCR = 0x00001D00;
  
  /*Enable Flex SPI module*/
  FLEXSPI_MCR0 &= ~(1<<1);   

  /*Initialize LUT table*/
  InitLUT();
  
  /*Software reset*/
  FLEXSPI_MCR0 |= (1<<0);
  
  /*Reset Flash memory*/
  FlashCleanup();
  /* Build NOR flash layout */
  if(RESULT_OK == CFI_Query(LAYOUT_OVERRIDE_BUFFER))
  {
    /*Set device size in KB*/
    FLEXSPI_FLSHA1CR0 = FlashDevInfo.TotalDevSize/1024;
    
    result = (OVERRIDE_LAYOUT);
  }
  
  return result; 
}

/*************************************************************************
 * Function Name: write
 * Parameters: addr, data size, ram buffer
 *             pointer
 * Return: RESULT_OK - Write Successful
 *         RESULT_ERROR - Write Fail
 * Description. writes data in to NOR
 *************************************************************************/
static uint32_t write(uint32_t addr,
                    uint32_t count,
                    char const *buffer)
{
uint32_t result = (RESULT_OK);
uint32_t size = BLOCK_SIZE - ((uint32_t)addr&(BLOCK_SIZE-1));
flashunit status;

  while(count)
  {
    size = (size<count)?size:count;
    
    ClearStatus();

    CmdSend(FLASH_A_SEQ_ADD1,FLASH_CODE1);
    CmdSend(FLASH_A_SEQ_ADD2,FLASH_CODE2);
    CmdSend(FLASH_A_SEQ_ADD1,WRITE_CODE);
    WriteTransfer(addr-FlexSPI_AHB_BASE,(uint64_t const *) buffer,size);
  
    while(!((status=ReadStatus()) & (STATUS_BIT_DRB)));
  
    if((status & (STATUS_BIT_PSB)))
    {
      result = RESULT_ERROR;
      break;
    }
    count -= size;
    addr += size;
    buffer += size;
    size = BLOCK_SIZE;
  }
  
  return result;
}

/*************************************************************************
 * Function Name: erase
 * Parameters:  Block Address
 *
 * Return: RESULT_OK - Erase Successful
 *         RESULT_ERROR - Erase Fail
 * Description: Erase block
 *************************************************************************/
static uint32_t erase(void *block_start)
{
uint32_t result = (RESULT_ERROR);
flashunit status;

  ClearStatus();

  CmdSend(FLASH_A_SEQ_ADD1,FLASH_CODE1);
  CmdSend(FLASH_A_SEQ_ADD2,FLASH_CODE2);
  CmdSend(FLASH_A_SEQ_ADD1,ERASE_CODE);
  CmdSend(FLASH_A_SEQ_ADD1,FLASH_CODE1);
  CmdSend(FLASH_A_SEQ_ADD2,FLASH_CODE2);
  CmdSend((uint32_t)(block_start)-FlexSPI_AHB_BASE,ERASE_SECTOR);

  while(!((status=ReadStatus()) & (STATUS_BIT_DRB)));
  
  if(!(status & (STATUS_BIT_ESTAT)))
  {
    result = RESULT_OK;
  }

  return result;
}

static uint32_t erase_chip(void)
{
uint32_t result = (RESULT_ERROR);
flashunit status;

  ClearStatus();

  CmdSend(FLASH_A_SEQ_ADD1 ,FLASH_CODE1);
  CmdSend(FLASH_A_SEQ_ADD2 ,FLASH_CODE2);
  CmdSend(FLASH_A_SEQ_ADD1 ,ERASE_CODE);
  CmdSend(FLASH_A_SEQ_ADD1 ,FLASH_CODE1);
  CmdSend(FLASH_A_SEQ_ADD2 ,FLASH_CODE2);
  CmdSend(FLASH_A_SEQ_ADD1 ,ERASE_CHIP);
  
  while(!((status=ReadStatus()) & (STATUS_BIT_DRB)));
  
  if(!(status & (STATUS_BIT_ESB)))
  {
    result = RESULT_ERASE_DONE;
  }

  return result;
}

static void InitLUT(void)
{
uint16_t * lut_table = (uint16_t *) &FLEXSPI_LUT0;

  /*Unlock LUT*/
  if(0x2 != FLEXSPI_LUTCR)
  {
    FLEXSPI_LUTKEY = 0x5AF05AF0;
    FLEXSPI_LUTCR = 0x2;
  }
  
  /*AHB READ sequence*/
  lut_table[AHB_READ_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_DDR,   LUT_PADS_EIGHT,   0xA0);
  lut_table[AHB_READ_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_DDR, LUT_PADS_EIGHT,   0x18);
  lut_table[AHB_READ_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_CADDR_DDR, LUT_PADS_EIGHT,   0x10);
  lut_table[AHB_READ_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_READ_DDR,  LUT_PADS_EIGHT,   0x04);
  lut_table[AHB_READ_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_READ_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_READ_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_READ_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);

  /*AHB WRITE sequence*/
  lut_table[AHB_WRITE_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_DDR,   LUT_PADS_EIGHT,   0x20);
  lut_table[AHB_WRITE_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_DDR, LUT_PADS_EIGHT,   0x18);
  lut_table[AHB_WRITE_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_CADDR_DDR, LUT_PADS_EIGHT,   0x10);
  lut_table[AHB_WRITE_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_WRITE_DDR, LUT_PADS_EIGHT,   0x02);
  lut_table[AHB_WRITE_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_WRITE_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_WRITE_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
  lut_table[AHB_WRITE_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                   0);
}

static void FlashCleanup(void)
{
uint64_t Dummy;  
  /*Send Reset command*/
  CmdSend(0,FLASH_RESET);
  for(volatile int i = 20; i; i--);
  /*Dummy read*/
  ReadTransfer(0,&Dummy,2);
}

static uint32_t CFI_Query (char * Buffer)
{
uint32_t result = (RESULT_ERROR);
SectorInfo_t SectorInfo;
uint8_t *pDst;
volatile flashunit * pSrc;
uint32_t i, j;
uint32_t BlockSize, BlockNumb;

char srt[18];

  *Buffer = '\0';
  /*Put Flash memory in CFI mode*/
  CmdSend(FLASH_A_SEQ_ADD1,QUERY_CFI);
  /*Read QRY from AHB*/
  if((*((volatile flashunit *)FlexSPI_AHB_BASE + 0x10)>>8 == 'Q')&&
     (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x11)>>8 == 'R')&&
     (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x12)>>8 == 'Y'))
  {
    /*Get timing characteristic of the device*/
    FlashDevInfo.TypWordProgTO = (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x1F))>>8;
    FlashDevInfo.TypWordProgTO = (1UL << FlashDevInfo.TypWordProgTO); // us
    FlashDevInfo.TypBlockEraseTO = (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x21))>>8;
    FlashDevInfo.TypBlockEraseTO = (1UL << FlashDevInfo.TypBlockEraseTO) * 1000; // ms
  
    FlashDevInfo.MaxWordProgTO = (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x23))>>8;
    FlashDevInfo.MaxWordProgTO = (1UL << FlashDevInfo.MaxWordProgTO) * FlashDevInfo.TypWordProgTO;
    FlashDevInfo.MaxBlockEraseTO = (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x25))>>8;
    FlashDevInfo.MaxBlockEraseTO = (1UL << FlashDevInfo.MaxBlockEraseTO) * FlashDevInfo.TypBlockEraseTO;
  
    FlashDevInfo.NumbOfBlock = (*((volatile flashunit *)FlexSPI_AHB_BASE + 0x2C))>>8;
  
    pSrc = (volatile flashunit *)FlexSPI_AHB_BASE + 0x2D;
    FlashDevInfo.TotalNumbOfSectors = 0;
    FlashDevInfo.TotalDevSize = 0;
    /*Build flash leyout table*/
    for(i = 0; i < FlashDevInfo.NumbOfBlock; i++)
    {
      pDst = (unsigned char *)&SectorInfo;
      for(j = 0; j < 4; j++)
      {
        *pDst++ = (*pSrc++)>>8;
      }
      BlockSize = SectorInfo.BlockSize * 256;
      BlockNumb = SectorInfo.BlockNumb + 1;
      sprintf(srt,"%d 0x%X, ",BlockNumb,BlockSize);
      FlashDevInfo.TotalNumbOfSectors += BlockNumb;
      FlashDevInfo.TotalDevSize += BlockNumb * BlockSize;
      strcat(Buffer,srt);
    }
    result = (RESULT_OK);
  }
  
  /*return to read mode*/
  FlashCleanup();
  
  return result;
}

static flashunit ReadStatus(void)
{
uint64_t status;
  CmdSend(FLASH_A_SEQ_ADD1, READ_STATUS);
  ReadTransfer(0, &status, 2);
  return status;
}

static void ClearStatus(void)
{ 
  CmdSend(FLASH_A_SEQ_ADD1, CLEAR_STATUS);
}

static uint32_t CmdSend(uint32_t addr, flashunit cmd)
{
uint64_t data = cmd;
 return WriteTransfer(addr,&data, 2);
}

static uint32_t ReadTransfer(uint32_t addr, uint64_t * data, uint16_t size)
{
uint32_t result = RESULT_OK;
uint64_t * fifo;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  /*Set Sequence ID 0,Sequence Num 0 and Data size*/
  FLEXSPI_IPCR1 = (0<<20) | (AHB_READ_SEQUENCE<<16) | (size);
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPRXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /**/
  while(size)
  {
    uint32_t wc= (size<sizeof(uint64_t))?size:sizeof(uint64_t);
    fifo = (uint64_t *) &FLEXSPI_RFDR0;
    /*check for data in the fifo*/
    while(!(FLEXSPI_INTR & (1<<5)));

    *data++ = *fifo;
    
    FLEXSPI_INTR |= (1<<5);
    size -= wc;
    /*check for errors*/
    if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
    {
      result = RESULT_ERROR;
      break;
    }
  }
  
  while(!(FLEXSPI_STS0 & (1<<1)));
  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  return result;
}

static uint32_t WriteTransfer(uint32_t addr,uint64_t const * data, uint16_t size)
{
uint32_t result = RESULT_OK;
uint64_t * fifo;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = (uint32_t)addr;
  /*Set Sequence ID 1,Sequence Num 0 and Data size*/
  FLEXSPI_IPCR1 = (0<<20) | (AHB_WRITE_SEQUENCE<<16) | (size);
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPTXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /**/
  while(size)
  {

    uint32_t wc= (size<sizeof(uint64_t))?size:sizeof(uint64_t);
    fifo = (uint64_t *) &FLEXSPI_TFDR0;
    while(!(FLEXSPI_INTR & (1<<6)));
    *fifo = *data++;
    /*write data to fifo*/
    FLEXSPI_INTR |= (1<<6);
    size -= wc;
    /*check for errors*/
    if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
    {
      result = RESULT_ERROR;
      break;
    }
  }
  
  while(!(FLEXSPI_STS0 & (1<<1)));
  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  return result;
}
