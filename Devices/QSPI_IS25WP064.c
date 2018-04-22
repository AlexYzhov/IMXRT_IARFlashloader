/*************************************************************************
 *
 *   Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2017
 *
 *    File name   : Flash.c
 *    Description : iMXRT1050_EVK QSPI flash loader
 *
 *    History :
 *    1. Date        : November, 2017
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

#include "QSPI_IS25WP064.h"

/** local definitions **/
#define ISSI_CMD_NORD        0x03
#define ISSI_CMD_FRD         0x0B
#define ISSI_CMD_FRDIO       0xBB
#define ISSI_CMD_FRDO        0x3B
#define ISSI_CMD_FRQIO       0xEB
#define ISSI_CMD_FRQO        0x6B
#define ISSI_CMD_FRDTR       0x0D
#define ISSI_CMD_FRDDTR      0xBD
#define ISSI_CMD_FRQDTR      0xED
#define ISSI_CMD_PP          0x02
#define ISSI_CMD_PPQ         0x38
#define ISSI_CMD_SER         0x20
#define ISSI_CMD_BER32       0x52
#define ISSI_CMD_BER64       0xD8
#define ISSI_CMD_SECUNLOCK   0x26
#define ISSI_CMD_RDDYB       0xFA
#define ISSI_CMD_WRDYB       0xFB
#define ISSI_CMD_RDPPB       0xFC
#define ISSI_CMD_PGPPB       0xFD
#define ISSI_CMD_4NORD       0x13
#define ISSI_CMD_4FRD        0x0C
#define ISSI_CMD_4FRDIO      0xBC
#define ISSI_CMD_4FRDO       0x3C
#define ISSI_CMD_4FRQIO      0xEC
#define ISSI_CMD_4FRQO       0x6C
#define ISSI_CMD_4FRDTR      0x0E
#define ISSI_CMD_4FRDDTR     0xBE
#define ISSI_CMD_4FRQDTR     0xEE
#define ISSI_CMD_4PP         0x12
#define ISSI_CMD_4PPQ        0x3E
#define ISSI_CMD_4SER        0x21
#define ISSI_CMD_4BER32      0x5C
#define ISSI_CMD_4BER64      0xDC
#define ISSI_CMD_4SECUNLOCK  0x25
#define ISSI_CMD_4RDDYB      0xE0
#define ISSI_CMD_4WRDYB      0xE1
#define ISSI_CMD_4RDPPB      0xE2
#define ISSI_CMD_4PGPPB      0xE3
#define ISSI_CMD_CER         0x60
#define ISSI_CMD_WREN        0x06
#define ISSI_CMD_WRDI        0x04
#define ISSI_CMD_RDSR        0x05
#define ISSI_CMD_WRSR        0x01
#define ISSI_CMD_RDFR        0x48
#define ISSI_CMD_WRFR        0x42
#define ISSI_CMD_QIOEN       0x35
#define ISSI_CMD_QIODI       0xF5
#define ISSI_CMD_PERSUS      0xB0
#define ISSI_CMD_PERRSM      0x30
#define ISSI_CMD_DP          0xB9
#define ISSI_CMD_RDID        0xAB
#define ISSI_CMD_SRPNV       0x65
#define ISSI_CMD_SRPV        0x63
#define ISSI_CMD_SERPNV      0x85
#define ISSI_CMD_SERPV       0x83
#define ISSI_CMD_RDRP        0x61
#define ISSI_CMD_RDERP       0x81
#define ISSI_CMD_CLERP       0x82
#define ISSI_CMD_RDJDID      0x9F
#define ISSI_CMD_RDMDID      0x90
#define ISSI_CMD_RDJDIDQ     0xAF
#define ISSI_CMD_RDUID       0x4B
#define ISSI_CMD_RDSFDP      0x5A
#define ISSI_CMD_NOP         0x00
#define ISSI_CMD_RSTEN       0x66
#define ISSI_CMD_RST         0x99
#define ISSI_CMD_IRER        0x64
#define ISSI_CMD_IRP         0x62
#define ISSI_CMD_IRRD        0x68
#define ISSI_CMD_SECLOCK     0x24
#define ISSI_CMD_RDABR       0x14
#define ISSI_CMD_WRABR       0x15
#define ISSI_CMD_RDBR        0xC8
#define ISSI_CMD_WRBRV       0xC5
#define ISSI_CMD_WRBRNV      0x18
#define ISSI_CMD_EN4B        0xB7
#define ISSI_CMD_EX4B        0x29
#define ISSI_CMD_RDASP       0x2B
#define ISSI_CMD_PGASP       0x2F
#define ISSI_CMD_RDPLB       0xA7
#define ISSI_CMD_WRPLB       0xA6
#define ISSI_CMD_SFRZ        0x91
#define ISSI_CMD_RDPWD       0xE7
#define ISSI_CMD_PGPWD       0xE8
#define ISSI_CMD_UNPWD       0xE9
#define ISSI_CMD_GBLK        0x7E
#define ISSI_CMD_GBUN        0x98

#define ISSI_PAGE_SIZE            256
#define ISSI_SUBSECTOR_SIZE       (4*1024)
#define ISSI_SECTOR_SIZE          (32*1024)

#define READ_SEQUENCE               LUT_SEQUENCE_0
#define READ_ID_SEQUENCE            LUT_SEQUENCE_7
#define WRITE_E_SEQUENCE            LUT_SEQUENCE_8
#define BER32_ERASE_SEQUENCE        LUT_SEQUENCE_9
#define CHIP_ERASE_SEQUENCE         LUT_SEQUENCE_10
#define QUAD_PROG_SEQUENCE          LUT_SEQUENCE_11
#define RD_EXTENDED_REG_SEQUENCE    LUT_SEQUENCE_12
#define CLR_EXTENDED_REG_SEQUENCE   LUT_SEQUENCE_13
#define RD_STATUS_REG_SEQUENCE      LUT_SEQUENCE_14
#define WR_STATUS_REG_SEQUENCE      LUT_SEQUENCE_15

#define READ_LUT_INDEX              (READ_SEQUENCE<<3)
#define READ_ID_LUT_INDEX           (READ_ID_SEQUENCE<<3)
#define WRITE_E_LUT_INDEX           (WRITE_E_SEQUENCE<<3)
#define BER32_ERASE_LUT_INDEX       (BER32_ERASE_SEQUENCE<<3)
#define CHIP_ERASE_LUT_INDEX        (CHIP_ERASE_SEQUENCE<<3)
#define QUAD_PROG_LUT_INDEX         (QUAD_PROG_SEQUENCE<<3)
#define RD_EXTENDED_REG_LUT_INDEX   (RD_EXTENDED_REG_SEQUENCE<<3)
#define CLR_EXTENDED_REG_LUT_INDEX  (CLR_EXTENDED_REG_SEQUENCE<<3)
#define RD_STATUS_REG_LUT_INDEX     (RD_STATUS_REG_SEQUENCE<<3)
#define WR_STATUS_REG_LUT_INDEX     (WR_STATUS_REG_SEQUENCE<<3)

#define ADDR_3B        24
#define ADDR_4B        32 

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
static uint32_t signoff(void);
static void InitLUT(void);
static void SetLUT32b(void);
static uint32_t ReadID(uint32_t addr, uint32_t * id);
static uint32_t WriteEnable(uint32_t addr);
static uint32_t SectorErase(uint32_t addr);
static uint32_t ChipErase(uint32_t addr);
static uint32_t PageProgram(uint32_t addr, const uint64_t * data,uint16_t size);
//static uint32_t ReadExtendedReg(uint32_t addr, uint8_t * status);
//static uint32_t ClearExtendedReg(uint32_t addr);
static uint32_t ReadStatusReg(uint32_t addr, uint8_t * status);
static uint32_t WriteStatusReg(uint32_t addr, uint8_t data);

/** public data **/
const device_t QSPIFlash_IS25WP064 = 
{
  .init = init,
  .write = write,
  .erase = erase,
  .erase_chip = erase_chip,
  .signoff = signoff
};
/** private data **/
static uint32_t a1_size;
static uint32_t b1_size;
static uint8_t a1_StatusReg;
static uint8_t b1_StatusReg;
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
uint32_t id;

  a1_size = 0;
  b1_size = 0;
  /**/
  FLEXSPI_MCR0 = 0xFFFF8802;   
  FLEXSPI_MCR1 = 0xFFFFFFFF; 
  FLEXSPI_MCR2 = 0x200041F7;
  FLEXSPI_AHBCR = 0x00000078;
  FLEXSPI_AHBRXBUF0CR0 = 0x00000000;
  FLEXSPI_AHBRXBUF1CR0 = 0x00000000;
  FLEXSPI_AHBRXBUF2CR0 = 0x00000000;
  FLEXSPI_FLSHA1CR0 = 0x00010000;
  FLEXSPI_FLSHA2CR0 = 0x00000000;
  FLEXSPI_FLSHB1CR0 = 0x00010000;
  FLEXSPI_FLSHB2CR0 = 0x00000000;
  FLEXSPI_FLSHA1CR1 = 0x00000063;
  FLEXSPI_FLSHB1CR1 = 0x00000063;
  FLEXSPI_FLSHA1CR2 =  (READ_SEQUENCE);
  FLEXSPI_FLSHCR4 = 0x00000000; 
  //FLEXSPI_DLLACR = 0x00001D00;
  //FLEXSPI_DLLBCR = 0x00001D00;
  
  /*Enable Flex SPI module*/
  FLEXSPI_MCR0 &= ~(1<<1);   

  /*Initialize LUT table*/
  InitLUT();
  
  /*Software reset*/
  FLEXSPI_MCR0 |= (1<<0);
  
  while((1<<0) & FLEXSPI_MCR0);
    
  /*Read A1 ID*/
  ReadID(0x00000000,&id);
  
  if(0x9D == (id & 0x000000FF))
  {
    a1_size = 1<<((id >> 16) & 0x00FF);
    ReadStatusReg(0x00000000, &a1_StatusReg);
    if(!((a1_StatusReg &= 0xFC)& (1<<6)))
    {
      WriteEnable(0x00000000);
      WriteStatusReg(0x00000000, a1_StatusReg | (1<<6));
    }
  }
  
  /*Read B1 ID*/
  ReadID(FLEXSPI_FLSHA1CR0*1024,&id);
  if(0x9D == (id & 0x000000FF))
  {
    b1_size = 1<<((id >> 16) & 0x00FF);
    ReadStatusReg(FLEXSPI_FLSHA1CR0*1024, &b1_StatusReg);
    if(!((b1_StatusReg &= 0xFC)& (1<<6)))
    {
      WriteEnable(FLEXSPI_FLSHA1CR0*1024);
      WriteStatusReg(FLEXSPI_FLSHA1CR0*1024, b1_StatusReg | (1<<6));
    }
  }

  if(FlFindOption("--setQE", 0, argc, argv)) {
    a1_StatusReg |= (1<<6);
    b1_StatusReg |= (1<<6);
  } else  if(FlFindOption("--clrQE", 0, argc, argv)) {
    a1_StatusReg &= ~(1<<6);
    b1_StatusReg &= ~(1<<6);
  }

  FLEXSPI_FLSHA1CR0 = a1_size/1024;
  FLEXSPI_FLSHB1CR0 = b1_size/1024;
  
  sprintf(LAYOUT_OVERRIDE_BUFFER,"%d 0x%X\0",(a1_size + b1_size)/ISSI_SECTOR_SIZE,ISSI_SECTOR_SIZE);
  
  result = (OVERRIDE_LAYOUT);
  /**/
  if(1<<24 < a1_size)
  {/*A1 device is bigger*/
    /*Change LUT Read, Write and Erase
      sequences to use 4Byte address*/
    SetLUT32b();
    
    if(0 != b1_size)
    {
      if(1<<24 >= b1_size)
      {/*2 devices with 
         none compatible size*/
        result =  (RESULT_ERROR);
      }
    }   
  }
  else
  {
    if(1<<24 < b1_size)
    {/*B1 device is bigger*/
      /*Change LUT Read, Write and Erase
        sequences to use 4Byte address*/
      SetLUT32b();
      
      if( 0 != a1_size)
      {/*2 devices with 
         none compatible size*/
        result =  (RESULT_ERROR);
      }
    }   
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
uint32_t loadaddr = addr-FlexSPI_AHB_BASE;
uint8_t status;
uint32_t size = ISSI_PAGE_SIZE - ((uint32_t)addr&(ISSI_PAGE_SIZE-1));

  while(count && (RESULT_OK==result))
  {
    size = (size<count)?size:count;
    
    /*Clear QSPI Flash status flags*/
    WriteEnable(loadaddr);
    /*Page Program*/
    PageProgram(loadaddr,(uint64_t *)buffer,size);
    /*Wait the end of the operation on the 
      QSPI Flash side*/
    do
    {
      /*Read QSPI Flash Status Flags*/
      result=ReadStatusReg(loadaddr, &status);
    }while((RESULT_OK==result) && (status & (1<<0)));
    /*Program error*/
    count -= size;
    loadaddr += size;
    buffer += size;
    size = ISSI_PAGE_SIZE;
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
uint32_t result = (RESULT_OK);
uint32_t addr = (uint32_t)(block_start)-FlexSPI_AHB_BASE;
uint8_t status;

  /*Write enable*/
  WriteEnable(addr);
  /*Erase Sector*/
  SectorErase(addr);
  /*Wait the end of the operation on the 
    QSPI Flash side*/
  do
  {
    /*Read QSPI Flash Status Flags*/
    result=ReadStatusReg(addr, &status);
  }while((RESULT_OK==result) && (status & (1<<0)));
  /*Erase error*/
  
  return result;
}

static uint32_t erase_chip(void)
{
uint32_t result = (RESULT_ERASE_DONE);
uint32_t addr;
uint8_t status;

  /*check for device on A1*/
  if(a1_size)
  {
    addr = 0;
    /*Write enable*/
    WriteEnable(addr);
    /*Erase Sector*/
    ChipErase(addr);
    /*Wait the end of the operation on the 
      QSPI Flash side*/
    do
    {
      /*Read QSPI Flash Status Flags*/
      result=ReadStatusReg(addr, &status);
    }while((RESULT_OK==result) && (status & (1<<0)));
  }
  /*check for error and device on B1*/
  if((RESULT_ERROR != result) && b1_size)
  {
    addr = a1_size;
    /*Write enable*/
    WriteEnable(addr);
    /*Erase Sector*/
    ChipErase(addr);
    /*Wait the end of the operation on the 
      QSPI Flash side*/
    do
    {
      /*Read QSPI Flash Status Flags*/
      result=ReadStatusReg(addr, &status);
    }while((RESULT_OK==result) && (status & (1<<0)));
  }
  
  return result;
}

/** private functions **/
static void InitLUT(void)
{
uint16_t * lut_table = (uint16_t *) &FLEXSPI_LUT0;

  /*Unlock LUT*/
  if(0x2 != FLEXSPI_LUTCR)
  {
    FLEXSPI_LUTKEY = 0x5AF05AF0;
    FLEXSPI_LUTCR = 0x2;
  }
  
  /*Quad Input/output read sequence*/
  lut_table[READ_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,    LUT_PADS_ONE,  ISSI_CMD_FRQIO);
  lut_table[READ_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR,  LUT_PADS_FOUR,        ADDR_3B);
  lut_table[READ_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_DUMMY_SDR,  LUT_PADS_FOUR,              6);
  lut_table[READ_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_READ_SDR,   LUT_PADS_FOUR,              8);
  lut_table[READ_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_JMP_ON_CS,  0,                          0);
  lut_table[READ_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                          0);
  lut_table[READ_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                          0);
  lut_table[READ_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                          0);

  /*READ ID sequence*/
  lut_table[READ_ID_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_RDJDID);
  lut_table[READ_ID_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_READ_SDR,  LUT_PADS_ONE,               3);
  lut_table[READ_ID_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);
  lut_table[READ_ID_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);
  lut_table[READ_ID_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);
  lut_table[READ_ID_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);
  lut_table[READ_ID_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);
  lut_table[READ_ID_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                          0);

  /*Write Enable sequence*/
  lut_table[WRITE_E_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_WREN);
  lut_table[WRITE_E_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WRITE_E_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);

  /*Block 32K Erase sequence*/
  lut_table[BER32_ERASE_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,    LUT_PADS_ONE, ISSI_CMD_BER32);
  lut_table[BER32_ERASE_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR,  LUT_PADS_ONE,        ADDR_3B);
  lut_table[BER32_ERASE_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);
  lut_table[BER32_ERASE_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);
  lut_table[BER32_ERASE_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);
  lut_table[BER32_ERASE_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);
  lut_table[BER32_ERASE_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);
  lut_table[BER32_ERASE_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                         0);

  /*Chip Erase sequence*/
  lut_table[CHIP_ERASE_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,    LUT_PADS_ONE, ISSI_CMD_CER);
  lut_table[CHIP_ERASE_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);
  lut_table[CHIP_ERASE_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,       0,                       0);

  /*Quad Page Program secquence*/
  lut_table[QUAD_PROG_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_PPQ);
  lut_table[QUAD_PROG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR, LUT_PADS_ONE,      ADDR_3B);
  lut_table[QUAD_PROG_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_WRITE_SDR, LUT_PADS_FOUR,           0);
  lut_table[QUAD_PROG_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                       0);
  lut_table[QUAD_PROG_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                       0);
  lut_table[QUAD_PROG_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                       0);
  lut_table[QUAD_PROG_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                       0);
  lut_table[QUAD_PROG_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                       0);
  
  /*Read Extended Register sequence*/
  lut_table[RD_EXTENDED_REG_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_RDERP);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_READ_SDR,  LUT_PADS_ONE,              1);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);
  lut_table[RD_EXTENDED_REG_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP, 0,                              0);

  /*Clear Extended Register sequence*/
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,  LUT_PADS_ONE, ISSI_CMD_CLERP);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);
  lut_table[CLR_EXTENDED_REG_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,     0,                         0);

  /*Read Status Register sequence*/
  lut_table[RD_STATUS_REG_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_RDSR);
  lut_table[RD_STATUS_REG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_READ_SDR,  LUT_PADS_ONE,             1);
  lut_table[RD_STATUS_REG_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[RD_STATUS_REG_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[RD_STATUS_REG_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[RD_STATUS_REG_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[RD_STATUS_REG_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[RD_STATUS_REG_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  
  /*Write Status Register sequence*/
  lut_table[WR_STATUS_REG_LUT_INDEX+0] = FLEXSPI_LUT_INST(LUT_CODE_CMD_SDR,   LUT_PADS_ONE, ISSI_CMD_WRSR);
  lut_table[WR_STATUS_REG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_WRITE_SDR, LUT_PADS_ONE,             1);
  lut_table[WR_STATUS_REG_LUT_INDEX+2] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WR_STATUS_REG_LUT_INDEX+3] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WR_STATUS_REG_LUT_INDEX+4] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WR_STATUS_REG_LUT_INDEX+5] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WR_STATUS_REG_LUT_INDEX+6] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
  lut_table[WR_STATUS_REG_LUT_INDEX+7] = FLEXSPI_LUT_INST(LUT_CODE_STOP,      0,                        0);
}

static void SetLUT32b(void)
{
uint16_t * lut_table = (uint16_t *) &FLEXSPI_LUT0;

  /*Change Quad Input/output read sequence*/
  lut_table[READ_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR,  LUT_PADS_FOUR,        ADDR_4B);

  /*Change Block Erase sequence*/
  lut_table[BER32_ERASE_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR,  LUT_PADS_ONE,        ADDR_4B);

  /*Change Quad Program secquence*/
  lut_table[QUAD_PROG_LUT_INDEX+1] = FLEXSPI_LUT_INST(LUT_CODE_RADDR_SDR, LUT_PADS_ONE,      ADDR_4B);
}

static uint32_t ReadID(uint32_t addr,uint32_t * id)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  /*Set Sequence ID 0,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (READ_ID_SEQUENCE<<16) | 3;
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPRXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;

  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));
 
  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    id = 0;
    result = RESULT_ERROR;
  }
  else
  {
    *id = FLEXSPI_RFDR0;
  }

  return result;
}

static uint32_t ReadStatusReg(uint32_t addr, uint8_t * status)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  /*Set Sequence ID,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (RD_STATUS_REG_SEQUENCE<<16);
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPRXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;

  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));

  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  else
  {
    /**/
    *status = FLEXSPI_RFDR0; 
  }

  return result;
}  

static uint32_t WriteStatusReg(uint32_t addr, uint8_t data)
{
uint32_t result = RESULT_OK;
    /*Clear interrupts*/
    FLEXSPI_INTR = 0x00000F3F;
    
    /*Set Transfer address*/
    FLEXSPI_IPCR0 = addr;
    /**/
    FLEXSPI_IPCR1 = (0<<20) | (WR_STATUS_REG_SEQUENCE<<16);
    /*Clear fifo. Watermark is 8 bytes*/
    FLEXSPI_IPTXFCR = 1;
    /*Start trensfer*/
    FLEXSPI_IPCMD = 1;
  
    FLEXSPI_TFDR0 = data;
  
    /*write data to fifo*/
    FLEXSPI_INTR |= (1<<6);
 
    /*wait transfer end*/
    while(!(FLEXSPI_INTR & (1<<0)));

    if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
    {
      result = RESULT_ERROR;
    }
  
  return result;
}

static uint32_t WriteEnable(uint32_t addr)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  
  /*Set Sequence ID 1,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (WRITE_E_SEQUENCE<<16);
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));
  
  return result;
}

static uint32_t SectorErase(uint32_t addr)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  
  /*Set Sequence ID 1,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (BER32_ERASE_SEQUENCE<<16);
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));
  
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  
  return result;
}

static uint32_t ChipErase(uint32_t addr)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  
  /*Set Sequence ID 1,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (CHIP_ERASE_SEQUENCE<<16);
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));
  
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  
  return result;
}

static uint32_t PageProgram(uint32_t addr, const uint64_t * data,uint16_t size)
{
uint32_t result = RESULT_OK;
   
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  
  /*Set Sequence ID 1,Sequence Num 0 and Data size*/
  FLEXSPI_IPCR1 = (0<<20) | (QUAD_PROG_SEQUENCE<<16) | (size);
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPTXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  
  while(size)
  {

    while(!(FLEXSPI_INTR & (1<<6)));
    *((uint64_t *) &FLEXSPI_TFDR0) = *data++;
    /*write data to fifo*/
    FLEXSPI_INTR |= (1<<6);
    size -= 8;
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

#if 0
static uint32_t ReadExtendedReg(uint32_t addr, uint8_t * status)
{
uint32_t result = RESULT_OK;
  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  /*Set Transfer address*/
  FLEXSPI_IPCR0 = addr;
  /*Set Sequence ID,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (RD_EXTENDED_REG_SEQUENCE<<16);
  /*Clear fifo. Watermark is 8 bytes*/
  FLEXSPI_IPRXFCR = 1;
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;

  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));

  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  else
  {
    *status = FLEXSPI_RFDR0;
  }
  return result;
}

static uint32_t ClearExtendedReg(uint32_t addr)
{
uint32_t result = RESULT_OK;

  /*Clear interrupts*/
  FLEXSPI_INTR = 0x00000F3F;
  
  /*Set Sequence ID 1,Sequence Num 0*/
  FLEXSPI_IPCR1 = (0<<20) | (CLR_EXTENDED_REG_SEQUENCE<<16);
  /*Start trensfer*/
  FLEXSPI_IPCMD = 1;
  /*wait transfer end*/
  while(!(FLEXSPI_INTR & (1<<0)));
  /*check for errors*/
  if(FLEXSPI_INTR & ((1<<1) | (1<<3)))
  {
    result = RESULT_ERROR;
  }
  return result;
}
#endif

static uint32_t signoff(void)
{
  if(a1_size && !(a1_StatusReg & (1<<6)))
  {
    WriteEnable(0x00000000);
    WriteStatusReg(0x00000000, a1_StatusReg );
  }
  
  if(b1_size && !(b1_StatusReg & (1<<6)))
  {
    WriteEnable(FLEXSPI_FLSHA1CR0*1024);
    WriteStatusReg(FLEXSPI_FLSHA1CR0*1024, b1_StatusReg );
  }

  return (RESULT_OK);
}
