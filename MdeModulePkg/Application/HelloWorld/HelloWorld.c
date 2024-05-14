/** @file
  This sample application bases on HelloWorld PCD setting
  to print "UEFI Hello World!" to the UEFI Console.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#ifndef  MAGIC
  #define MAGIC(n) do {                             \
    int simics_magic_instr_dummy;                   \
    __asm__ __volatile__ ("cpuid"                   \
      : "=a" (simics_magic_instr_dummy)             \
      : "a" (0x4711 | ((unsigned)(n) << 16))        \
      : "ecx", "edx", "ebx");                       \
  } while (0)
  #define MAGIC_ENTER MAGIC(0); MAGIC(1);
  #define MAGIC_EXIT MAGIC(2); MAGIC(0);
#endif
//
// String token ID of help message text.
// Shell supports to find help message in the resource section of an application image if
// .MAN file is not found. This global variable is added to make build tool recognizes
// that the help string is consumed by user and then build tool will add the string into
// the resource section. Thus the application can use '-?' option to show help message in
// Shell.
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID  mStringHelpTokenId = STRING_TOKEN (STR_HELLO_WORLD_HELP_INFORMATION);
GLOBAL_REMOVE_IF_UNREFERENCED UINT8 *Data = NULL;

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  Print (L"Hello UEFI World\n");
  UINTN DataSize = 16;
  gBS->AllocatePool(EfiBootServicesData, DataSize, (VOID *) &Data);
  MAGIC(0);
  Print (L"Allocated pointer: 0x%p\n", Data);
  Print (L"Pointer to pointer: 0x%p\n", &Data);
  gBS->FreePool(Data);
  return EFI_SUCCESS;
}
