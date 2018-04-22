#ifndef _QSPI_25Q128JVSQ_H_
#define _QSPI_25Q128JVSQ_H_

#include "fsl_common.h"
#include "fsl_flexspi.h"

/*******************************************************************************
* Definitions
******************************************************************************/

//Flash Properties
#define UPPER_PAGE_SIZE	0x01000000		// 16MBytes
#define FLASH_SIZE 		0x4000 			// 16MBytes
#define BLOCK_SIZE		0x10000			// 64KBytes
#define SECTOR_SIZE 	0x1000 			// 4K
#define PAGE_SIZE 		256				// 256Bytes

//LUT index
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL 			0
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST 				1
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD 			2
#define NOR_CMD_LUT_SEQ_IDX_READ_EA_STATUS 			3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE 			4

#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR 			5
#define NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK 				5

#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP				6 		 		
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 		7
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD 		8
#define NOR_CMD_LUT_SEQ_IDX_READID 					9
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG 			10
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI 				11
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI 				12
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG 			13
#define NOR_CMD_LUT_SEQ_IDX_WRITE_EA_STATUS			14
#define NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_VOLATILE 	15

//Control Definitions
//#define CUSTOM_LUT_LENGTH 80
#define FLASH_BUSY_STATUS_POL 		1
#define FLASH_BUSY_STATUS_OFFSET 	0
#define FLASH_ERROR_STATUS_MASK 	0x0e
#define FLASH_WEL_STATUS_OFFSET		1

const uint32_t QSPIFlash_25Q128JVSQ_LUT[] = {
		/* Normal read mode - SDR */
        [4*NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4*NOR_CMD_LUT_SEQ_IDX_READ_NORMAL+1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Fast read mode - SDR */
        [4*NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x0B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4*NOR_CMD_LUT_SEQ_IDX_READ_FAST+1] = 
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Fast read quad mode - SDR */
        [4*NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x6B, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4*NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD+1] = FLEXSPI_LUT_SEQ(
            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08, kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

        /* Read extend parameters */
        [4*NOR_CMD_LUT_SEQ_IDX_READ_EA_STATUS] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC8, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Write Enable */
        [4*NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Erase Sector  */
		[4*NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
		
		/* Erase BLOCK */
		/*
		[4*NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xD8, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
		*/
			
		/* Erase Chip */
		[4*NOR_CMD_LUT_SEQ_IDX_ERASECHIP]	=
			FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC7, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Page Program - single mode */
        [4*NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4*NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE+1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Page Program - quad mode */
        [4*NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x32, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
        [4*NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Read ID */
        [4*NOR_CMD_LUT_SEQ_IDX_READID] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),//0x18
        [4*NOR_CMD_LUT_SEQ_IDX_READID+1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Write status-1 register */
        [4*NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x01, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

        /* Enter QPI mode */
        [4*NOR_CMD_LUT_SEQ_IDX_ENTERQPI] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x35, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Exit QPI mode */
        [4*NOR_CMD_LUT_SEQ_IDX_EXITQPI] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_4PAD, 0xF5, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Read status register */
        [4*NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
						
		/* Write EA status register */
        [4*NOR_CMD_LUT_SEQ_IDX_WRITE_EA_STATUS] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xC5, kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),
			
		/* Write Enable for volatile register */
        [4*NOR_CMD_LUT_SEQ_IDX_WRITE_ENABLE_VOLATILE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x50, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
};

/** necessary functions as device_t call-backs **/
#if USE_ARGC_ARGV
static uint32_t init(void *base_of_flash,int argc, char const *argv[]);
#else
static uint32_t init(void *base_of_flash);
#endif /* USE_ARGC_ARGV */
static uint32_t write(uint32_t addr,uint32_t count,char const *buffer);
static uint32_t erase(void *block_start);
static uint32_t erase_chip(void);
static uint32_t signoff(void);

////////////////////////////////////////////////////////////////////////////////
/** internal functions to check status **/
static status_t flexspi_nor_Get_VendorID(FLEXSPI_Type *base, uint8_t *vendorId);
static status_t flexspi_nor_Erase_Sector(FLEXSPI_Type *base, uint32_t address);
static status_t flexspi_nor_Erase_Block(FLEXSPI_Type *base, uint32_t address);
static status_t flexspi_nor_Write_Page(FLEXSPI_Type *base, uint32_t dstAddr, uint32_t *src);
static status_t flexspi_nor_Write_Enable(FLEXSPI_Type *base);
static status_t flexspi_nor_Wait_Bus_If_Busy(FLEXSPI_Type *base);

#endif