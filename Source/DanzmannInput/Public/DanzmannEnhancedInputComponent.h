// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DanzmannDataAsset_Input.h"
#include "DanzmannLogDanzmannInput.h"
#include "EnhancedInputComponent.h"
#include "GameplayTags.h"

#include "DanzmannEnhancedInputComponent.generated.h"

/**
 * Delegate used to load or access already loaded input configurations.
 */
DANZMANNINPUT_API DECLARE_DELEGATE_OneParam(FDanzmannOnInputConfigurationReady, const UDanzmannDataAsset_Input* InputConfiguration);

/**
 * Specialization of UEnhancedInputComponent that allows binding Input Actions
 * by Gameplay Tag and also handles loading UDanzmannDataAsset_Input input configurations.
 */
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
			UE_CLOG(!InputConfiguration->InputActions.Contains(GameplayTag), LogDanzmannInput, Error, TEXT("[%hs] No valid Input Action found for \"%s\" within input configuration named %s."), __FUNCTION__, *GameplayTag.ToString(), *InputConfiguration->GetName());
			UInputAction* InputAction = InputConfiguration->InputActions.FindRef(GameplayTag).Get();
			return BindAction(InputAction, TriggerEvent, Owner, Callback);	
		}

		/**
		 * Try to load input configuration for the owner of UDanzmannEnhancedInputComponent.
		 * @param Delegate Callback to when load is completed, successfully or not.
		 * @note Loading is done only once. In case input configuration is already in
		 * memory, Delegate will be triggered immediately. Make sure you added input
		 * configurations in the Project Settings for component's owner class.
		 */
		void LoadInputConfiguration(FDanzmannOnInputConfigurationReady Delegate);
	
	private:
		/**
		 * Callback to when input configuration is loaded into memory.
		 * Execute delegates (functions that requested input configuration
		 * while it wasn't loaded yet) from PendingInputConfigurationLoadedDelegates.
		 */
		void OnInputConfigurationLoaded();

		/**
		 * List of delegates from functions that called LoadInputConfiguration() while
		 * input configuration wasn't loaded into memory yet. For each delegate received,
		 * we add them to this list and once loading is done, we iterate through them all
		 * triggering what they're bound to.
		 */
		TArray<FDanzmannOnInputConfigurationReady> PendingInputConfigurationLoadedDelegates;
	
		/**
		 * Streamable handle that stores reference to loaded input configuration.
		 */
		TSharedPtr<FStreamableHandle> InputConfigurationStreamableHandle = nullptr;
};
