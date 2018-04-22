/*************************************************************************
*
*   Used with ICCARM and AARM.
*
*    (c) Copyright IAR Systems 2017
*
*    File name   : Flash.c
*    Description : SphinxEVK OctaFlash flash loader
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
#include "flash_loader_extra.h"

#include "device.h"
#include "lut.h"

/** public data **/
const device_t OctaFlash_MX25UM;
/*{
.init = init,
.write = write,
.erase = erase,
.erase_chip = erase_chip,
.signoff = signoff
};*/