// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"

#include "DanzmannInputActionBindingHandle.generated.h"

/**
 * Store binding handles for an Input Action.
 */
USTRUCT()
struct FDanzmannInputActionBindingHandle
{
	GENERATED_BODY()

	/**
	 * Handle for each trigger event associated to an Input Action.
	 */
	UPROPERTY()
	TMap<ETriggerEvent, int32> Handles;
};
