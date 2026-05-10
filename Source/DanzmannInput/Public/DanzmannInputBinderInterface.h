// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DanzmannInputBinderInterface.generated.h"

class UDanzmannEnhancedInputComponent;
class UDanzmannInputProfile;

UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UDanzmannInputBinderInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Called during APawn::SetupPlayerInputComponent() and APlayerController::SetupInputComponent().
 * Any Pawn or Player Controller that wishes to bind inputs should implement this interface and override
 * BindInputActions(). Inputs should be bound according to available information on InputProfile. 
 * Any Component (from Pawn or Player Controller) that wishes to bind inputs should the implement the
 * interface as well. BindInputActions() will be called automatically.
 */
class DANZMANNINPUT_API IDanzmannInputBinderInterface
{
	GENERATED_BODY()

	public:
		/**
		 * Register input bindings using the tag-keyed Input Actions in InputProfile.
		 * @param EnhancedInputComponent The Pawn's or Player Controllers's Enhanced Input Component.
		 * @param InputProfile Input profile containing input information.
		 */
		virtual void BindInputActions(UDanzmannEnhancedInputComponent* EnhancedInputComponent, const UDanzmannInputProfile* InputProfile) = 0;
};
