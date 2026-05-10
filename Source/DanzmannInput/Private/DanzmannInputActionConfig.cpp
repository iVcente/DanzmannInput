// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#include "DanzmannInputActionConfig.h"

#include "InputAction.h"

void FDanzmannInputActionConfig::PostSerialize(const FArchive& Ar)
{
	#if WITH_EDITORONLY_DATA
		if (IsValid(InputAction) && InputGameplayTag.IsValid())
		{
			FString InputActionName = InputAction->GetName();
			InputActionName.RemoveFromEnd(TEXT("_C"));
			EditorDisplayName = FString::Printf(TEXT("%s | %s"), *InputActionName, *InputGameplayTag.ToString());	
		}
	#endif
}
