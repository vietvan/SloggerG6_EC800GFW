/*
 * Copyright (C) 2015-2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <stdint.h>
#include <stdio.h>

#define LOCAL_TRACE 0

#define LOG_TAG "AT_SERVICE"

#define ATLOGE(fmt, ...) \
    fprintf(stderr, "[%s]: (%s %d): " fmt, LOG_TAG, __func__, __LINE__, ##__VA_ARGS__)

#define ATLOGE_APP_NAME(fmt, ...)                                       \
    fprintf(stderr, "%s: %d: %s: " fmt, LOG_TAG, __LINE__, __progname, \
            ##__VA_ARGS__)

#if LOCAL_TRACE
#define ATLOGI(fmt, ...) \
    fprintf(stderr, "%s: %d: " fmt, LOG_TAG, __LINE__, ##__VA_ARGS__)
#else
#define ATLOGI(fmt, ...)
#endif
