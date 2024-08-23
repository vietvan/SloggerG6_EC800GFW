/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
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

//#define OSI_LOCAL_LOG_LEVEL OSI_LOG_LEVEL_DEBUG

#include "hal_config.h"
#include "boot_spi_flash.h"
#include "hal_spi_flash.h"
#include "hal_spi_flash_defs.h"
#include "boot_platform.h"
#include "hwregs.h"
#include "osi_api.h"
#include "osi_log.h"
#include "osi_byte_buf.h"
#include "drv_names.h"
#include <string.h>
#include "quec_proj_config.h"

#define DELAYUS(us) osiDelayUS(us)
#define FLASH_MAP_ADDRESS(d, offset) (const void *)REG_ACCESS_ADDRESS((uintptr_t)d->base_address + offset)

#ifndef CONFIG_QUEC_PROJECT_FEATURE
#define SIZE_4K (4 * 1024)
#define SIZE_32K (32 * 1024)
#define SIZE_64K (64 * 1024)
#endif

#define PAGE_SIZE (256)
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
#define size_16M 0x1000000
#define size_32M 0x2000000
#endif

struct bootSpiFlash
{
    bool opened;
    unsigned name;
    halSpiFlash_t flash;
    uintptr_t base_address;
    uint32_t block_prohibit[8]; // 16M/64K/32bit_per_word
#ifdef CONFIG_MED_CODE_ENCRYPT
    uint32_t med_buf_size;
    void *med_input_buf;
    void *med_remap_buf;
#endif
};

#ifdef CONFIG_SOC_8910
static bootSpiFlash_t gDrvSpiFlashCtx[] = {
    {
        .opened = false,
        .name = DRV_NAME_SPI_FLASH,
        .flash.hwp = (uintptr_t)hwp_spiFlash,
        .base_address = CONFIG_NOR_PHY_ADDRESS,
    },
    {
        .opened = false,
        .name = DRV_NAME_SPI_FLASH_EXT,
        .flash.hwp = (uintptr_t)hwp_spiFlashExt,
        .base_address = CONFIG_NOR_EXT_PHY_ADDRESS,
    },
};
#endif

#ifdef CONFIG_MED_CODE_ENCRYPT
static OSI_ALIGNED(32) uint8_t gMedBuffer[PAGE_SIZE * 2];
static bool prvDataMedEncrypt(bootSpiFlash_t *d, uint32_t offset, size_t size)
{
    uint32_t base = d->base_address + offset;
    REG_MED_MED_CH0_WORK_CFG_T ch0_work_cfg = {hwp_med->med_ch0_work_cfg};
    if (ch0_work_cfg.b.med_ch0_enable && !ch0_work_cfg.b.med_ch0_bypass_en)
    {
        if (base >= hwp_med->med_ch0_base_addr_cfg &&
            base + size <= hwp_med->med_ch0_base_addr_cfg + hwp_med->med_ch0_addr_size_cfg)
            return true;
    }

    return false;
}
#endif

#if defined(CONFIG_SOC_8811) || defined(CONFIG_SOC_8850)
static bootSpiFlash_t gDrvSpiFlashCtx[] = {
    {
        .opened = false,
        .name = DRV_NAME_SPI_FLASH,
        .flash.hwp = (uintptr_t)hwp_spiFlash,
        .base_address = CONFIG_NOR_PHY_ADDRESS,
#ifdef CONFIG_MED_CODE_ENCRYPT
        .med_buf_size = PAGE_SIZE,
        .med_input_buf = gMedBuffer,
        .med_remap_buf = &gMedBuffer[PAGE_SIZE],
#endif
    },
    {
        .opened = false,
        .name = DRV_NAME_SPI_FLASH_EXT,
        .flash.hwp = (uintptr_t)hwp_spiFlashExt,
        .base_address = CONFIG_NOR_EXT_PHY_ADDRESS,
#ifdef CONFIG_MED_CODE_ENCRYPT
        .med_buf_size = PAGE_SIZE,
        .med_input_buf = gMedBuffer,
        .med_remap_buf = &gMedBuffer[PAGE_SIZE],
#endif
    },
};
#endif

static bootSpiFlash_t *prvFlashGetByName(unsigned name)
{
    for (unsigned n = 0; n < OSI_ARRAY_SIZE(gDrvSpiFlashCtx); n++)
    {
        if (gDrvSpiFlashCtx[n].name == name)
            return &gDrvSpiFlashCtx[n];
    }
    return NULL;
}

static inline bool prvIsWriteProhibit(bootSpiFlash_t *d, uint32_t address)
{
    unsigned block = address / SIZE_64K; // 64KB block
    return osiBitmapIsSet(d->block_prohibit, block);
}

static void prvPageProgram(bootSpiFlash_t *d, uint32_t offset, const uint8_t *data, size_t size)
{
#ifdef CONFIG_MED_CODE_ENCRYPT
    if (prvDataMedEncrypt(d, offset, size))
    {
        uint32_t copy_size = OSI_ALIGN_UP(size, 32);
        if (copy_size > d->med_buf_size)
        {
            osiPanic();
        }

        hwp_med->med_write_addr_remap = (uint32_t)d->med_remap_buf;
        hwp_med->med_write_base_addr_cfg = (uint32_t)d->base_address + offset;
        hwp_med->med_write_addr_size_cfg = copy_size - 1;
        hwp_med->med_int_clear = 0xffffffff;

        uint32_t *flash_addr = (uint32_t *)(d->base_address + offset);
        for (unsigned i = 0; i < (copy_size / 4); i++)
        {
            flash_addr[i] = ((uint32_t *)data)[i];
        }

        osiDCacheClean(flash_addr, copy_size);

        //        OSI_LOGI(0xffffffff, "0x%x", flash_addr);   // add this line, can finish the download process, but crc check error.

        REG_MED_MED_INT_RAW_T med_int_raw = {};
        REG_WAIT_FIELD_NEZ(med_int_raw, hwp_med->med_int_raw, med_wr_done_int_raw);
        hwp_med->med_clr = 0xffffffff;

        osiDCacheInvalidate(d->med_remap_buf, copy_size);
        data = d->med_remap_buf;
    }
#endif
    halSpiFlashPrepareEraseProgram(&d->flash, offset, size);
    halSpiFlashPageProgram(&d->flash, offset, data, size);
}

static void prvErase(bootSpiFlash_t *d, uint32_t offset, size_t size)
{
    halSpiFlashPrepareEraseProgram(&d->flash, offset, size);
    halSpiFlashErase(&d->flash, offset, size);
}

static void prvFlashOpen(bootSpiFlash_t *d)
{
    if (d->opened)
        return;

    halSpiFlashInit(&d->flash);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    if(d->flash.mid == 0)
    {
        d->opened = false;
        return;
    }
#endif
    d->opened = true;
    OSI_LOGD(0x100090c2, "FLASH (0x%x) id/0x%06x cap/0x%x", d->base_address, d->flash.mid, d->flash.capacity);
}

bootSpiFlash_t *bootSpiFlashOpen(unsigned name)
{
    bootSpiFlash_t *d = prvFlashGetByName(name);
    if (d == NULL)
        return NULL;

    prvFlashOpen(d);
#ifdef CONFIG_QUEC_PROJECT_FEATURE_SPI6_EXT_NOR
    if (d->opened == false)
        return NULL;
#endif
    return d;
}

void bootSpiFlashSetRangeWriteProhibit(bootSpiFlash_t *d, uint32_t start, uint32_t end)
{
    if (start > d->flash.capacity || end > d->flash.capacity)
        return;

    unsigned block_start = OSI_ALIGN_UP(start, SIZE_64K) / SIZE_64K;
    unsigned block_end = OSI_ALIGN_DOWN(end, SIZE_64K) / SIZE_64K;
    for (unsigned block = block_start; block < block_end; block++)
        osiBitmapSet(d->block_prohibit, block);
}

void bootSpiFlashClearRangeWriteProhibit(bootSpiFlash_t *d, uint32_t start, uint32_t end)
{
    if (start > d->flash.capacity || end > d->flash.capacity)
        return;

    unsigned block_start = OSI_ALIGN_UP(start, SIZE_64K) / SIZE_64K;
    unsigned block_end = OSI_ALIGN_DOWN(end, SIZE_64K) / SIZE_64K;
    for (unsigned block = block_start; block < block_end; block++)
        osiBitmapClear(d->block_prohibit, block);
}

uint32_t bootSpiFlashID(bootSpiFlash_t *d)
{
    return (d == NULL) ? 0 : d->flash.mid;
}

uint32_t bootSpiFlashCapacity(bootSpiFlash_t *d)
{
    return (d == NULL) ? 0 : d->flash.capacity;
}

const void *bootSpiFlashMapAddress(bootSpiFlash_t *d, uint32_t offset)
{
    return FLASH_MAP_ADDRESS(d, offset);
}

bool bootSpiFlashOffsetValid(bootSpiFlash_t *d, uint32_t offset)
{
    return offset < d->flash.capacity;
}

bool bootSpiFlashMapAddressValid(bootSpiFlash_t *d, const void *address)
{
    uintptr_t ptr = (uintptr_t)address;
    return (ptr >= d->base_address) && (ptr < d->base_address + d->flash.capacity);
}

bool bootSpiFlashIsDone(bootSpiFlash_t *d)
{
    return halSpiFlashIsWipFinished(&d->flash);
}

void bootSpiFlashWaitDone(bootSpiFlash_t *d)
{
    halSpiFlashWaitWipFinish(&d->flash);
    halSpiFlashFinishEraseProgram(&d->flash);
}

int bootSpiFlashWriteNoWait(bootSpiFlash_t *d, uint32_t offset, const void *data, size_t size)
{
    if (data == NULL || offset + size > d->flash.capacity)
        return -1;

    if (prvIsWriteProhibit(d, offset))
        return -1;

    uint32_t next_page = OSI_ALIGN_DOWN(offset + PAGE_SIZE, PAGE_SIZE);
    uint32_t bsize = next_page - offset;
    if (bsize > size)
        bsize = size;

    halSpiFlashWaitWipFinish(&d->flash);
    prvPageProgram(d, offset, data, bsize);
    osiDCacheInvalidate(FLASH_MAP_ADDRESS(d, offset), bsize);
    return bsize;
}

int bootSpiFlashEraseNoWait(bootSpiFlash_t *d, uint32_t offset, size_t size)
{
    if (offset + size > d->flash.capacity)
        return -1;

    if (prvIsWriteProhibit(d, offset))
        return -1;

    bool force_4k = false;
    if (d->flash.force_4k_first_block && offset < SIZE_64K)
        force_4k = true;
    if (d->flash.force_4k_last_block && offset >= d->flash.capacity - SIZE_64K)
        force_4k = true;

    halSpiFlashWaitWipFinish(&d->flash);
    if (!force_4k && OSI_IS_ALIGNED(offset, SIZE_64K) && size >= SIZE_64K)
    {
        prvErase(d, offset, SIZE_64K);
        osiDCacheInvalidate(FLASH_MAP_ADDRESS(d, offset), SIZE_64K);
        return SIZE_64K;
    }
    else if (!force_4k && OSI_IS_ALIGNED(offset, SIZE_32K) && size >= SIZE_32K)
    {
        prvErase(d, offset, SIZE_32K);
        osiDCacheInvalidate(FLASH_MAP_ADDRESS(d, offset), SIZE_32K);
        return SIZE_32K;
    }
    else if (OSI_IS_ALIGNED(offset, SIZE_4K) && size >= SIZE_4K)
    {
        prvErase(d, offset, SIZE_4K);
        osiDCacheInvalidate(FLASH_MAP_ADDRESS(d, offset), SIZE_4K);
        return SIZE_4K;
    }
    else
    {
        return -1;
    }
}

void bootSpiFlashChipErase(bootSpiFlash_t *d)
{
    halSpiFlashWaitWipFinish(&d->flash);
    halSpiFlashPrepareEraseProgram(&d->flash, 0, d->flash.capacity);
    halSpiFlashChipErase(&d->flash);
    halSpiFlashWaitWipFinish(&d->flash);
    halSpiFlashFinishEraseProgram(&d->flash);
    osiDCacheInvalidate(FLASH_MAP_ADDRESS(d, 0), d->flash.capacity);
}

bool bootSpiFlashErase(bootSpiFlash_t *d, uint32_t offset, size_t size)
{
    while (size > 0)
    {
        int esize = bootSpiFlashEraseNoWait(d, offset, size);
        if (esize < 0)
        {
            halSpiFlashFinishEraseProgram(&d->flash);
            return false;
        }

        size -= esize;
        offset += esize;
        halSpiFlashWaitWipFinish(&d->flash);
    }
    halSpiFlashFinishEraseProgram(&d->flash);
    return true;
}

#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
void bootSpiFlash4ByteAddressMode(bootSpiFlash_t *d, uint32_t offset)
{
    if (offset < size_16M)
        return;

    uint32_t critical = osiEnterCritical();
    halSpiFlash4ByteAddressMode(&d->flash);
    REG_SPI_FLASH_SPI_256_512_FLASH_CONFIG_T flash_config = {
        .b.four_byte_addr = 1,
    };
    hwp_spiFlashExt->spi_256_512_flash_config = flash_config.v;

    if (offset >= size_32M)
    {
        REG_SPI_FLASH_SPI_CS_SIZE_T spi_cs_size1 = {
            .b.sel1_flash_1 = 1,
        };

        hwp_spiFlashExt->spi_cs_size = spi_cs_size1.v;
    }
    else if (offset >= size_16M)
    {
        REG_SPI_FLASH_SPI_CS_SIZE_T spi_cs_size1 = {
            .b.sel_flash_1 = 1,
        };

        hwp_spiFlashExt->spi_cs_size = spi_cs_size1.v;
    }

    osiExitCritical(critical);
    return;
}

void bootSpiFlashDisenable4ByteAddressMode(bootSpiFlash_t *d, uint32_t offset)
{
    if (offset < size_16M)
        return;

    uint32_t critical = osiEnterCritical();
    halSpiFlashDisenable4ByteAddressMode(&d->flash);
    REG_SPI_FLASH_SPI_256_512_FLASH_CONFIG_T flash_config = {
        .b.four_byte_addr = 0,
    };
    hwp_spiFlashExt->spi_256_512_flash_config = flash_config.v;

    if (offset >= size_32M)
    {
        REG_SPI_FLASH_SPI_CS_SIZE_T spi_cs_size1 = {
            .b.sel1_flash_1 = 0,
        };
        hwp_spiFlashExt->spi_cs_size = spi_cs_size1.v;
    }
    else if (offset >= size_16M)
    {
        REG_SPI_FLASH_SPI_CS_SIZE_T spi_cs_size1 = {
            .b.sel_flash_1 = 0,
        };
        hwp_spiFlashExt->spi_cs_size = spi_cs_size1.v;
    }

    osiExitCritical(critical);
    return;
}
#endif

bool bootSpiFlashWrite(bootSpiFlash_t *d, uint32_t offset, const void *data, size_t size)
{
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH) //large capacity flash
    bootSpiFlash4ByteAddressMode(d, offset);
#endif
    while (size > 0)
    {
        int wsize = bootSpiFlashWriteNoWait(d, offset, data, size);
        if (wsize < 0)
        {
            halSpiFlashFinishEraseProgram(&d->flash);
            return false;
        }

        size -= wsize;
        offset += wsize;
        data = (const char *)data + wsize;
        halSpiFlashWaitWipFinish(&d->flash);
    }
    halSpiFlashFinishEraseProgram(&d->flash);
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
    bootSpiFlashDisenable4ByteAddressMode(d, offset);
#endif
    return true;
}

/**
 * read data from flash
 */
bool bootSpiFlashRead(bootSpiFlash_t *d, uint32_t offset, void *data, uint32_t size)
{
    if (size == 0)
        return true;
    if (d == NULL || data == NULL || offset + size > d->flash.capacity)
        return false;
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
    bootSpiFlash4ByteAddressMode(d, offset);
#endif
    const void *fl = FLASH_MAP_ADDRESS(d, offset);
    memcpy(data, fl, size);
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
    bootSpiFlashDisenable4ByteAddressMode(d, offset);
#endif
    return true;
}

/**
 * read data from flash, and check with provided data
 */
bool bootSpiFlashReadCheck(bootSpiFlash_t *d, uint32_t offset, const void *data, uint32_t size)
{
    if (size == 0)
        return true;
    if (d == NULL || data == NULL || offset + size > d->flash.capacity)
        return false;
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
    bootSpiFlash4ByteAddressMode(d, offset);
#endif
    const void *fl = FLASH_MAP_ADDRESS(d, offset);
    bool flag = false;
    if (memcmp(data, fl, size) == 0)
        flag = true;
#if defined(CONFIG_SOC_8910) && defined(CONFIG_SUPPORT_LC_FLASH)
    bootSpiFlashDisenable4ByteAddressMode(d, offset);
#endif
    return flag;
}

int bootSpiFlashReadUniqueId(bootSpiFlash_t *d, uint8_t *id)
{
    return halSpiFlashReadUniqueId(&d->flash, id);
}
