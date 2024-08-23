/* Copyright (C) 2020 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _SRV_USB_H_
#define _SRV_USB_H_

#include "osi_compiler.h"
#include <stdbool.h>

OSI_EXTERN_C_BEGIN

bool srvUsbInit();
bool srvUsb8910Init(bool IsDeviceMode);
bool srv8850UsbInit(bool IsDeviceMode);

OSI_EXTERN_C_END

#endif