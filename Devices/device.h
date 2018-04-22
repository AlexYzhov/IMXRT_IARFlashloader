/*************************************************************************
 *
 * device definitons
 *
 **************************************************************************/
#include <stdint.h>
#include "flash_config.h"

#if defined(IMXRT1052_EVK)
	#define CPU_MIMXRT1052
#elif defined(SPHINX_EVK)
	#define CPU_MIMXRT1052
#elif defined(SHAREBOARD)
	#define CPU_MIMXRT1052
#elif defined(MCIMXRT1020)
#endif

#if defined(MCIMXRT1020)
	#include "MIMXRT1021.h"
#elif defined(CPU_MIMXRT1052)
	#include "MIMXRT1052.h"
#endif

/*device struct*/
typedef struct{
#if USE_ARGC_ARGV
  uint32_t (*init)(void *base_of_flash,int argc, char const *argv[]);
#else
  uint32_t (*init)(void *base_of_flash);
#endif /* USE_ARGC_ARGV */
  uint32_t (*write)(uint32_t addr,uint32_t count,char const *buffer);
  uint32_t (*erase)(void *block_start);
  uint32_t (*erase_chip)(void);
  uint32_t (*signoff)(void);
} device_t;

extern const device_t HyperFlash_S26KS512;
extern const device_t OctaFlash_MX25UM;
extern const device_t QSPIFlash_IS25WP064;
extern const device_t QSPIFlash_25Q128JVSQ;
extern const device_t QSPIFlash_25Q256JVEQ;
extern const device_t SDRAM_MT48LC16M;

