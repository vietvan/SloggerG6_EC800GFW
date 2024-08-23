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

configure_file(include/vtrust_config.h.in ${out_inc_dir}/vtrust_config.h)

target_sources(
	${target}
	PRIVATE
	test/vtrust_tee_test.c
	test/command.c
	src/vtrust_tee_driver.c
	src/vtrust_at_client.c
)

target_include_directories(${target} PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/include/interface/at_client
)

#target_include_targets(${target} PRIVATE kernel driver lwip http)
target_include_targets(${target} PRIVATE kernel driver lwip)

target_include_targets(${target} PRIVATE
	vtrust_tee_core
)

include(core.cmake)


target_include_targets(${target} PRIVATE
	at_crypt
)
include(at_crypt.cmake)
#relative_glob(srcs test/*.c include/*.h)
#beautify_c_code(${target} ${srcs})
