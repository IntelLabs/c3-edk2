/*
 * Copyright 2023 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#ifndef C3_EDK2_TRY_BOX_H
#define C3_EDK2_TRY_BOX_H

#ifndef _CC_GLOBALS_NO_INCLUDES_
#define _CC_GLOBALS_NO_INCLUDES_
#endif
#include <Base.h>
#define uint8_t UINT8
#define uint16_t UINT16
#define uint32_t UINT32
#define uint64_t UINT64
#define uintptr_t UINTN
#define size_t UINT64
#if __has_include("../../../../../malloc/try_box.h")
#include "../../../../../malloc/try_box.h"
#endif
#undef uint8_t
#undef uint16_t
#undef uint32_t
#undef uint64_t
#undef uintptr_t
#undef size_t

#endif // C3_EDK2_TRY_BOX_H