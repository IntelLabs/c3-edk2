/*
 * Copyright 2023 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#ifndef C3_GLOBALS_UTILS_H_
#define C3_GLOBALS_UTILS_H_

#include <Library/C3Defines.h>
#include <Library/C3PointerFunctions.h>
#include <Library/C3ConstantSections.h>

typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} EFI_GUID_COPY;

typedef struct C3ModuleInfo {
  EFI_GUID_COPY   DriverGUID;
  INT32           Enabled;
  UINTN           StartOffset;
  UINTN           EndOffset;
} C3_MODULE_INFO;

static inline UINT32 ModuleNameMatch(const CHAR8* Name1, const CHAR8* Name2) {
    for (UINT32 index = 0; index < 256; index++){
        if (Name1[index] != Name2[index]) return 0;
        if (Name1[index] == 0) return 1;
    }
    return 0; // Name longer than 256
}

static inline INT32 FindC3EnabledModule(CHAR8 FileName[256]) {
  for (UINT32 i = 0; i < NUMBER_OF_MODULES; i++) {
    if (ModuleNameMatch(ModuleNames[i], FileName)) {
      return i;
    }
  }
  return -1;
}

static inline INT32
C3IsEqualGuid(
  const EFI_GUID_COPY *A,
  const EFI_GUID_COPY *B
  )
{
  if (A->Data1 != B->Data1 ||A->Data2 != B->Data2 || A->Data3 != B->Data3) {
    return 0;
  }
  
  if (A->Data4[0] != B->Data4[0] || A->Data4[1] != B->Data4[1] ||
      A->Data4[2] != B->Data4[2] || A->Data4[3] != B->Data4[3] ||
      A->Data4[4] != B->Data4[4] || A->Data4[5] != B->Data4[5] ||
      A->Data4[6] != B->Data4[6] || A->Data4[7] != B->Data4[7]) {
    return 0;
  }

  return 1;
}

#endif  // 83_GLOBALS_UTILS_H_