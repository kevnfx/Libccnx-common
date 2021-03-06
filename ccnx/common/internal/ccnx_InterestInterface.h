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
 * @brief A structure of functions representing an Interest implementation.
 *
 * The underlying implementation should support the CCNxInterest API.
 *
 * @author Alan Walendowski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014-2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef CCNx_Common_ccnx_internal_InterestInterface_h
#define CCNx_Common_ccnx_internal_InterestInterface_h

#include <ccnx/common/internal/ccnx_TlvDictionary.h>

#include <ccnx/common/internal/ccnx_InterestPayloadIdMethod.h>
#include <ccnx/common/ccnx_InterestPayloadId.h>

typedef struct ccnx_interest_interface {
    /** A human-readable label for this implementation */
    char                 *description;

    /** @see ccnxInterest_Create */
    CCNxTlvDictionary *(*create)(const CCNxName * name,                  // required
                                 const uint32_t lifetimeMilliseconds,    // may use DefaultLimetimeMilliseconds
                                 const PARCBuffer * keyId,               // may be NULL
                                 const PARCBuffer * contentObjectHash,   // may be NULL
                                 const uint32_t hopLimit);

    /** @see ccnxInterest_CreateSimple */
    CCNxTlvDictionary *(*createSimple)(const CCNxName *name);

    /** @see ccnxInterest_GetName */
    CCNxName          *(*getName)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_SetLifetime */
    bool               (*setLifetime)(CCNxTlvDictionary *dict, uint32_t lifetime);

    /** @see ccnxInterest_GetLifetime */
    uint32_t           (*getLifetime)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_SetHopLimit */
    bool               (*setHopLimit)(CCNxTlvDictionary *dict, uint32_t hopLimit);

    /** @see ccnxInterest_GetHopLimit */
    uint32_t           (*getHopLimit)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_SetKeyIdRestriction */
    bool               (*setKeyIdRestriction)(CCNxTlvDictionary *dict, const PARCBuffer *keyId);

    /** @see ccnxInterest_GetKeyIdRestriction */
    PARCBuffer        *(*getKeyIdRestriction)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_SetPayload */
    bool               (*setPayload)(CCNxTlvDictionary *dict, const PARCBuffer *payload);

    /** @see ccnxInterest_SetPayloadAndId */
    bool               (*setPayloadAndId)(CCNxTlvDictionary *dict, const PARCBuffer *payload);

    /** @see ccnxInterest_SetPayloadWithId */
    bool               (*setPayloadWithId)(CCNxTlvDictionary *dict, const PARCBuffer *payload, const CCNxInterestPayloadId *id);

    /** @see ccnxInterest_GetPayload */
    PARCBuffer        *(*getPayload)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_SetContentObjectHashRestriction */
    bool               (*setContentObjectHashRestriction)(CCNxTlvDictionary *dict, const PARCBuffer *contentObjectHash);

    /** @see ccnxInterest_GetContentObjectHashRestriction */
    PARCBuffer        *(*getContentObjectHashRestriction)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_Equals */
    bool               (*equals)(const CCNxTlvDictionary *objectA, const CCNxTlvDictionary *objectB);
    /** @see ccnxInterest_AssertValid */
    void               (*assertValid)(const CCNxTlvDictionary *dict);

    /** @see ccnxInterest_ToString */
    char              *(*toString)(const CCNxTlvDictionary *dict);
    /** @see ccnxInterest_Display */
    void               (*display)(const CCNxTlvDictionary *interestDictionary, size_t indentation);
} CCNxInterestInterface;


/**
 * The SchemaV1 Interest implementaton
 */
extern CCNxInterestInterface CCNxInterestFacadeV1_Implementation;

/**
 * Given a CCNxTlvDictionary representing a CCNxInterest, return the address of the CCNxInterestInterface
 * instance that should be used to access the Interest. This will also update the CCNxTlvDictionary's interface
 * pointer for future references.
 *
 * @param interestDictionary - a {@link CCNxTlvDictionary} representing a CCNxInterest.
 * @return the address of the `CCNxContentObjectInterface` instance that should be used to access the CCNxInterest.
 *
 * Example:
 * @code
 * {
 *     CCNxName *name = ccnxName_CreateFromURI("lci:/boose/roo/pie");
 *
 *     CCNxInterest *interestV1 =
 *          ccnxInterest_CreateWithImpl(&CCNxInterestFacadeV1_Implementation,
 *                                      name,
 *                                      CCNxInterestDefault_LifetimeMilliseconds,
 *                                      NULL,
 *                                      NULL,
 *                                      CCNxInterestDefault_HopLimit);
 *
 *     assertTrue(ccnxInterestInterface_GetInterface(interestV1) == &CCNxInterestFacadeV1_Implementation,
 *                "Expected V1 Implementation");
 *
 *     ccnxName_Release(&name);
 *     ccnxInterest_Release(&interestV1);
 * } * @endcode
 */
CCNxInterestInterface *ccnxInterestInterface_GetInterface(const CCNxTlvDictionary *interestDictionary);
#endif
