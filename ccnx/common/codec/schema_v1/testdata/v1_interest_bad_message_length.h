/*
 * Copyright (c) 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @file v1_interest_all_fields.h
 * @brief A hand-encoded v1 interest in wireformat with all Interest fields.
 *
 * The Interest TLV length goes beyond the end of the packet
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef CCNx_Common_v1_interest_bad_message_length_h
#define CCNx_Common_v1_interest_bad_message_length_h

#include <ccnx/common/codec/testdata/testdata_common.h>
#include <ccnx/common/codec/schema_v1/testdata/v1_InterestSchema.h>

/**
 * A well formed interest with all allowed Interest fields
 */
__attribute__((unused))
static uint8_t v1_interest_bad_message_length[] = {
    0x01, 0x00, 0x00,   30,     // ver = 1, type = interest, length = 30
    0x20, 0x00, 0x11,   14,     // HopLimit = 31, reserved = 0, flags = 0x11, header length = 14
    // ------------------------
    0x00, 0x01, 0x00,    2,     // Interest Lifetime (2 bytes)
    0xEA, 0xEB,
    // ------------------------
    0x00, 0x01, 0x00,   13,     // type = interest, length = 13 (1 byte too far)
    // ------------------------
    0x00, 0x00, 0x00,    8,     // type = name, length = 8
    0x00, 0x02, 0x00,    4,     // type = binary, length = 4
    'c',  'o',  'o',   'l',     // "cool"
};

__attribute__((unused))
static TruthTableEntry TRUTHTABLENAME(v1_interest_bad_message_length)[] = {
    { .wellKnownType = false, .indexOrKey = T_INVALID,             .extent = { 0,  0} },
};

#define v1_interest_bad_message_length_truthTable TABLEENTRY(v1_interest_bad_message_length, TLV_ERR_TOO_LONG)

#endif
