// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DanzmannDataAsset_Input.h"
#include "GameplayTags.h"

#include "DanzmannEnhancedInputComponent.generated.h"

UCLASS()
class DANZMANNINPUT_API UDanzmannEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	public:
		/**
		 * Bind an action according to a Gameplay Tag.
		 * @tparam TOwner Owner object that we'd like to bind to of type UObject.
		 * @tparam TCallback Callback function to bind with the following signature: const FInputActionInstance& Instance.
		 * @param GameplayTag Gameplay Tag to find Input Action in input configuration.
		 * @param InputConfiguration Data Asset containing a map of available Input Actions for given Pawn.
		 * @param TriggerEvent How Input Action should be triggered.
		 * @param Owner Owner object that we'd like to bind to.
		 * @param Callback Callback function to bind.
		 * @return Reference to FEnhancedInputActionEventBinding just bound.
		 */
		template<class TOwner = UObject, typename TCallback>
		FEnhancedInputActionEventBinding& BindActionByGameplayTag(const FGameplayTag& GameplayTag, const UDanzmannDataAsset_Input* InputConfiguration, ETriggerEvent TriggerEvent, TOwner* Owner, TCallback Callback)
		{
			checkf(InputConfiguration->InputActions.Contains(GameplayTag), TEXT("[%hs] No valid Input Action found for %s Gameplay Tag within %s input configuration."), __FUNCTION__, *GameplayTag.ToString(), *InputConfiguration->GetName());
			UInputAction* InputAction = InputConfiguration->InputActions.FindRef(GameplayTag);
			return BindAction(InputAction, TriggerEvent, Owner, Callback);	
		}
};
