/** @file
 * Shared Clipboard - Common header for the service extension.
 */

/*
 * Copyright (C) 2006-2024 Oracle and/or its affiliates.
 *
 * This file is part of VirtualBox base platform packages, as
 * available from https://www.virtualbox.org.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, in version 3 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses>.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL), a copy of it is provided in the "COPYING.CDDL" file included
 * in the VirtualBox distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 *
 * SPDX-License-Identifier: GPL-3.0-only OR CDDL-1.0
 */

#ifndef VBOX_INCLUDED_HostServices_VBoxClipboardExt_h
#define VBOX_INCLUDED_HostServices_VBoxClipboardExt_h
#ifndef RT_WITHOUT_PRAGMA_ONCE
# pragma once
#endif

#include <VBox/types.h>
#include <VBox/GuestHost/SharedClipboard.h>
#ifdef VBOX_WITH_SHARED_CLIPBOARD_TRANSFERS
# include <VBox/GuestHost/SharedClipboard-transfers.h>
#endif

/** Sets (or unsets) a clipboard extension callback. */
#define VBOX_CLIPBOARD_EXT_FN_SET_CALLBACK               (0)
/** The guest reports clipboard formats to the extension. */
#define VBOX_CLIPBOARD_EXT_FN_FORMAT_REPORT_TO_HOST      (1)
/** The clipboard service wants to report formats to the guest. */
#define VBOX_CLIPBOARD_EXT_FN_FORMAT_REPORT_TO_GUEST     (2)
/** The clipboard service requests clipboard data from the extension. */
#define VBOX_CLIPBOARD_EXT_FN_DATA_READ                  (3)
/** The clipboard service writes clipboard data to the extension. */
#define VBOX_CLIPBOARD_EXT_FN_DATA_WRITE                 (4)
/** The clipboard service announces an error to the extension. */
#define VBOX_CLIPBOARD_EXT_FN_ERROR                      (5)

typedef DECLCALLBACKTYPE(int, FNSHCLEXTCALLBACK,(uint32_t u32Function, uint32_t u32Format, void *pvData, uint32_t cbData));
typedef FNSHCLEXTCALLBACK *PFNSHCLEXTCALLBACK;

/**
 * Structure for holding Shared Clipboard service extension parameters.
 */
typedef struct _SHCLEXTPARMS
{
    union
    {
        /** Reports clipboard formats. */
        struct
        {
            SHCLFORMATS             uFormats;
        } ReportFormats;
        /** Reads / writes clipboard data. */
        struct
        {
            SHCLFORMAT              uFormat;
            void                   *pvData;
            uint32_t                cbData;
        } ReadWriteData;
        /** Sets a read / write callback. */
        struct
        {
            PFNSHCLEXTCALLBACK
                                    pfnCallback;
        } SetCallback;
        /** Reports a clipboard error. */
        struct
        {
            /** Clipboard ID. Optional and can be NULL. */
            char                   *pszId;
            /** User friendly error message. */
            char                   *pszMsg;
            /** IPRT-style error code. */
            int                     rc;
        } Error;
    } u;
} SHCLEXTPARMS;
/** Pointer to Shared Clipboard service extension parameters. */
typedef SHCLEXTPARMS *PSHCLEXTPARMS;

#endif /* !VBOX_INCLUDED_HostServices_VBoxClipboardExt_h */
