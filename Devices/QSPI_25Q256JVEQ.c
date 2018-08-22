/*************************************************************************
*
*   Used with ICCARM and AARM.
*
*    (c) Copyright IAR Systems 2017
*
*    File name   : QSPI_25Q256JVEQ.c
*    Description : SphinxEVK QSPI flash loader
*
*    History :
*    1. Date        : April, 2018
*       Author      : Alex Yzhov
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

#include "fsl_flexspi.h"
#include "fsl_iomuxc.h"
#include "clock_config.h"

#include "device.h"
#include "lut.h"

#include "QSPI_25Q256JVEQ.h"

/** public data **/
const device_t QSPIFlash_25Q256JVEQ =
{
	.init = init,
	.write = write,
	.erase = erase,
	.erase_chip = erase_chip,
	//.signoff = signoff
};

#if USE_ARGC_ARGV
static uint32_t init(void *base_of_flash,int argc, char const *argv[])
#else
static uint32_t init(void *base_of_flash)
#endif /* USE_ARGC_ARGV */
{
	uint8_t vendorID = 0;
	flexspi_config_t config = 
	{
		.rxSampleClock = kFLEXSPI_ReadSampleClkLoopbackInternally,
    	.enableSckFreeRunning = false,
    	.enableCombination = false,
    	.enableDoze = true,
    	.enableHalfSpeedAccess = false,
    	.enableSckBDiffOpt = false,
    	.enableSameConfigForAll = false,
    	.seqTimeoutCycle = 0xFFFFU,
    	.ipGrantTimeoutCycle = 0xFFU,
    	.txWatermark = 8,
    	.rxWatermark = 8,
    	.ahbConfig.enableAHBWriteIpTxFifo = false,
    	.ahbConfig.enableAHBWriteIpRxFifo = false,
    	.ahbConfig.ahbGrantTimeoutCycle = 0xFFU,
    	.ahbConfig.ahbBusTimeoutCycle = 0xFFFFU,
    	.ahbConfig.resumeWaitCycle = 0x20U,
		.ahbConfig.buffer = {0, 0, 0, 0},
    	.ahbConfig.enableClearAHBBufferOpt = false,
    	.ahbConfig.enableAHBPrefetch = false,
    	.ahbConfig.enableAHBBufferable = false,
    	.ahbConfig.enableAHBCachable = false,
	};
	
	flexspi_device_config_t QSPIFlash_25Q256JVEQ_config = {
		.flexspiRootClk = 10000000UL,
		.flashSize = FLASH_SIZE,
		.CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
		.CSInterval = 2,
		.CSHoldTime = 3,
		.CSSetupTime = 3,
		.dataValidTime = 0,
		.columnspace = 0,
		.enableWordAddress = 0,
		.AWRSeqIndex = 0,
		.AWRSeqNumber = 0,
		.ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
		.ARDSeqNumber = 1,
		.AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
		.AHBWriteWaitInterval = 0,
	};
	
	/* Set FlexSPI Pin mux && Pin config */
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,  1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_06_FLEXSPIA_SS0_B,  0x10F1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,   1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_07_FLEXSPIA_SCLK,   0x10F1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_08_FLEXSPIA_DATA00, 0x10F1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_09_FLEXSPIA_DATA01, 0x10F1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_10_FLEXSPIA_DATA02, 0x10F1);
	IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 1U); IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B1_11_FLEXSPIA_DATA03, 0x10F1);
	
	/* Set FlexSPI Clock config */
	BOARD_BootClockRUN();
    CLOCK_InitUsb1Pll(&(clock_usb_pll_config_t){.loopDivider = 0U});
    CLOCK_InitUsb1Pfd(kCLOCK_Pfd0,  24);   /* Set PLL3 PFD0 clock 360MHZ. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3);  /* Choose PLL3 PFD0 clock as flexspi source clock. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);    /* flexspi clock 120M. */
	
	/* Set AHB buffer size for reading data through AHB bus. */
    FLEXSPI_Init(FLEXSPI, &config);
	
	/* Configure flash settings according to serial flash feature. */
    FLEXSPI_SetFlashConfig(FLEXSPI, &QSPIFlash_25Q256JVEQ_config, kFLEXSPI_PortA1);
	
	/* Update LUT table. */
    FLEXSPI_UpdateLUT(FLEXSPI, 0, QSPIFlash_25Q256JVEQ_LUT, sizeof(QSPIFlash_25Q256JVEQ_LUT)/sizeof(uint32_t));
	
	/* Get vendor ID. */
    if (kStatus_Success != flexspi_nor_Get_VendorID(FLEXSPI, &vendorID))
    {
        return RESULT_ERROR;
    }
	else
	{
		return vendorID;
	}
}

static uint32_t write(uint32_t addr, uint32_t count, char const *buffer)
{
	status_t status = kStatus_Success;
	uint32_t index = addr-FlexSPI_AHB_BASE;
	uint32_t size = PAGE_SIZE-((uint32_t)addr&(PAGE_SIZE-1));
	
	/* Write the Pages that needed */
	while(count)
	{
		flexspi_nor_Write_Page(FLEXSPI, index, (void*)buffer);
		if(kStatus_Success != status)
		{
			return RESULT_ERROR;
		}
		
		count -= size;
		index += size;
		buffer += size;
		size = PAGE_SIZE;
	}
	
	return RESULT_OK;
}

static uint32_t erase(void *block_start)
{
	status_t status = kStatus_Success;
	uint32_t addr = (uint32_t)(block_start) - FlexSPI_AHB_BASE;
	
	status = flexspi_nor_Erase_Sector(FLEXSPI, addr);
	//status = flexspi_nor_Erase_Block(FLEXSPI, addr);
	
	if(kStatus_Success != status)
    {
        return RESULT_ERROR;
    }
	
	return RESULT_OK;
}

static uint32_t erase_chip(void)
{
	status_t status = kStatus_Success;
	flexspi_transfer_t flashXfer =
	{
		.deviceAddress = 0x00000000UL,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Command,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASECHIP,
		.SeqNumber = 1,
		.data = 0x00000000UL,
		.dataSize = 0,
	};
	
	/* Enable Writting. */
	flexspi_nor_Write_Enable(FLEXSPI);
	
	/* Erase Chip. */
	status = FLEXSPI_TransferBlocking(FLEXSPI, &flashXfer);
    if(kStatus_Success != status)
    {
        return status;
    }
	
	return flexspi_nor_Wait_Bus_If_Busy(FLEXSPI);
}

////////////////////////////////////////////////////////////////////////////////

static status_t flexspi_nor_Get_VendorID(FLEXSPI_Type *base, uint8_t *vendorId)
{
    status_t status = kStatus_Success;
	uint32_t temp = 0x00000000;
	flexspi_transfer_t flashXfer =
	{
		.deviceAddress = 0x00000000UL,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Read,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID,
		.SeqNumber = 1,
		.data = &temp,
		.dataSize = 1,
	};
	
	/* Read VendorID. */
    status = FLEXSPI_TransferBlocking(base, &flashXfer);
	
	*vendorId = temp;

    return status;
}

static status_t flexspi_nor_Erase_Sector(FLEXSPI_Type *base, uint32_t address)
{
	status_t status = kStatus_Success;
	flexspi_transfer_t flashXfer =
	{		
		.deviceAddress = address,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Command,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR,
		.SeqNumber = 1,
		.data = 0x00000000UL,
		.dataSize = 0,
	};
	
	/* Enable Writting. */
	flexspi_nor_Write_Enable(base);
	
	/* Erase a Sector. */
	status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if(kStatus_Success != status)
    {
        return status;
    }
	
	return flexspi_nor_Wait_Bus_If_Busy(base);
}

static status_t flexspi_nor_Erase_Block(FLEXSPI_Type *base, uint32_t address)
{
	status_t status = kStatus_Success;
	flexspi_transfer_t flashXfer =
	{		
		.deviceAddress = address,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Command,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASEBLOCK,
		.SeqNumber = 1,
		.data = 0x00000000UL,
		.dataSize = 0,
	};
	
	/* Enable Writting. */
	flexspi_nor_Write_Enable(base);
	
	/* Erase a Sector. */
	status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if(kStatus_Success != status)
    {
        return status;
    }
	
	return flexspi_nor_Wait_Bus_If_Busy(base);
}


status_t flexspi_nor_Write_Page(FLEXSPI_Type *base, uint32_t dstAddr, uint32_t *src)
{
    status_t status = kStatus_Success;
    flexspi_transfer_t flashXfer = 
	{
		.deviceAddress = dstAddr,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Write,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD,
		.SeqNumber = 1,
		.data = src,
		.dataSize = PAGE_SIZE,
	};

	/* Enable Writting. */
	flexspi_nor_Write_Enable(base);
	
	/* Write Page. */
    status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if (kStatus_Success != status)
    {
        return status;
    }

    return flexspi_nor_Wait_Bus_If_Busy(base);
}

static status_t flexspi_nor_Write_Enable(FLEXSPI_Type *base)
{
	status_t status = kStatus_Success;
	flexspi_transfer_t flashXfer =
	{
		.deviceAddress = 0x00000000UL,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Command,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE,
		.SeqNumber = 1,
		.data = 0x00000000UL,
		.dataSize = 0,
	};
	
	/* Enable Writting. */
	status = FLEXSPI_TransferBlocking(base, &flashXfer);
    if(kStatus_Success != status)
    {
        return status;
    }
	
	return kStatus_Success;
}

static status_t flexspi_nor_Wait_Bus_If_Busy(FLEXSPI_Type *base)
{
	/* Wait status ready. */
    bool isBusy = true;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer = 
	{
		.deviceAddress = 0x00000000UL,
		.port = kFLEXSPI_PortA1,
		.cmdType = kFLEXSPI_Read,
    	.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG,
		.SeqNumber = 1,
		.data = &readValue,
		.dataSize = 1,
	};
	
    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);
        if(kStatus_Success != status)
        {
            return status;
        }
		
        if(FLASH_BUSY_STATUS_POL)
        {
            if(readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else
        {
            if(readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

//        if (readValue & FLASH_ERROR_STATUS_MASK)
//        {
//            status = kStatus_Fail;
//            break;
//        }

    } while(isBusy);

    return status;
}