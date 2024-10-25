/********************************************************************************/
/*										*/
/*			    Build Switches	 				*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*            $Id: TpmBuildSwitches.h $	*/
/*										*/
/*  Licenses and Notices							*/
/*										*/
/*  1. Copyright Licenses:							*/
/*										*/
/*  - Trusted Computing Group (TCG) grants to the user of the source code in	*/
/*    this specification (the "Source Code") a worldwide, irrevocable, 		*/
/*    nonexclusive, royalty free, copyright license to reproduce, create 	*/
/*    derivative works, distribute, display and perform the Source Code and	*/
/*    derivative works thereof, and to grant others the rights granted herein.	*/
/*										*/
/*  - The TCG grants to the user of the other parts of the specification 	*/
/*    (other than the Source Code) the rights to reproduce, distribute, 	*/
/*    display, and perform the specification solely for the purpose of 		*/
/*    developing products based on such documents.				*/
/*										*/
/*  2. Source Code Distribution Conditions:					*/
/*										*/
/*  - Redistributions of Source Code must retain the above copyright licenses, 	*/
/*    this list of conditions and the following disclaimers.			*/
/*										*/
/*  - Redistributions in binary form must reproduce the above copyright 	*/
/*    licenses, this list of conditions	and the following disclaimers in the 	*/
/*    documentation and/or other materials provided with the distribution.	*/
/*										*/
/*  3. Disclaimers:								*/
/*										*/
/*  - THE COPYRIGHT LICENSES SET FORTH ABOVE DO NOT REPRESENT ANY FORM OF	*/
/*  LICENSE OR WAIVER, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, WITH	*/
/*  RESPECT TO PATENT RIGHTS HELD BY TCG MEMBERS (OR OTHER THIRD PARTIES)	*/
/*  THAT MAY BE NECESSARY TO IMPLEMENT THIS SPECIFICATION OR OTHERWISE.		*/
/*  Contact TCG Administration (admin@trustedcomputinggroup.org) for 		*/
/*  information on specification licensing rights available through TCG 	*/
/*  membership agreements.							*/
/*										*/
/*  - THIS SPECIFICATION IS PROVIDED "AS IS" WITH NO EXPRESS OR IMPLIED 	*/
/*    WARRANTIES WHATSOEVER, INCLUDING ANY WARRANTY OF MERCHANTABILITY OR 	*/
/*    FITNESS FOR A PARTICULAR PURPOSE, ACCURACY, COMPLETENESS, OR 		*/
/*    NONINFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS, OR ANY WARRANTY 		*/
/*    OTHERWISE ARISING OUT OF ANY PROPOSAL, SPECIFICATION OR SAMPLE.		*/
/*										*/
/*  - Without limitation, TCG and its members and licensors disclaim all 	*/
/*    liability, including liability for infringement of any proprietary 	*/
/*    rights, relating to use of information in this specification and to the	*/
/*    implementation of this specification, and TCG disclaims all liability for	*/
/*    cost of procurement of substitute goods or services, lost profits, loss 	*/
/*    of use, loss of data or any incidental, consequential, direct, indirect, 	*/
/*    or special damages, whether under contract, tort, warranty or otherwise, 	*/
/*    arising in any way out of use or reliance upon this specification or any 	*/
/*    information herein.							*/
/*										*/
/*  (c) Copyright IBM Corp. and others, 2016 - 2020				*/
/*										*/
/********************************************************************************/

/* 5.19	TpmBuildSwitches.h */

/* This file contains the build switches. This contains switches for multiple versions of the
   crypto-library so some may not apply to your environment. */
/* The switches are guarded so that they can either be set on the command line or set here. If the
   switch is listed on the command line (-DSOME_SWITCH) with no setting, then the switch will be set
   to YES. If the switch setting is not on the command line or if the setting is other than YES or
   NO, then the switch will be set to the default value. The default can either be YES or NO as
   indicated on each line where the default is selected. */
/* A caution. Do not try to test these macros by inserting #defines in this file. For some curious
   reason, a variable set on the command line with no setting will have a value of 1. An #if
   SOME_VARIABLE will work if the variable is not defined or is defined on the command line with no
   initial setting. However, a #define SOME_VARIABLE is a null string and when used in #if
   SOME_VARIABLE will not be a proper expression. If you want to test various switches, either use
   the command line or change the default. */

#ifndef TPMBUILDSWITCHES_H
#define TPMBUILDSWITCHES_H

#undef YES
#define YES 1
#undef NO
#define NO 0

/* Allow the command line to specify a profile file */

#ifdef PROFILE
#   define PROFILE_QUOTE(a) #a
#   define PROFILE_INCLUDE(a) PROFILE_QUOTE(a)
#   include PROFILE_INCLUDE(PROFILE)
#endif

// Need an unambiguous definition for DEBUG. Don't change this
#ifndef DEBUG
#   ifdef NDEBUG
#       define  DEBUG   NO
#   else
#       define  DEBUG   YES
#   endif
#elif (DEBUG != NO) && (DEBUG != YES)
#   undef   DEBUG
#   define  DEBUG       YES       // Default: Either YES or NO
#endif
#include "CompilerDependencies.h"

// This definition is required for the re-factored code
#if (!defined USE_BN_ECC_DATA) || ((USE_BN_ECC_DATA != NO) && (USE_BN_ECC_DATA != YES))
#   undef   USE_BN_ECC_DATA
#   define  USE_BN_ECC_DATA     YES     // Default: Either YES or NO
#endif

/* The SIMULATION switch allows certain other macros to be enabled. The things that can be enabled
   in a simulation include key caching, reproducible random sequences, instrumentation of the RSA
   key generation process, and certain other debug code. SIMULATION Needs to be defined as either
   YES or NO. This grouping of macros will make sure that it is set correctly. A simulated TPM would
   include a Virtual TPM. The interfaces for a Virtual TPM should be modified from the standard ones
   in the Simulator project. If SIMULATION is in the compile parameters without modifiers, make
   SIMULATION == YES */
#if !(defined SIMULATION) || ((SIMULATION != NO) && (SIMULATION != YES))
#   undef   SIMULATION
#   define  SIMULATION      NO     // Default: Either YES or NO    libtpms: NO
#endif

// Define this to run the function that checks the compatibility between the chosen big number math
// library and the TPM code. Not all ports use this.

#if !(defined LIBRARY_COMPATIBILITY_CHECK)				\
    || (( LIBRARY_COMPATIBILITY_CHECK != NO)				\
	&& (LIBRARY_COMPATIBILITY_CHECK != YES))
#   undef   LIBRARY_COMPATIBILITY_CHECK
#   define  LIBRARY_COMPATIBILITY_CHECK YES     // Default: Either YES or NO  libtpms: YES
#endif
#if !(defined FIPS_COMPLIANT) || ((FIPS_COMPLIANT != NO) && (FIPS_COMPLIANT != YES))
#   undef   FIPS_COMPLIANT
#   define  FIPS_COMPLIANT      NO     // Default: Either YES or NO    libtpms: NO
#endif

// Definition to allow alternate behavior for non-orderly startup. If there is a chance that the TPM
// could not update failedTries

/* Removes the behavior of automatically incrementing the failed tries counter after any non-orderly
   shutdown.  When YES, the failed counter is incremented on non-orderly shutdown only if an attempt
   to access a DA protected object was made on the previous cycle. */

#if !(defined USE_DA_USED) || ((USE_DA_USED != NO) && (USE_DA_USED != YES))
#   undef   USE_DA_USED
#   define  USE_DA_USED     YES         // Default: Either YES or NO
#endif

// Define TABLE_DRIVEN_DISPATCH to use tables rather than case statements for command dispatch and
// handle unmarshaling
#if !(defined TABLE_DRIVEN_DISPATCH)					\
    || ((TABLE_DRIVEN_DISPATCH != NO) && (TABLE_DRIVEN_DISPATCH != YES))
#   undef   TABLE_DRIVEN_DISPATCH
#   define  TABLE_DRIVEN_DISPATCH   YES     // Default: Either YES or NO
#endif

/* This switch is used to enable the self-test capability in AlgorithmTests.c */
#if !(defined SELF_TEST) || ((SELF_TEST != NO) && (SELF_TEST != YES))
#   undef   SELF_TEST
#   define  SELF_TEST       YES         // Default: Either YES or NO
#endif

/* Enable the generation of RSA primes using a sieve. */
#if !(defined RSA_KEY_SIEVE) || ((RSA_KEY_SIEVE != NO) && (RSA_KEY_SIEVE != YES))
#   undef   RSA_KEY_SIEVE
#   define  RSA_KEY_SIEVE           YES         // Default: Either YES or NO
#endif

/* Enable the instrumentation of the sieve process. This is used to tune the sieve variables.*/
#if RSA_KEY_SIEVE && SIMULATION
#   if !(defined RSA_INSTRUMENT) || ((RSA_INSTRUMENT != NO) && (RSA_INSTRUMENT != YES))
#       undef   RSA_INSTRUMENT
#       define  RSA_INSTRUMENT      NO         // Default: Either YES or NO
#   endif
#endif

/* This switch enables the RNG state save and restore */
#if !(defined _DRBG_STATE_SAVE)						\
    || ((_DRBG_STATE_SAVE != NO) && (_DRBG_STATE_SAVE != YES))
#   undef   _DRBG_STATE_SAVE
#   define  _DRBG_STATE_SAVE        YES     // Default: Either YES or NO
#endif

/* Switch added to support packed lists that leave out space associated with unimplemented
   commands. Comment this out to use linear lists. */
/* NOTE: if vendor specific commands are present, the associated list is always in compressed
   form. */
#if !(defined COMPRESSED_LISTS)						\
    || ((COMPRESSED_LISTS != NO) && (COMPRESSED_LISTS != YES))
#   undef   COMPRESSED_LISTS
#   define  COMPRESSED_LISTS        YES     // Default: Either YES or NO
#endif


/* This switch indicates where clock epoch value should be stored. If this value defined, then it is
   assumed that the timer will change at any time so the nonce should be a random number kept in
   RAM. When it is not defined, then the timer only stops during power outages. */
#if !(defined CLOCK_STOPS) || ((CLOCK_STOPS != NO) && (CLOCK_STOPS != YES))
#   undef   CLOCK_STOPS
#   define  CLOCK_STOPS             NO     // Default: Either YES or NO
#endif

// This switch allows use of #defines in place of pass-through marshaling or unmarshaling code. A
// pass-through function just calls another function to do the required function and does no
// parameter checking of its own. The table-driven dispatcher calls directly to the lowest level
// marshaling/unmarshaling code and by-passes any pass-through functions.
#if (defined USE_MARSHALING_DEFINES) && (USE_MARSHALING_DEFINES != NO)
#   undef   USE_MARSHALING_DEFINES
#   define  USE_MARSHALING_DEFINES  YES
#else
#   define USE_MARSHALING_DEFINES   YES     // Default: Either YES or NO
#endif

// The switches in this group can only be enabled when doing debug during simulation
#if SIMULATION && DEBUG

/* This forces the use of a smaller context slot size. This reduction reduces the range of the epoch
   allowing the tester to force the epoch to occur faster than the normal defined in TpmProfile.h */
#   if !(defined CONTEXT_SLOT)
#       define CONTEXT_SLOT             UINT8
#   endif

// Enables use of the key cache. Default is YES
#   if !(defined USE_RSA_KEY_CACHE)					\
    || ((USE_RSA_KEY_CACHE != NO) && (USE_RSA_KEY_CACHE != YES))
#       undef   USE_RSA_KEY_CACHE
#       define  USE_RSA_KEY_CACHE   YES   // Default: Either YES or NO
#   endif

// Enables use of a file to store the key cache values so that the TPM will start faster during
// debug. Default for this is YES
#   if USE_RSA_KEY_CACHE
#       if !(defined USE_KEY_CACHE_FILE)				\
    || ((USE_KEY_CACHE_FILE != NO) && (USE_KEY_CACHE_FILE != YES))
#           undef   USE_KEY_CACHE_FILE
#           define  USE_KEY_CACHE_FILE  YES     // Default: Either YES or NO
#       endif
#   else
#       undef   USE_KEY_CACHE_FILE
#       define  USE_KEY_CACHE_FILE      NO
#   endif   // USE_RSA_KEY_CACHE

// This provides fixed seeding of the RNG when doing debug on a simulator. This should allow
// consistent results on test runs as long as the input parameters to the functions remains the
// same. There is no default value.
#   if !(defined USE_DEBUG_RNG) || ((USE_DEBUG_RNG != NO) && (USE_DEBUG_RNG != YES))
#       undef   USE_DEBUG_RNG
#       define  USE_DEBUG_RNG           YES      // Default: Either YES or NO
#   endif

// Don't change these. They are the settings needed when not doing a simulation and not doing
// debug. Can't use the key cache except during debug. Otherwise, all of the key values end up being
// the same
#else
#   define USE_RSA_KEY_CACHE        NO
#   define USE_RSA_KEY_CACHE_FILE   NO
#   define USE_DEBUG_RNG            NO
#endif  // DEBUG && SIMULATION

#if DEBUG

// In some cases, the relationship between two values may be dependent on things that change based
// on various selections like the chosen cryptographic libraries. It is possible that these
// selections will result in incompatible settings. These are often detectable by the compiler but
// it isn't always possible to do the check in the preprocessor code. For example, when the check
// requires use of 'sizeof()' then the preprocessor can't do the comparison. For these cases, we
// include a special macro that, depending on the compiler will generate a warning to indicate if
// the check always passes or always fails because it involves fixed constants. To run these checks,
// define COMPILER_CHECKS.
#   if !(defined COMPILER_CHECKS)					\
    || ((COMPILER_CHECKS != NO) && (COMPILER_CHECKS != YES))
#       undef   COMPILER_CHECKS
#       define  COMPILER_CHECKS     NO      // Default: Either YES or NO
#   endif

// Some of the values (such as sizes) are the result of different options set in
// TpmProfile.h. The combination might not be consistent. A function is defined
// (TpmSizeChecks()) that is used to verify the sizes at run time. To enable the function, define
// this parameter.
#   if !(defined RUNTIME_SIZE_CHECKS)					\
    || ((RUNTIME_SIZE_CHECKS != NO) && (RUNTIME_SIZE_CHECKS != YES))
#       undef RUNTIME_SIZE_CHECKS
#       define RUNTIME_SIZE_CHECKS      NO      // Default: Either YES or NO  libtpms: NO
#   endif

// If doing debug, can set the DRBG to print out the intermediate test values. Before enabling this,
// make sure that the dbgDumpMemBlock() function has been added someplace (preferably, somewhere in
// CryptRand.c)
#   if !(defined DRBG_DEBUG_PRINT)					\
    || ((DRBG_DEBUG_PRINT != NO) && (DRBG_DEBUG_PRINT != YES))
#       undef   DRBG_DEBUG_PRINT
#       define  DRBG_DEBUG_PRINT    NO      // Default: Either YES or NO
#   endif

// If an assertion event it not going to produce any trace information (function and line number)
// then make FAIL_TRACE == NO
#   if !(defined FAIL_TRACE) || ((FAIL_TRACE != NO) && (FAIL_TRACE != YES))
#       undef   FAIL_TRACE
#       define  FAIL_TRACE          YES      // Default: Either YES or NO
#   endif

#endif // DEBUG

/* Indicate if the implementation is going to give lockout time credit for time up to the last
   orderly shutdown. */
#if !(defined ACCUMULATE_SELF_HEAL_TIMER)				\
    || ((ACCUMULATE_SELF_HEAL_TIMER != NO) && (ACCUMULATE_SELF_HEAL_TIMER != YES))
#   undef   ACCUMULATE_SELF_HEAL_TIMER
#   define  ACCUMULATE_SELF_HEAL_TIMER      YES       // Default: Either YES or NO
#endif

/* If the implementation is to compute the sizes of the proof and primary seed size values based on
   the implemented algorithms, then use this define. */
#if !(defined USE_SPEC_COMPLIANT_PROOFS)				\
    || ((USE_SPEC_COMPLIANT_PROOFS != NO) && (USE_SPEC_COMPLIANT_PROOFS != YES))
#   undef   USE_SPEC_COMPLIANT_PROOFS
#   define  USE_SPEC_COMPLIANT_PROOFS       YES       // Default: Either YES or NO
#endif

// Comment this out to allow compile to continue even though the chosen proof values do not match
// the compliant values. This is written so that someone would have to proactively ignore errors.
#if !(defined SKIP_PROOF_ERRORS)					\
    || ((SKIP_PROOF_ERRORS != NO) && (SKIP_PROOF_ERRORS != YES))
#   undef   SKIP_PROOF_ERRORS
#   define  SKIP_PROOF_ERRORS           NO       // Default: Either YES or NO
#endif

// This define is used to eliminate the use of bit-fields. It can be enabled for big- or
// little-endian machines. For big-endian architectures that number bits in registers from left to
// right (MSb0()) this must be enabled.  Little-endian machines number from right to left with the
// least significant bit having assigned a bit number of 0. These are LSb0() machines (they are also
// little-endian so they are also least-significant byte 0 (LSB0) machines. Big-endian (MSB0)
// machines may number in either direction (MSb0() or LSb0()). For an MSB0+MSb0() machine this
// value is required to be NO

#if !(defined USE_BIT_FIELD_STRUCTURES)					\
    || ((USE_BIT_FIELD_STRUCTURES != NO) && (USE_BIT_FIELD_STRUCTURES != YES))
#   undef   USE_BIT_FIELD_STRUCTURES
#   define  USE_BIT_FIELD_STRUCTURES    NO        // Default: Either YES or NO   libtpms: NO
#endif

// This define is used to control the debug for the CertifyX509() command.
#if !(defined CERTIFYX509_DEBUG)					\
    || ((CERTIFYX509_DEBUG != NO) && (CERTIFYX509_DEBUG != YES))
#   undef   CERTIFYX509_DEBUG
#   define  CERTIFYX509_DEBUG NO               // libtpms: NO
#endif

#if !(defined TABLE_DRIVEN_MARSHAL)					\
    || ((TABLE_DRIVEN_MARSHAL != NO) && (TABLE_DRIVEN_MARSHAL != YES))
#   undef   TABLE_DRIVEN_MARSHAL
#   define  TABLE_DRIVEN_MARSHAL NO    // Default: Either YES or NO  libtpms: NO
#endif

/* Change these definitions to turn all algorithms or commands ON or OFF. That is, to turn all
   algorithms on, set ALG_NO to YES. This is mostly useful as a debug feature. */
#define      ALG_YES      YES
#define      ALG_NO       NO
#define      CC_YES       YES
#define      CC_NO        NO
#endif // _TPM_BUILD_SWITCHES_H_

