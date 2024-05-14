/*
 * Copyright 2023 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#ifndef C3DEFINES_H
#define C3DEFINES_H

#define ENABLE_HEAP_ENCRYPTION
#define ENABLE_GLOBALS_ENCRYPTION

#ifdef ENABLE_GLOBALS_ENCRYPTION
#define C3_ENABLE_SMM_GLOBALS_ENCRYPTION
#define CC_GLOBALS_ENC_ENTRY
#define CC_GLOBALS_ENC_DATA
#endif  // ENABLE_GLOBALS_ENCRYPTION

#ifdef ENABLE_HEAP_ENCRYPTION
// Opt-in for C3 protection of Lockbox at
// MdeModulePkg/Library/SmmLockBoxLib/SmmLockBoxMmLib.c.
#define C3_HEAP_OPTIN_LOCKBOX

// Enable SMM Memory pool allocator C3 hardening (wip)
#define C3_SMM_POOL_ALLOCATOR

#endif  // ENABLE_HEAP_ENCRYPTION

#ifndef  MAGIC
  #define MAGIC(n) do {                             \
    int simics_magic_instr_dummy;                   \
    __asm__ __volatile__ ("cpuid"                   \
      : "=a" (simics_magic_instr_dummy)             \
      : "a" (0x4711 | ((unsigned)(n) << 16))        \
      : "ecx", "edx", "ebx");                       \
  } while (0)
#endif

#ifndef  MAGIC_SEND
  #define MAGIC_SEND(n,v) do {                      \
    int simics_magic_instr_dummy;                   \
    __asm__ __volatile__ ("cpuid"                   \
      : "=a" (simics_magic_instr_dummy)             \
      : "a"  (0x4711 | ((unsigned)(n) << 16)),      \
        "D" (v)                                     \
      : "ecx", "edx", "ebx");                       \
  } while (0)
#endif

#ifndef MAGIC_SEND2
#define MAGIC_SEND2(n,v1, v2) do {                  \
    int simics_magic_instr_dummy;                   \
    __asm__ __volatile__ ("cpuid"                   \
      : "=a" (simics_magic_instr_dummy)             \
      : "a"  (0x4711 | ((unsigned)(n) << 16)),      \
        "S" ((UINT64)(v1)),                         \
        "D" ((UINT64)(v2))                          \
      : "ecx", "edx", "ebx");                       \
} while(0)
#endif

#ifndef DEBUG_MAGIC_SEND
#define DEBUG_MAGIC_SEND(n,v) do {                          \
  DEBUG((DEBUG_INFO, "[C3_HACK]: " #v " at 0x%lx\n", v));   \
  MAGIC_SEND(n,v);                                          \
} while(0)
#endif


#define MAGIC_MODULE_LOAD() MAGIC(2)

#endif
