// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#include "DanzmannInputMappingContextConfig.h"

#include "InputMappingContext.h"

void FDanzmannInputMappingContextConfig::PostSerialize(const FArchive& Ar)
{
	#if WITH_EDITORONLY_DATA
		if (IsValid(InputMappingContext))
		{
			FString InputMappingContextName = InputMappingContext->GetName();
			InputMappingContextName.RemoveFromEnd(TEXT("_C"));
			EditorDisplayName = FString::Printf(TEXT("%s | Priority %d"), *InputMappingContextName, Priority);	
		}
	#endif
}
