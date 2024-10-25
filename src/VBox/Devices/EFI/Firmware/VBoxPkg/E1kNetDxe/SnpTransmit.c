/** @file

  Implementation of the SNP.Transmit() function and its private helpers if any.

  Copyright (c) 2021 - 2024, Oracle and/or its affiliates.
  Copyright (C) 2013, Red Hat, Inc.
  Copyright (c) 2006 - 2013, Intel Corporation. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "E1kNet.h"

/**
  Places a packet in the transmit queue of a network interface.

  @param  This       The protocol instance pointer.
  @param  HeaderSize The size, in bytes, of the media header to be filled in by
                     the Transmit() function. If HeaderSize is non-zero, then
                     it must be equal to This->Mode->MediaHeaderSize and the
                     DestAddr and Protocol parameters must not be NULL.
  @param  BufferSize The size, in bytes, of the entire packet (media header and
                     data) to be transmitted through the network interface.
  @param  Buffer     A pointer to the packet (media header followed by data) to
                     be transmitted. This parameter cannot be NULL. If
                     HeaderSize is zero, then the media header in Buffer must
                     already be filled in by the caller. If HeaderSize is
                     non-zero, then the media header will be filled in by the
                     Transmit() function.
  @param  SrcAddr    The source HW MAC address. If HeaderSize is zero, then
                     this parameter is ignored. If HeaderSize is non-zero and
                     SrcAddr is NULL, then This->Mode->CurrentAddress is used
                     for the source HW MAC address.
  @param  DestAddr   The destination HW MAC address. If HeaderSize is zero,
                     then this parameter is ignored.
  @param  Protocol   The type of header to build. If HeaderSize is zero, then
                     this parameter is ignored. See RFC 1700, section "Ether
                     Types", for examples.

  @retval EFI_SUCCESS           The packet was placed on the transmit queue.
  @retval EFI_NOT_STARTED       The network interface has not been started.
  @retval EFI_NOT_READY         The network interface is too busy to accept
                                this transmit request.
  @retval EFI_BUFFER_TOO_SMALL  The BufferSize parameter is too small.
  @retval EFI_INVALID_PARAMETER One or more of the parameters has an
                                unsupported value.
  @retval EFI_DEVICE_ERROR      The command could not be sent to the network
                                interface.
  @retval EFI_UNSUPPORTED       This function is not supported by the network
                                interface.

**/

EFI_STATUS
EFIAPI
E1kNetTransmit (
  IN EFI_SIMPLE_NETWORK_PROTOCOL *This,
  IN UINTN                       HeaderSize,
  IN UINTN                       BufferSize,
  IN /* +OUT! */ VOID            *Buffer,
  IN EFI_MAC_ADDRESS             *SrcAddr  OPTIONAL,
  IN EFI_MAC_ADDRESS             *DestAddr OPTIONAL,
  IN UINT16                      *Protocol OPTIONAL
  )
{
  E1K_NET_DEV           *Dev;
  EFI_TPL               OldTpl;
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  DeviceAddress;

  DEBUG((DEBUG_INFO, "E1kNetTransmit: HeaderSize=%u BufferSize=%u Buffer=%p\n",
         HeaderSize, BufferSize, Buffer));

  if (This == NULL || BufferSize == 0 || Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = E1K_NET_FROM_SNP (This);
  OldTpl = gBS->RaiseTPL (TPL_CALLBACK);
  switch (Dev->Snm.State) {
  case EfiSimpleNetworkStopped:
    Status = EFI_NOT_STARTED;
    goto Exit;
  case EfiSimpleNetworkStarted:
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  default:
    break;
  }

  if (BufferSize < Dev->Snm.MediaHeaderSize) {
    Status = EFI_BUFFER_TOO_SMALL;
    goto Exit;
  }
  if (BufferSize > Dev->Snm.MediaHeaderSize + Dev->Snm.MaxPacketSize) {
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // check if we have room for transmission
  //
  ASSERT (Dev->TxCurPending <= Dev->TxMaxPending);
  if (Dev->TxCurPending == Dev->TxMaxPending) {
    Status = EFI_NOT_READY;
    goto Exit;
  }

  //
  // the caller may want us to fill in the media header:
  // dst MAC, src MAC, Ethertype
  //
  if (HeaderSize != 0) {
    UINT8 *Ptr;

    if (HeaderSize != Dev->Snm.MediaHeaderSize ||
        DestAddr == NULL || Protocol == NULL) {
      Status = EFI_INVALID_PARAMETER;
      goto Exit;
    }
    Ptr = Buffer;
    ASSERT (sizeof (E1K_NET_MAC) <= sizeof (EFI_MAC_ADDRESS));

    CopyMem (Ptr, DestAddr, sizeof (E1K_NET_MAC));
    Ptr += sizeof (E1K_NET_MAC);

    CopyMem (Ptr,
      (SrcAddr == NULL) ? &Dev->Snm.CurrentAddress : SrcAddr,
      sizeof (E1K_NET_MAC));
    Ptr += sizeof (E1K_NET_MAC);

    *Ptr++ = (UINT8) (*Protocol >> 8);
    *Ptr++ = (UINT8) *Protocol;

    ASSERT ((UINTN) (Ptr - (UINT8 *) Buffer) == Dev->Snm.MediaHeaderSize);
  }

  //
  // Map the transmit buffer system physical address to device address.
  //
  Status = E1kNetMapTxBuf (
             Dev,
             Buffer,
             BufferSize,
             &DeviceAddress
             );
  if (EFI_ERROR (Status)) {
    Status = EFI_DEVICE_ERROR;
    goto Exit;
  }

  Dev->TxCurPending++;
  Dev->TxRing[Dev->TxLastUsed].AddrBufferLow  = (UINT32)DeviceAddress;
  Dev->TxRing[Dev->TxLastUsed].AddrBufferHigh = (UINT32)RShiftU64(DeviceAddress, 32);
  Dev->TxRing[Dev->TxLastUsed].BufferLength   = (UINT16)BufferSize;
  Dev->TxRing[Dev->TxLastUsed].Status         = 0;
  Dev->TxRing[Dev->TxLastUsed].Command        = E1K_TX_CMD_EOP | E1K_TX_CMD_FCS | E1K_TX_CMD_RS;

  Dev->TxLastUsed = (Dev->TxLastUsed + 1) % E1K_NET_MAX_PENDING;
  E1kNetRegWrite32(Dev, E1K_REG_TDT, Dev->TxLastUsed);

Exit:
  gBS->RestoreTPL (OldTpl);
  return Status;
}
