# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

set(import_lib ${out_lib_dir}/libboot_ex_fat_rdonly.a)
configure_file(${SOURCE_TOP_DIR}/components/ql-kernel/boot_ex/libboot_ex_fat_rdonly.a ${import_lib} COPYONLY)
add_app_libraries(${import_lib})
add_library(${target} STATIC IMPORTED GLOBAL)
if (CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC)	
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include	
	IMPORTED_LINK_INTERFACE_LIBRARIES boot_ex_sdmmc
)
elseif (CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC)
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include	
	IMPORTED_LINK_INTERFACE_LIBRARIES boot_ex_emmc
)
elseif (CONFIG_QUEC_PROJECT_FEATURE_BOOT_SDMMC2)	
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include	
	IMPORTED_LINK_INTERFACE_LIBRARIES boot_ex_sdmmc2
)
elseif (CONFIG_QUEC_PROJECT_FEATURE_BOOT_EMMC2)	
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include	
	IMPORTED_LINK_INTERFACE_LIBRARIES boot_ex_emmc2
)
elseif (CONFIG_QUEC_PROJECT_FEATURE_BOOT_NAND_FLASH)	
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include	
	IMPORTED_LINK_INTERFACE_LIBRARIES boot_ex_nandflash
)
else()
set_target_properties(${target} PROPERTIES
    IMPORTED_LOCATION ${import_lib}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)
endif()

