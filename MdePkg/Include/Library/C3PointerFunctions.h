/*
 * Copyright 2023 Intel Corporation
 * SPDX-License-Identifier: MIT
 */
#ifndef CC_POINTER_FUNCTIONS_H
#define CC_POINTER_FUNCTIONS_H

#ifndef _CC_GLOBALS_NO_INCLUDES_
#define _CC_GLOBALS_NO_INCLUDES_
#endif
#include <Library/DebugLib.h>
#include <Library/c3_edk2_cc_globals.h>
#include <Library/c3_edk2_cc_try_box.h>

#define is_encoded_address(addr) (is_encoded_cc_ptr((UINT64)((UINTN)addr)))
#define c3_dec_if_encoded_address(addr) ( \
  (VOID *) ((UINTN) cc_dec_if_encoded_ptr((UINT64)((UINTN)addr))))

#ifdef ENABLE_GLOBALS_ENCRYPTION
#define c3_dec_global_ptr(type, var) (                                         \
  (type *)((UINTN)c3_dec_if_encoded_address((UINT64)((UINTN)&var))))
#define c3_dec_global_val(type, var) (                                         \
  (type)(*(c3_dec_global_ptr(type, var))))
#else
#define c3_dec_global_ptr(type, var) (&var)
#define c3_dec_global_val(type, var) (var)
#endif

static inline UINT64 C3TryBoxAndEncode(UINT64 pointer, UINTN size) {
  ASSERT(sizeof(void *) == 8);
  ptr_metadata_t ptr_metadata = {0};

  if(try_box(pointer, size, &ptr_metadata)) {
    return cc_isa_encptr(pointer, &ptr_metadata);
  }
  return 0;
}

static inline void clear_icv_and_zero(void *ptr, UINT64 size) {
    cc_set_icv_lock(0);
    // TODO(hliljest): Use proper memset functionality
    for (void *end = (ptr + size); ptr < end; ptr += sizeof(char)) {
      *((char *)ptr) = 0;
    }
    cc_set_icv_lock(1);
}

static inline void clear_icv(void *ptr, UINT64 size) {
    cc_set_icv_lock(0);
    for (void *end = (ptr + size); ptr < end; ptr += sizeof(char)) {
      __asm__ volatile (
        "mov (%[ptr]), %%al  \n"
        "mov %%al, (%[ptr])  \n"
        : : [ptr] "r" (ptr) : "memory", "rax"
        );
    }
    cc_set_icv_lock(1);
}

static inline void *revert_ca_alloc(void *ca, UINT64 size) {
    ASSERT(sizeof(void *) == 8);
    // ASSERT(is_encoded_address(ca)); // Breaks --disable_ptrenc
    void *const la_base = (void *)((UINTN)cc_isa_decptr((UINTN)ca));
    void *const la_end = la_base + size;

    cc_set_icv_lock(0);
    // Decrypt and clear ICV for each byte
    void *la = la_base;
    while (la < la_end) {
        __asm__ volatile (
            "mov (%[ca]), %%al  \n"
            "mov %%al, (%[la])  \n"
            : [la] "+r" (la), [ca] "+r" (ca) : : "memory", "rax");
        la += sizeof(char);
        ca += sizeof(char);
    }
    cc_set_icv_lock(1);
    DEBUG((DEBUG_INFO, "[C3_HEAP]: Allocation reverted %16lx\n", la_base));

    // Return the decoded input CA
    return la_base;
}

static inline BOOLEAN c3_alloc_shim(VOID **ptr, UINTN Size) {
    ASSERT(sizeof(void *) == 8);
    // ASSERT(!is_encoded_address(*ptr)); // Breaks --disable_ptrenc
    UINT64 Buffer = (UINT64)((UINTN)*ptr);
    if (Buffer != 0) {
      ptr_metadata_t ptr_metadata = {0};
      if(try_box(Buffer, Size, &ptr_metadata)) {
        const UINT64 old = Buffer;
        (void) old;  // maybe_unused
        Buffer = cc_isa_encptr(Buffer, &ptr_metadata);
#ifdef C3_LOG_ALL_HEAP_ALLOCATIONS
        DEBUG((DEBUG_INFO,
               "[C3_HEAP]: Allocation encoded %016lx %lu to %016lx\n",
               old, Size, Buffer));
#endif  // C3_LOG_ALL_HEAP_ALLOCATIONS
        *ptr = (VOID*)((UINTN)Buffer);
        clear_icv_and_zero(*ptr, Size);
        return 1;
      }
      DEBUG((DEBUG_INFO,
             "[C3_HEAP]: Allocation try_box failed %16lx %lu\n",
             Buffer, Size));
    }
    return 0;
}

#endif
