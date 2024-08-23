
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
/*=================================================================

						EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN			  WHO		  WHAT, WHERE, WHY
------------	 -------	 -------------------------------------------------------------------------------

=================================================================*/


#ifndef _GNSSDEMO_H
#define _GNSSDEMO_H

#include "ql_gnss.h"

#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

/*===========================================================================
 * Struct
 ===========================================================================*/


/*===========================================================================
 * Functions declaration
 ===========================================================================*/
void ql_gnss_app_init(void);
nmea_type nmea_get_type(const char *sentence);
struct nmea_s* nmea_parse(char *sentence, int length, int check_checksum);
int nmea_value_update(struct nmea_s *nmea, ql_gnss_data_t *gps_data);
char *strptime(const char *buf, const char *fmt, struct tm *tm);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* _GNSSDEMO_H */


