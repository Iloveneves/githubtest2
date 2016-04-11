/****************************************************************************/
/*                                                                          */
/*  Copyright (c) 2001-2003 Shanghai Advanced Digital Technology Co., Ltd   */
/*                                                                          */
/* PROPRIETARY RIGHTS of Shanghai Advanced Digital Technology Co., Ltd      */
/* are involved in the subject matter of this material. All manufacturing,  */
/* reproduction, use, and sales rights pertaining to this subject matter    */
/* are governed by the license agreement. The recipient of this software    */
/* implicitly accepts the terms of the license.                             */
/*                                                                          */
/****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* FILE NAME                                                                */
/*      mgeos.c                                                             */
/*                                                                          */
/* VERSION                                                                  */
/*      v2.6                                                                */
/*                                                                          */
/* DESCRIPTION                                                              */
/*      The functions contained in this file should be rewrite when port to */
/*                                                                          */
/*                                                                          */
/* AUTHOR                                                                   */
/*      Gaston Shi                                                          */
/*                                                                          */
/****************************************************************************/
#include "mgdef.h"

#include "../../inc/fyf_def.h"
#include "../../dev/fyf_demux.h"
#include "../../os/fyf_semaphore.h"
extern unsigned char	 fyf_ca_mg_atr[33];
#include "../../inc/ca/fyf_ca.h"


#define FYF_CA_Debug soc_printf


BS32 mgca_handle;
static MG_STATUS fyf_ca_mg_regstate = MG_FAILING;
static MG_STATUS fyf_ca_mg_needupdate = MG_FAILING;

BS32 FYF_CA_MG_GetCaRegState(void)
{
	return fyf_ca_mg_regstate;
}

BU32 FYF_CA_MG_SetCaRegState(BS32 state)
{
	fyf_ca_mg_regstate = state;
}
MG_STATUS FYF_CA_MG_GetCASNeedUpdateSatus(void)
{
	return fyf_ca_mg_needupdate;
}
void FYF_CA_MG_SetCASNeedUpdateSatus(MG_STATUS UpdateSatus)
{
	if(UpdateSatus == MG_ERR_NEED_GKUPD)
	{
		FYF_CA_Debug("MG_ERR_NEED_GKUPD\n");
	}
	fyf_ca_mg_needupdate = UpdateSatus;
}

