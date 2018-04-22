/*************************************************************************
 *
 * Common LUT definitions
 *
 **************************************************************************/
 
#define FlexSPI_AHB_BASE 0x60000000ul

/*FlexSPI controller definitions*/
#define FLEXSPI_LUT_INST(code,pads,opr)       ((uint16_t)((code<<10) | (pads<<8) | (opr)))

#define LUT_CODE_STOP         0x00
#define LUT_CODE_CMD_SDR      0x01
#define LUT_CODE_CMD_DDR      0x21
#define LUT_CODE_RADDR_SDR    0x02
#define LUT_CODE_RADDR_DDR    0x22
#define LUT_CODE_CADDR_SDR    0x03
#define LUT_CODE_CADDR_DDR    0x23
#define LUT_CODE_MODE1_SDR    0x04
#define LUT_CODE_MODE1_DDR    0x24
#define LUT_CODE_MODE2_SDR    0x05
#define LUT_CODE_MODE2_DDR    0x25
#define LUT_CODE_MODE4_SDR    0x06
#define LUT_CODE_MODE4_DDR    0x26
#define LUT_CODE_MODE8_SDR    0x07
#define LUT_CODE_MODE8_DDR    0x27
#define LUT_CODE_WRITE_SDR    0x08
#define LUT_CODE_WRITE_DDR    0x28
#define LUT_CODE_READ_SDR     0x09
#define LUT_CODE_READ_DDR     0x29
#define LUT_CODE_LEARN_SDR    0x0A
#define LUT_CODE_LEARN_DDR    0x2A
#define LUT_CODE_DATSZ_SDR    0x0B
#define LUT_CODE_DATSZ_DDR    0x2B
#define LUT_CODE_DUMMY_SDR    0x0C
#define LUT_CODE_DUMMY_DDR    0x2C
#define LUT_CODE_DUMMY_RWDS_SDR  0x0D
#define LUT_CODE_DUMMY_RWDS_DDR  0x2D
#define LUT_CODE_JMP_ON_CS    0x1F

#define LUT_PADS_ONE         0
#define LUT_PADS_TWO         1
#define LUT_PADS_FOUR        2
#define LUT_PADS_EIGHT       3

#define LUT_SEQUENCE_0       0
#define LUT_SEQUENCE_1       1
#define LUT_SEQUENCE_2       2
#define LUT_SEQUENCE_3       3
#define LUT_SEQUENCE_4       4
#define LUT_SEQUENCE_5       5
#define LUT_SEQUENCE_6       6
#define LUT_SEQUENCE_7       7
#define LUT_SEQUENCE_8       8
#define LUT_SEQUENCE_9       9
#define LUT_SEQUENCE_10     10
#define LUT_SEQUENCE_11     11
#define LUT_SEQUENCE_12     12
#define LUT_SEQUENCE_13     13
#define LUT_SEQUENCE_14     14
#define LUT_SEQUENCE_15     15
