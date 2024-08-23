
#ifndef _PAM_DELAY_H_
#define _PAM_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
void pamDelayInit(void);

void pamHandShakeLteResHandle(void *param);

void pamGnssColdStartDelay(uint8_t gnss_sleep);

void pamLteServiceDelay(uint8_t lte_sleep);

uint8_t _checkNetWorkAndActivateStatus(void);

void pamLteAndGnssServiceCoexistHandle(void *param);

#endif
#endif