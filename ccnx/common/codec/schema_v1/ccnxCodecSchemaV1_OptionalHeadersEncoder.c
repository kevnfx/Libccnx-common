/*
 * Copyright (c) 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_Buffer.h>

#include <ccnx/common/codec/schema_v1/ccnxCodecSchemaV1_OptionalHeadersEncoder.h>
#include <ccnx/common/codec/schema_v1/ccnxCodecSchemaV1_Types.h>
#include <ccnx/common/codec/ccnxCodec_TlvUtilities.h>

static ssize_t
_EncodeInterestLifetime(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;

    // allow either encoding as an Integer or as a Buffer

    if (ccnxTlvDictionary_IsValueInteger(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_InterestLifetime)) {
        uint64_t lifetime = ccnxTlvDictionary_GetInteger(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_InterestLifetime);
        length = ccnxCodecTlvEncoder_AppendVarInt(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_InterestLifetime, lifetime);
    } else if (ccnxTlvDictionary_IsValueBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_InterestLifetime)) {
        PARCBuffer *lifetime = ccnxTlvDictionary_GetBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_InterestLifetime);
        length = ccnxCodecTlvEncoder_AppendBuffer(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_InterestLifetime, lifetime);
    }

    return length;
}

static ssize_t
_EncodeRecommendedCacheTime(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;

    // allow either encoding as an Integer or as a Buffer

    if (ccnxTlvDictionary_IsValueInteger(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_RecommendedCacheTime)) {
        uint64_t cacheTime = ccnxTlvDictionary_GetInteger(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_RecommendedCacheTime);
        length = ccnxCodecTlvEncoder_AppendVarInt(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_RecommendedCacheTime, cacheTime);
    } else if (ccnxTlvDictionary_IsValueBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_RecommendedCacheTime)) {
        PARCBuffer *cacheTime = ccnxTlvDictionary_GetBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_RecommendedCacheTime);
        length = ccnxCodecTlvEncoder_AppendBuffer(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_RecommendedCacheTime, cacheTime);
    }

    return length;
}

static ssize_t
_EncodeInterestFrag(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;
    PARCBuffer *buffer = ccnxTlvDictionary_GetBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_INTFRAG);
    if (buffer != NULL) {
        length = ccnxCodecTlvEncoder_AppendBuffer(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_InterestFragment, buffer);
    }
    return length;
}

static ssize_t
_EncodeContentObjectFrag(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;
    PARCBuffer *buffer = ccnxTlvDictionary_GetBuffer(packetDictionary, CCNxCodecSchemaV1TlvDictionary_HeadersFastArray_OBJFRAG);
    if (buffer != NULL) {
        length = ccnxCodecTlvEncoder_AppendBuffer(optionalHeadersEncoder, CCNxCodecSchemaV1Types_OptionalHeaders_ContentObjectFragment, buffer);
    }
    return length;
}

static ssize_t
_EncodeInterestHeaders(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;
    ssize_t intFragLength = _EncodeInterestFrag(optionalHeadersEncoder, packetDictionary);
    if (intFragLength < 0) {
        return intFragLength;
    }
    length += intFragLength;

    ssize_t intLifeLength = _EncodeInterestLifetime(optionalHeadersEncoder, packetDictionary);
    if (intLifeLength < 0) {
        return intLifeLength;
    }
    length += intLifeLength;

    ssize_t customLength = ccnxCodecTlvUtilities_EncodeCustomList(optionalHeadersEncoder, packetDictionary, CCNxCodecSchemaV1TlvDictionary_Lists_HEADERS);
    if (customLength < 0) {
        return customLength;
    }
    length += customLength;

    return length;
}

static ssize_t
_EncodeContentObjectHeaders(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    ssize_t length = 0;
    ssize_t result;

    result = _EncodeContentObjectFrag(optionalHeadersEncoder, packetDictionary);
    if (result < 0) {
        return result;
    }
    length += result;

    result = _EncodeRecommendedCacheTime(optionalHeadersEncoder, packetDictionary);
    if (result < 0) {
        return result;
    }
    length += result;

    result = ccnxCodecTlvUtilities_EncodeCustomList(optionalHeadersEncoder, packetDictionary, CCNxCodecSchemaV1TlvDictionary_Lists_HEADERS);
    if (result < 0) {
        return result;
    }
    length += result;

    return length;
}


ssize_t
ccnxCodecSchemaV1OptionalHeadersEncoder_Encode(CCNxCodecTlvEncoder *optionalHeadersEncoder, CCNxTlvDictionary *packetDictionary)
{
    assertNotNull(optionalHeadersEncoder, "Parameter optionalHeadersEncoder must be non-null");
    assertNotNull(packetDictionary, "Parameter packetDictionary must be non-null");

    ssize_t result = 0;
    if (ccnxTlvDictionary_IsInterest(packetDictionary) || ccnxTlvDictionary_IsInterestReturn(packetDictionary)) {
        result = _EncodeInterestHeaders(optionalHeadersEncoder, packetDictionary);
    } else if (ccnxTlvDictionary_IsContentObject(packetDictionary)) {
        result = _EncodeContentObjectHeaders(optionalHeadersEncoder, packetDictionary);
    } else if (ccnxTlvDictionary_IsControl(packetDictionary)) {
        result = ccnxCodecTlvUtilities_EncodeCustomList(optionalHeadersEncoder, packetDictionary, CCNxCodecSchemaV1TlvDictionary_Lists_HEADERS);
    } else {
        CCNxCodecError *error = ccnxCodecError_Create(TLV_ERR_PACKETTYPE, __func__, __LINE__, ccnxCodecTlvEncoder_Position(optionalHeadersEncoder));
        ccnxCodecTlvEncoder_SetError(optionalHeadersEncoder, error);
        ccnxCodecError_Release(&error);
        result = -1;
    }

    return result;
}
