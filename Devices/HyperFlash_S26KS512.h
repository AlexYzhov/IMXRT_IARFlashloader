#ifndef _HYPERFLASH_S26KS512_H_
#define _HYPERFLASH_S26KS512_H_

/** FLEXSPI - Peripheral register structure */
typedef struct FLEXSPI_MemMap {
  uint32_t MCR0;                                   /**< Module Control Register 0, offset: 0x0 */
  uint32_t MCR1;                                   /**< Module Control Register 1, offset: 0x4 */
  uint32_t MCR2;                                   /**< Module Control Register 2, offset: 0x8 */
  uint32_t AHBCR;                                  /**< AHB Bus Control Register, offset: 0xC */
  uint32_t INTEN;                                  /**< Interrupt Enable Register, offset: 0x10 */
  uint32_t INTR;                                   /**< Interrupt Register, offset: 0x14 */
  uint32_t LUTKEY;                                 /**< LUT Key Register, offset: 0x18 */
  uint32_t LUTCR;                                  /**< LUT Control Register, offset: 0x1C */
  uint32_t AHBRXBUFCR0[4];                         /**< AHB RX Buffer 0 Control Register 0..AHB RX Buffer 3 Control Register 0, array offset: 0x20, array step: 0x4 */
  uint8_t RESERVED_0[48];
  uint32_t FLSHCR0[4];                             /**< Flash A1 Control Register 0..Flash B2 Control Register 0, array offset: 0x60, array step: 0x4 */
  uint32_t FLSHCR1[4];                             /**< Flash A1 Control Register 1..Flash B2 Control Register 1, array offset: 0x70, array step: 0x4 */
  uint32_t FLSHCR2[4];                             /**< Flash A1 Control Register 2..Flash B2 Control Register 2, array offset: 0x80, array step: 0x4 */
  uint8_t RESERVED_1[4];
  uint32_t FLSHCR4;                                /**< Flash Control Register 4, offset: 0x94 */
  uint8_t RESERVED_2[8];
  uint32_t IPCR0;                                  /**< IP Control Register 0, offset: 0xA0 */
  uint32_t IPCR1;                                  /**< IP Control Register 1, offset: 0xA4 */
  uint8_t RESERVED_3[8];
  uint32_t IPCMD;                                  /**< IP Command Register, offset: 0xB0 */
  uint8_t RESERVED_4[4];
  uint32_t IPRXFCR;                                /**< IP RX FIFO Control Register, offset: 0xB8 */
  uint32_t IPTXFCR;                                /**< IP TX FIFO Control Register, offset: 0xBC */
  uint32_t DLLCR[2];                               /**< DLL Control Register 0, array offset: 0xC0, array step: 0x4 */
  uint8_t RESERVED_5[24];
  uint32_t STS0;                                   /**< Status Register 0, offset: 0xE0 */
  uint32_t STS1;                                   /**< Status Register 1, offset: 0xE4 */
  uint32_t STS2;                                   /**< Status Register 2, offset: 0xE8 */
  uint32_t AHBSPNDSTS;                             /**< AHB Suspend Status Register, offset: 0xEC */
  uint32_t IPRXFSTS;                               /**< IP RX FIFO Status Register, offset: 0xF0 */
  uint32_t IPTXFSTS;                               /**< IP TX FIFO Status Register, offset: 0xF4 */
  uint8_t RESERVED_6[8];
  uint32_t RFDR[32];                               /**< IP RX FIFO Data Register 0..IP RX FIFO Data Register 31, array offset: 0x100, array step: 0x4 */
  uint32_t TFDR[32];                               /**< IP TX FIFO Data Register 0..IP TX FIFO Data Register 31, array offset: 0x180, array step: 0x4 */
  uint32_t LUT[64];                                /**< LUT 0..LUT 63, array offset: 0x200, array step: 0x4 */
} volatile *FLEXSPI_MemMapPtr;

/* FLEXSPI - Peripheral instance base addresses */
/** Peripheral FLEXSPI base pointer */
#define FLEXSPI_BASE_PTR                         ((FLEXSPI_MemMapPtr)0x402A8000u)

/* FLEXSPI - Register accessors */
#define FLEXSPI_MCR0_REG(base)                   ((base)->MCR0)
#define FLEXSPI_MCR1_REG(base)                   ((base)->MCR1)
#define FLEXSPI_MCR2_REG(base)                   ((base)->MCR2)
#define FLEXSPI_AHBCR_REG(base)                  ((base)->AHBCR)
#define FLEXSPI_INTEN_REG(base)                  ((base)->INTEN)
#define FLEXSPI_INTR_REG(base)                   ((base)->INTR)
#define FLEXSPI_LUTKEY_REG(base)                 ((base)->LUTKEY)
#define FLEXSPI_LUTCR_REG(base)                  ((base)->LUTCR)
#define FLEXSPI_AHBRXBUFCR0_REG(base,index)      ((base)->AHBRXBUFCR0[index])
#define FLEXSPI_FLSHCR0_REG(base,index)          ((base)->FLSHCR0[index])
#define FLEXSPI_FLSHCR1_REG(base,index)          ((base)->FLSHCR1[index])
#define FLEXSPI_FLSHCR2_REG(base,index)          ((base)->FLSHCR2[index])
#define FLEXSPI_FLSHCR4_REG(base)                ((base)->FLSHCR4)
#define FLEXSPI_IPCR0_REG(base)                  ((base)->IPCR0)
#define FLEXSPI_IPCR1_REG(base)                  ((base)->IPCR1)
#define FLEXSPI_IPCMD_REG(base)                  ((base)->IPCMD)
#define FLEXSPI_IPRXFCR_REG(base)                ((base)->IPRXFCR)
#define FLEXSPI_IPTXFCR_REG(base)                ((base)->IPTXFCR)
#define FLEXSPI_DLLCR_REG(base,index)            ((base)->DLLCR[index])
#define FLEXSPI_STS0_REG(base)                   ((base)->STS0)
#define FLEXSPI_STS1_REG(base)                   ((base)->STS1)
#define FLEXSPI_STS2_REG(base)                   ((base)->STS2)
#define FLEXSPI_AHBSPNDSTS_REG(base)             ((base)->AHBSPNDSTS)
#define FLEXSPI_IPRXFSTS_REG(base)               ((base)->IPRXFSTS)
#define FLEXSPI_IPTXFSTS_REG(base)               ((base)->IPTXFSTS)
#define FLEXSPI_RFDR_REG(base,index)             ((base)->RFDR[index])
#define FLEXSPI_TFDR_REG(base,index)             ((base)->TFDR[index])
#define FLEXSPI_LUT_REG(base,index)              ((base)->LUT[index])

/* FLEXSPI - Register instance definitions */
/* FLEXSPI */
#define FLEXSPI_MCR0                             FLEXSPI_MCR0_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_MCR1                             FLEXSPI_MCR1_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_MCR2                             FLEXSPI_MCR2_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_AHBCR                            FLEXSPI_AHBCR_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_INTEN                            FLEXSPI_INTEN_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_INTR                             FLEXSPI_INTR_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_LUTKEY                           FLEXSPI_LUTKEY_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_LUTCR                            FLEXSPI_LUTCR_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_AHBRXBUF0CR0                     FLEXSPI_AHBRXBUFCR0_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_AHBRXBUF1CR0                     FLEXSPI_AHBRXBUFCR0_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_AHBRXBUF2CR0                     FLEXSPI_AHBRXBUFCR0_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_AHBRXBUF3CR0                     FLEXSPI_AHBRXBUFCR0_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_FLSHA1CR0                        FLEXSPI_FLSHCR0_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_FLSHA2CR0                        FLEXSPI_FLSHCR0_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_FLSHB1CR0                        FLEXSPI_FLSHCR0_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_FLSHB2CR0                        FLEXSPI_FLSHCR0_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_FLSHA1CR1                        FLEXSPI_FLSHCR1_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_FLSHA2CR1                        FLEXSPI_FLSHCR1_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_FLSHB1CR1                        FLEXSPI_FLSHCR1_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_FLSHB2CR1                        FLEXSPI_FLSHCR1_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_FLSHA1CR2                        FLEXSPI_FLSHCR2_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_FLSHA2CR2                        FLEXSPI_FLSHCR2_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_FLSHB1CR2                        FLEXSPI_FLSHCR2_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_FLSHB2CR2                        FLEXSPI_FLSHCR2_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_FLSHCR4                          FLEXSPI_FLSHCR4_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPCR0                            FLEXSPI_IPCR0_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPCR1                            FLEXSPI_IPCR1_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPCMD                            FLEXSPI_IPCMD_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPRXFCR                          FLEXSPI_IPRXFCR_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPTXFCR                          FLEXSPI_IPTXFCR_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_DLLACR                           FLEXSPI_DLLCR_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_DLLBCR                           FLEXSPI_DLLCR_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_STS0                             FLEXSPI_STS0_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_STS1                             FLEXSPI_STS1_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_STS2                             FLEXSPI_STS2_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_AHBSPNDSTS                       FLEXSPI_AHBSPNDSTS_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPRXFSTS                         FLEXSPI_IPRXFSTS_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_IPTXFSTS                         FLEXSPI_IPTXFSTS_REG(FLEXSPI_BASE_PTR)
#define FLEXSPI_RFDR0                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_RFDR1                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_RFDR2                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_RFDR3                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_RFDR4                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,4)
#define FLEXSPI_RFDR5                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,5)
#define FLEXSPI_RFDR6                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,6)
#define FLEXSPI_RFDR7                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,7)
#define FLEXSPI_RFDR8                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,8)
#define FLEXSPI_RFDR9                            FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,9)
#define FLEXSPI_RFDR10                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,10)
#define FLEXSPI_RFDR11                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,11)
#define FLEXSPI_RFDR12                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,12)
#define FLEXSPI_RFDR13                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,13)
#define FLEXSPI_RFDR14                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,14)
#define FLEXSPI_RFDR15                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,15)
#define FLEXSPI_RFDR16                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,16)
#define FLEXSPI_RFDR17                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,17)
#define FLEXSPI_RFDR18                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,18)
#define FLEXSPI_RFDR19                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,19)
#define FLEXSPI_RFDR20                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,20)
#define FLEXSPI_RFDR21                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,21)
#define FLEXSPI_RFDR22                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,22)
#define FLEXSPI_RFDR23                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,23)
#define FLEXSPI_RFDR24                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,24)
#define FLEXSPI_RFDR25                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,25)
#define FLEXSPI_RFDR26                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,26)
#define FLEXSPI_RFDR27                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,27)
#define FLEXSPI_RFDR28                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,28)
#define FLEXSPI_RFDR29                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,29)
#define FLEXSPI_RFDR30                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,30)
#define FLEXSPI_RFDR31                           FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,31)
#define FLEXSPI_TFDR0                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_TFDR1                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_TFDR2                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_TFDR3                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_TFDR4                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,4)
#define FLEXSPI_TFDR5                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,5)
#define FLEXSPI_TFDR6                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,6)
#define FLEXSPI_TFDR7                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,7)
#define FLEXSPI_TFDR8                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,8)
#define FLEXSPI_TFDR9                            FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,9)
#define FLEXSPI_TFDR10                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,10)
#define FLEXSPI_TFDR11                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,11)
#define FLEXSPI_TFDR12                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,12)
#define FLEXSPI_TFDR13                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,13)
#define FLEXSPI_TFDR14                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,14)
#define FLEXSPI_TFDR15                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,15)
#define FLEXSPI_TFDR16                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,16)
#define FLEXSPI_TFDR17                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,17)
#define FLEXSPI_TFDR18                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,18)
#define FLEXSPI_TFDR19                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,19)
#define FLEXSPI_TFDR20                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,20)
#define FLEXSPI_TFDR21                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,21)
#define FLEXSPI_TFDR22                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,22)
#define FLEXSPI_TFDR23                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,23)
#define FLEXSPI_TFDR24                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,24)
#define FLEXSPI_TFDR25                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,25)
#define FLEXSPI_TFDR26                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,26)
#define FLEXSPI_TFDR27                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,27)
#define FLEXSPI_TFDR28                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,28)
#define FLEXSPI_TFDR29                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,29)
#define FLEXSPI_TFDR30                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,30)
#define FLEXSPI_TFDR31                           FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,31)
#define FLEXSPI_LUT0                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,0)
#define FLEXSPI_LUT1                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,1)
#define FLEXSPI_LUT2                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,2)
#define FLEXSPI_LUT3                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,3)
#define FLEXSPI_LUT4                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,4)
#define FLEXSPI_LUT5                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,5)
#define FLEXSPI_LUT6                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,6)
#define FLEXSPI_LUT7                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,7)
#define FLEXSPI_LUT8                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,8)
#define FLEXSPI_LUT9                             FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,9)
#define FLEXSPI_LUT10                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,10)
#define FLEXSPI_LUT11                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,11)
#define FLEXSPI_LUT12                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,12)
#define FLEXSPI_LUT13                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,13)
#define FLEXSPI_LUT14                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,14)
#define FLEXSPI_LUT15                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,15)
#define FLEXSPI_LUT16                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,16)
#define FLEXSPI_LUT17                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,17)
#define FLEXSPI_LUT18                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,18)
#define FLEXSPI_LUT19                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,19)
#define FLEXSPI_LUT20                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,20)
#define FLEXSPI_LUT21                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,21)
#define FLEXSPI_LUT22                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,22)
#define FLEXSPI_LUT23                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,23)
#define FLEXSPI_LUT24                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,24)
#define FLEXSPI_LUT25                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,25)
#define FLEXSPI_LUT26                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,26)
#define FLEXSPI_LUT27                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,27)
#define FLEXSPI_LUT28                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,28)
#define FLEXSPI_LUT29                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,29)
#define FLEXSPI_LUT30                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,30)
#define FLEXSPI_LUT31                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,31)
#define FLEXSPI_LUT32                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,32)
#define FLEXSPI_LUT33                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,33)
#define FLEXSPI_LUT34                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,34)
#define FLEXSPI_LUT35                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,35)
#define FLEXSPI_LUT36                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,36)
#define FLEXSPI_LUT37                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,37)
#define FLEXSPI_LUT38                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,38)
#define FLEXSPI_LUT39                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,39)
#define FLEXSPI_LUT40                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,40)
#define FLEXSPI_LUT41                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,41)
#define FLEXSPI_LUT42                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,42)
#define FLEXSPI_LUT43                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,43)
#define FLEXSPI_LUT44                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,44)
#define FLEXSPI_LUT45                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,45)
#define FLEXSPI_LUT46                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,46)
#define FLEXSPI_LUT47                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,47)
#define FLEXSPI_LUT48                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,48)
#define FLEXSPI_LUT49                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,49)
#define FLEXSPI_LUT50                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,50)
#define FLEXSPI_LUT51                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,51)
#define FLEXSPI_LUT52                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,52)
#define FLEXSPI_LUT53                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,53)
#define FLEXSPI_LUT54                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,54)
#define FLEXSPI_LUT55                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,55)
#define FLEXSPI_LUT56                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,56)
#define FLEXSPI_LUT57                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,57)
#define FLEXSPI_LUT58                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,58)
#define FLEXSPI_LUT59                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,59)
#define FLEXSPI_LUT60                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,60)
#define FLEXSPI_LUT61                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,61)
#define FLEXSPI_LUT62                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,62)
#define FLEXSPI_LUT63                            FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,63)

/* FLEXSPI - Register array accessors */
#define FLEXSPI_AHBRXBUFCR0(index)               FLEXSPI_AHBRXBUFCR0_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_FLSHCR0(index)                   FLEXSPI_FLSHCR0_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_FLSHCR1(index)                   FLEXSPI_FLSHCR1_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_FLSHCR2(index)                   FLEXSPI_FLSHCR2_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_DLLCR(index)                     FLEXSPI_DLLCR_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_RFDR(index)                      FLEXSPI_RFDR_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_TFDR(index)                      FLEXSPI_TFDR_REG(FLEXSPI_BASE_PTR,index)
#define FLEXSPI_LUT(index)                       FLEXSPI_LUT_REG(FLEXSPI_BASE_PTR,index)

#endif