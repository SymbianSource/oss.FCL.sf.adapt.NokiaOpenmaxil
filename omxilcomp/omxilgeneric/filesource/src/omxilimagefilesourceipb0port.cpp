// Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "omxilimagefilesourceipb0port.h"
#include "omxilfilesourceprocessingfunction.h"
#include <string.h>
#include <uri8.h>

COmxILImageFileSourceIPB0Port* COmxILImageFileSourceIPB0Port::NewL(const TOmxILCommonPortData& aCommonPortData,
	                                                 const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
	                                                 const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats,
													 const COmxILFileSourceProcessingFunction& aFileSourcePF)
	{
	COmxILImageFileSourceIPB0Port* self = new (ELeave) COmxILImageFileSourceIPB0Port(aFileSourcePF);
	CleanupStack::PushL(self);
	self->ConstructL(aCommonPortData, aSupportedImageFormats, aSupportedColorFormats);
	CleanupStack::Pop(self);
	return self;
	}

void COmxILImageFileSourceIPB0Port::ConstructL(const TOmxILCommonPortData& aCommonPortData,
                                                const RArray<OMX_IMAGE_CODINGTYPE>& aSupportedImageFormats,
                                                const RArray<OMX_COLOR_FORMATTYPE>& aSupportedColorFormats)
	{
    COmxILImagePort::ConstructL(aCommonPortData, aSupportedImageFormats, aSupportedColorFormats);
	GetSupportedImageFormats().AppendL(OMX_IMAGE_CodingUnused);
	GetParamPortDefinition().eDomain = OMX_PortDomainImage;
	}

COmxILImageFileSourceIPB0Port::COmxILImageFileSourceIPB0Port(const COmxILFileSourceProcessingFunction& aFileSourcePF)
	: iFileSourcePF(aFileSourcePF)
	{
	}

COmxILImageFileSourceIPB0Port::~COmxILImageFileSourceIPB0Port()
	{
	iMimeType.Close();
	CleanUpPort();
	}

OMX_ERRORTYPE COmxILImageFileSourceIPB0Port::GetLocalOmxParamIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILImagePort::GetLocalOmxParamIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILImageFileSourceIPB0Port::GetLocalOmxConfigIndexes(RArray<TUint>& aIndexArray) const
	{
	return COmxILImagePort::GetLocalOmxConfigIndexes(aIndexArray);
	}

OMX_ERRORTYPE COmxILImageFileSourceIPB0Port::GetParameter(OMX_INDEXTYPE aParamIndex,
														TAny* apComponentParameterStructure) const
	{
	return COmxILImagePort::GetParameter(aParamIndex, apComponentParameterStructure);
	}

OMX_ERRORTYPE COmxILImageFileSourceIPB0Port::SetParameter(OMX_INDEXTYPE aParamIndex,
														const TAny* apComponentParameterStructure,
														TBool& aUpdateProcessingFunction)
	{
	return COmxILImagePort::SetParameter(aParamIndex,
										 apComponentParameterStructure,
										 aUpdateProcessingFunction);
	}

OMX_ERRORTYPE COmxILImageFileSourceIPB0Port::SetFormatInPortDefinition(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition,
																TBool& /*aUpdateProcessingFunction*/)
	{
	OMX_ERRORTYPE omxErr = OMX_ErrorNone;
    if (aPortDefinition.format.image.eCompressionFormat == OMX_IMAGE_CodingUnused)
        {
        OMX_STRING ptempMIMEType = GetParamPortDefinition().format.image.cMIMEType;
        GetParamPortDefinition().format.image = aPortDefinition.format.image;
        
        GetParamPortDefinition().format.image.cMIMEType = ptempMIMEType;
        if(aPortDefinition.format.image.cMIMEType)
            {
            TInt len= strlen (aPortDefinition.format.image.cMIMEType);
            if(len > 0)
                {
                TPtrC8 mimetype(reinterpret_cast<const TUint8 *>(aPortDefinition.format.image.cMIMEType), len +1 );
                HBufC8 *pHBuf = mimetype.Alloc();
                if(pHBuf != NULL )
                    {
                    iMimeType.Close();
                    iMimeType.Assign(pHBuf );
                    TUint8* pTempBuff = const_cast<TUint8*>(iMimeType.PtrZ() );
                    GetParamPortDefinition().format.image.cMIMEType = reinterpret_cast<OMX_STRING>(pTempBuff );
                    }
                }
            }
        }
    else
        {
        omxErr = OMX_ErrorUnsupportedSetting;
        }
	    
	return omxErr;
	}

TBool COmxILImageFileSourceIPB0Port::IsTunnelledPortCompatible(const OMX_PARAM_PORTDEFINITIONTYPE& aPortDefinition) const
	{
    if(aPortDefinition.eDomain != GetParamPortDefinition().eDomain)
        {
        return EFalse;
        }

    if (aPortDefinition.format.image.eCompressionFormat == OMX_IMAGE_CodingMax)
        {
        return EFalse;
        }
	return ETrue;
	}
