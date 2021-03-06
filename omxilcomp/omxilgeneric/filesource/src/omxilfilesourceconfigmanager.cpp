// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//



/**
 * @file
 * @internalTechnology
 */

#include "omxilfilesourceconfigmanager.h"
#include "omxilfilesourceprocessingfunction.h"
#include <openmax/il/common/omxilspecversion.h>

COmxILFileSourceConfigManager* COmxILFileSourceConfigManager::NewL(
                                        const TDesC8& aComponentName,
                                        const OMX_VERSIONTYPE& aComponentVersion,
                                        const RPointerArray<TDesC8>& aComponentRoleList,
                                        COmxILFileSourceProcessingFunction& aFileSourcePF)
	{
	COmxILFileSourceConfigManager* self = new(ELeave) COmxILFileSourceConfigManager(aFileSourcePF);
	CleanupStack::PushL(self);
	self->ConstructL(aComponentName, aComponentVersion, aComponentRoleList);
	CleanupStack::Pop(self);
	return self;
	}

COmxILFileSourceConfigManager::COmxILFileSourceConfigManager(COmxILFileSourceProcessingFunction& aFileSourcePF)
	:iFileSourcePF(aFileSourcePF)
	{
	}

void COmxILFileSourceConfigManager::ConstructL(
                                    const TDesC8& aComponentName,
                                    const OMX_VERSIONTYPE& aComponentVersion,
                                    const RPointerArray<TDesC8>& aComponentRoleList)
	{
	COmxILConfigManager::ConstructL(aComponentName, aComponentVersion, aComponentRoleList);
	
	InsertParamIndexL(OMX_IndexParamContentURI);
	}

COmxILFileSourceConfigManager::~COmxILFileSourceConfigManager()
	{
	}

OMX_ERRORTYPE COmxILFileSourceConfigManager::GetParameter(OMX_INDEXTYPE aParamIndex, TAny* aPtr) const
	{
	TInt index = FindParamIndex(aParamIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}
		
	switch (aParamIndex)
		{
		case OMX_IndexParamContentURI:
			{			
			OMX_PARAM_CONTENTURITYPE* param = reinterpret_cast<OMX_PARAM_CONTENTURITYPE*>(aPtr);
			const HBufC8* uriData = iFileSourcePF.Uri();
			if (!uriData)
				{
				return OMX_ErrorNotReady;
				}
			
			const OMX_PARAM_CONTENTURITYPE* uri = reinterpret_cast<const OMX_PARAM_CONTENTURITYPE*>(uriData->Ptr());
			if (uri->nSize > param->nSize)
				{
				return OMX_ErrorBadParameter;
				}
			
			// The client's structure is guaranteed to be big enough.
			Mem::Copy(param, uri, uri->nSize);            
			}
			break;
			
		default:
			{
			return COmxILConfigManager::GetParameter(aParamIndex, aPtr);
			}
		}

	return OMX_ErrorNone;
	}

OMX_ERRORTYPE COmxILFileSourceConfigManager::SetParameter(OMX_INDEXTYPE aParamIndex, const TAny* aPtr, OMX_BOOL aInitTime)
	{
	TInt index = FindParamIndex(aParamIndex);
	if (KErrNotFound == index)
		{
		return OMX_ErrorUnsupportedIndex;
		}

	OMX_ERRORTYPE omxErr = OMX_ErrorNone;
	
	switch (aParamIndex)
		{
		case OMX_IndexParamContentURI:
		    {
            if(aInitTime == OMX_TRUE)
                {
                omxErr = iFileSourcePF.ParamIndication(aParamIndex, aPtr);
                }
            else
                {
                omxErr = OMX_ErrorUndefined;
                }
            break;
			}
		default:
			{
			omxErr = COmxILConfigManager::SetParameter(aParamIndex, aPtr, aInitTime);
			break;
			}
		}

	return omxErr;
	}
