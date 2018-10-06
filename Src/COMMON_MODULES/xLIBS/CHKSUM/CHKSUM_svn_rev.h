/*! \file
*
*   \brief      CHKSUM SVN revision information generated from .tmpl file using SubWCRev
*
*/
/* COPYRIGHT NOTICE
* =================
*
* The contents of this document are protected under copyright and contain
* commercially and/or technically confidential information. The content of
* this document must not be used other than for the purpose for which it was
* provided nor may it be disclosed or copied (by the authorised recipient or
* otherwise) without the prior written consent of an authorised officer of
* Schrader Electronics Ltd.
*
*         (C) $Date:: 2009#$ Schrader Electronics Ltd.
*
*******************************************************************************
*/

#ifndef CHKSUM_SVN_REV_H
#define CHKSUM_SVN_REV_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

/*******************************************************************************
**                      SVN revisions Section                                 **
*******************************************************************************/
/* Current Module SVN revision */
#define CHKSUM_SVN_REVISION            (16309ul)

/* Module is NOT locally modified - OK */
#define CHKSUM_SVN_LOCALLY_MODIFIED    (0u)

/* Module is NOT mixed - OK */
#define CHKSUM_SVN_MIXED               (0u)

/* Other SVN Strings */
#define CHKSUM_SVN_URL                 "https://selacvs01.schrader.local:8443/svn/ECU_Software/COMMON_MODULES/LIBS/trunk/CHKSUM"
#define CHKSUM_SVN_DATE                "2013/11/28 14:34:47"
#define CHKSUM_SVN_RANGE               "16309"

/*******************************************************************************
**                      Version Checks                                        **
*******************************************************************************/

// Check that <MSN> folder matches the expected version
#if( CHKSUM_SVN_REVISION != CHKSUM_EXPECTED_SVN_REVISION )
    //#error "Module SVN folder revision does not match expected revision, check correct SVN versions have been checked out"
#endif // #if( CHKSUM_SVN_REVISION != CHKSUM_EXPECTED_SVN_REVISION )

// Check that SVN folder is NOT partially updated
#if( CHKSUM_SVN_MIXED == 1u )
    #error "Mixed SVN Module versions, please do a SVN update on Module"
#endif // #if( CHKSUM_SVN_MIXED == 1u )

// Check correct version of config file is being used
#if( CHKSUM_CONFIG_VERSION != 2 )
    //#error "Incorrect CHKSUM_config.h file version being used, please update to latest format in CHKSUM_config_template.h"
#endif // #if( CHKSUM_CONFIG_VERSION == X )

#endif /* #ifndef CHKSUM_SVN_REV_H */
