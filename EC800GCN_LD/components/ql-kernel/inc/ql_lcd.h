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


#ifndef QL_LCD_H
#define QL_LCD_H


#ifdef __cplusplus
extern "C" {
#endif

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QL_LCD_ERRCODE_BASE (QL_COMPONENT_BSP_LCD<<16)

/*===========================================================================
 * Enum
 ===========================================================================*/
/**
 * LCD errcode
 */
typedef enum
{
    QL_LCD_SUCCESS = QL_SUCCESS,

    QL_LCD_EXECUTE_ERR                 = 1|QL_LCD_ERRCODE_BASE,
    QL_LCD_INVALID_PARAM_ERR,
    QL_LCD_INIT_ERR,
    QL_LCD_BUFFER_CREATE_ERR,
    QL_LCD_BLOCK_SET_ERR,
    QL_LCD_OPERATION_ERR,
    QL_LCD_NOT_INIT_ERR,

    QL_LCD_CREATE_MUTEX_ERR,
    QL_LCD_MUTEX_TIMEOUT_ERR
}ql_errcode_lcd;

/**
 * LCD bus mode
 */
typedef enum
{
    QL_LCD_SPI_LINE_4        = 0,
    QL_LCD_SPI_LINE_3        = 1,
    QL_LCD_SPI_LINE_3_2_LANE = 3,

    QL_LCD_SPI_LINE_MAX
}ql_lcd_busmode_s;

/**
 * LCD bwscreen driver select index
 * this index is equal to the index of ql_lcd_bwscreen_tab in drv_lcd_cfg.c
 */
typedef enum
{
    QL_LCD_BW_ST7567A        = 0,    // don't change name&num
    // customer add bwscreen driver from here...

    QL_LCD_BW_MAX
}ql_lcd_bwscreen_s;

typedef enum
{
    QL_LCD_RGB565       = 2,    // don't change name&num
    QL_LCD_YUV422_UYVY  = 5,

    QL_LCD_RESOLUTION_MAX
}ql_lcd_resolution_s;

/**
 * LCD backlight pin select
 */
typedef enum
{
    QL_LCD_BL_SEL_ISINK    = 0,

    QL_LCD_BL_SEL_MAX
}QL_LCD_BLSEL;

/*===========================================================================
 * Struct
 ===========================================================================*/
typedef struct
{
    uint32_t  drv_id;
    uint32_t  width;
    uint32_t  height;
    ql_lcd_busmode_s bus_mode;
    uint32_t  freq;
} ql_lcd_info_t;

typedef struct
{
    uint8_t index;
} ql_lcd_bw_info_t;

typedef struct
{
    uint16_t lcdrotation;                  //LCD rotation
    uint16_t widthoriginal;                //width of display
    ql_lcd_resolution_s lcdcolorformat;    //LCD color format
    bool clean_screen;                     //clean_screen
    ql_lcd_busmode_s bus_mode;             // driver busmode
    uint32_t freq;                         // driver frequence
} ql_lcd_config_t;

/*===========================================================================
 * Functions declaration
 ===========================================================================*/
/**
 * @brief Extend init the LCD moudle
 * @param color : background color
 * @param backlight_sw : whether to open backlight in LCD initialize program
                         if this argument is false,
                         customers need set ql_lcd_set_brightness.
 * @return
 *      QL_LCD_INIT_ERR
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_init_ext(uint16_t color, bool backlight_sw);

/**
 * @brief Init the LCD moudle
 * @return
 *      QL_LCD_INIT_ERR
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_init(void);

/**
 * @brief Init the LCD(st7567a) moudle
 * @return
 *      QL_LCD_INIT_ERR
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_init_st7567a(void);

/**
 * @brief Init the LCD bwscreen driver
 * @param bw_info
 *         .index : this index is equal to the index of ql_lcd_bwscreen_tab in drv_lcd_cfg.c
 * @return
 *      QL_LCD_INIT_ERR
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_init_bwscreen(ql_lcd_bw_info_t bw_info);

/**
 * @brief Free LCD memory space
 * @return
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_free(void);

/*** this API apply to color screen driver,          ***/
/*** if driver is bwscreen, apply only to ST7567A.   ***/
/*** other bwscreen directly use write cmd&data API. ***/
/**
 * @brief Clear screen background to a kind of color
 * @param color : background color
 * @return
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_clear_screen(uint16_t color);

/*** this API is apply to LCD's driver what except bwscreen ***/
/*** if driver is bwscreen, directly use write cmd&data API ***/
/*** location param is page and column                      ***/
/**
 * @brief LCD part area set
 * @param buffer  : write-in data buffer
 * @param start_x : horizontal coordinate x start location
 * @param start_y : vertical coordinate y start location
 * @param end_x   : horizontal coordinate x end location
 * @param end_x   : vertical coordinate y end location
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_BLOCK_SET_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_write(uint16_t *buffer, uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y);

/**
 * @brief LCD display turn on
 * @return
 *      QL_LCD_OPERATION_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_display_on(void);

/**
 * @brief LCD display turn off
 * @return
 *      QL_LCD_OPERATION_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_display_off(void);

/**
 * @brief LCD SPI CMD send interface
 * @param cmd : LCD SPI CMD param
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_spi_lcd_write_cmd(uint8_t cmd);

/**
 * @brief LCD SPI DATA send interface
 * @param data : LCD SPI DATA param
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_spi_lcd_write_data(uint8_t data);

/**
 * @brief LCD backlight brightness pin select function
 * @param backlight_pin : select LCD backlight brightness pin
 *                        pin name is: 0: ISINK
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_brightness_pin_sel(QL_LCD_BLSEL backlight_pin);

/**
 * @brief LCD backlight brightness adjust function
 * @param level : LCD backlight brightness level
 *                level is 0 to 5
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_set_brightness(uint32_t level);

/**
 * @brief LCD backlight brightness one channel of each level set function
 * @param level : LCD backlight brightness level
 *                level 0: means close backlight, so can't set
 *                set backlight brightness, from level 1 to level 5
 * @param level_brightness : 1. when pin is ISINK,
 *                              each level steps set, range is 0~63, one step is 0.84mA
 *                              one channel current: mix is 1.68mA, max is 54.6mA
 *                              there is 3 channels
 *                              total current is (1.68 + 0.84 * level_brightness) * 3 (mA)
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_set_level_brightness(uint32_t level, uint8_t level_brightness);

/**
 * @brief LCD enter and exit sleep function
 *        if the lcd has been initialized, the function must be called before enter sleep
 * @param is_sleep : true --lcd enter sleep 
 *                   false--lcd exit sleep 
 * @return
 *      QL_LCD_OPERATION_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_enter_sleep(bool is_sleep);

/**
 * @brief LCD display area set
 * @param start_x_offset : start pixel's x_offset (distance from (0,0) in x)
 * @param start_y_offset : start pixel's y_offset (distance from (0,0) in y)
 * @param width_offset   : width's x_offset (the diff between the supported width in driver and the width user want)
 * @param height_offset  : height's y_offset (the diff between the supported height in driver and the height user want)
 *                         when less than the supported width/height, offset is positive value;
 *                         when greater than the supported width/height, offset is negative value.
 * @return
 *      QL_LCD_EXECUTE_ERR
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_SUCCESS
 * @notice!!!
 *      The next configuration clears the last configured offset.
 */
ql_errcode_lcd ql_lcd_set_display_offset(uint16_t start_x_offset, uint16_t start_y_offset, int16_t width_offset, int16_t height_offset);

/**
 * @brief LCD busmode set function
 *            be used before LCD init!!!
 * @param bus_mode : Line4/Line3/Line3_2lane
 * @return
 *      QL_LCD_BUFFER_CREATE_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_set_busmode(ql_lcd_busmode_s bus_mode);

/**
 * @brief LCD screen cancel reset control
          LCD initialization will reset LCD by default. 
          This function can configure whether to skip reset during initialization.
 * @param lcd_reset : true  -- lcd reset
                      false -- lcd does not reset
 * @return
 *      QL_LCD_INVALID_PARAM_ERR
 *      QL_LCD_SUCCESS
 *
 * @attention
 *      This Function need to be used before LCD init
 */
ql_errcode_lcd ql_lcd_reset_config(bool lcd_reset);

/**
 * @brief get LCD driver info
 *            this API is used after initialization!!!
 * @param *info : driver info
 * @return
 *      QL_LCD_EXECUTE_ERR
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_get_info(ql_lcd_info_t *info);

/**
 * @brief Customer set LCD configure value
 *                   rotation/colorformat/widthoriginal/clean_screen/drv bus_mode/drv freq
 * @param *config :
 *                 rotation      -- 0:not rotating  1:rotate 90
 *                 widthoriginal -- Width of landscape screen
 *                 colorformat   -- QL_LCD_YUV422_UYVY  QL_LCD_RGB565
 *                 clean_screen  -- false: clean screen  true: keep screen
 *                 bus_mode      -- Line4/Line3/Line3_2lane
 *                 freq          -- 0 means use default driver frequence
 * @return
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_set_config(ql_lcd_config_t *config);

/**
 * @brief Customer get LCD configure value
 *                   rotation/colorformat/widthoriginal/clean_screen/drv bus_mode/drv freq
 *                   Notice: the getted value is Customer set value by ql_lcd_set_config!!!
                             if not set config, the value is default value!!!
 * @param *config :
 *                 rotation      -- default:0
 *                 widthoriginal -- default:0
 *                 colorformat   -- default:QL_LCD_RGB565
 *                 clean_screen  -- default:false
 *                 bus_mode      -- default:QL_LCD_SPI_LINE_4
 *                 freq          -- default:0 (0 means use default driver frequence)
 * @return
 *      QL_LCD_SUCCESS
 */
ql_errcode_lcd ql_lcd_get_config(ql_lcd_config_t *config);

#ifdef __cplusplus
} /*"C" */
#endif

#endif /* QL_LCD_H */


