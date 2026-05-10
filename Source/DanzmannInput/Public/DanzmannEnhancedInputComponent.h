// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DanzmannInputActionBindingHandle.h"
#include "DanzmannInputProfile.h"
#include "GameplayTags.h"
#include "InputAction.h"

#include "DanzmannEnhancedInputComponent.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * Enhanced Input Component extension with helpers for binding actions sourced from a UDanzmannInputProfile.
 * It also stores handles for all bound Input Actions. 
 * ProcessInputProfile() should be called during APawn::SetupPlayerInputComponent() and APlayerController::SetupInputComponent().
 */
UCLASS()
class DANZMANNINPUT_API UDanzmannEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	public:
		/**
		 * Try to add Input Mapping Contexts and start the setup for input bindings for owner and its Components.
		 * @param InputProfile Input profile containing Input Actions and Input Mapping Contexts.
		 */
		void ProcessInputProfile(const UDanzmannInputProfile* InputProfile);
		
		/**
		 * Bind an Input Action looked up by Gameplay Tag in the Input Profile.
		 * @tparam TOwner Owning UObject.
		 * @tparam TCallback Member function pointer matching one of the BindAction() signatures.
		 * @param InputProfile Input Profile to look the tag up in.
		 * @param InputGameplayTag Tag identifying the desired Input Action.
		 * @param TriggerEvent Trigger event to bind to.
		 * @param Owner Owner object that we'd like to bind to.
		 * @param Callback Member function on Owner to call when the trigger fires.
		 * @param bLogIfNotFound Log an error if InputGameplayTag is missing from the profile.
		 */
		template<class TOwner, typename TCallback>
		void BindInputAction(const UDanzmannInputProfile* InputProfile, const FGameplayTag& InputGameplayTag, ETriggerEvent TriggerEvent, TOwner* Owner, TCallback Callback, const bool bLogIfNotFound = true)
		{
			if (!IsValid(InputProfile))
			{
				return;
			}

			if (const UInputAction* InputAction = InputProfile->FindInputActionForGameplayTag(InputGameplayTag, bLogIfNotFound); IsValid(InputAction))
			{
				// Bind action and store handle
				const FEnhancedInputActionEventBinding& EventBinding = BindAction(InputAction, TriggerEvent, Owner, Callback);
				InputActionsBindingHandles.FindOrAdd(InputGameplayTag).Handles.FindOrAdd(EventBinding.GetTriggerEvent(), EventBinding.GetHandle());
			}
		}

		/**
		 * Remove binding for a handle.
		 * @param BindingHandle Binding handle to remove.
		 */
		void RemoveInputActionBinding(const uint32 BindingHandle);
		
		/**
		 * Remove every binding from BindHandles, and then reset the array.
		 * @param BindingHandles Bind handles to remove.
		 */
		void RemoveInputActionBindings(TArray<uint32>& BindingHandles);
		
	
	protected:
		/**
		 * Remove all inputs bindings and remove Input Mapping Contexts.
		 * @see more info in ActorComponent.h.
		 */
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
	private:
		/**
		 * Helper to get UEnhancedInputLocalPlayerSubsystem.
		 * @return Enhanced Input Local Player Subsystem if found. Otherwise, return nullptr.
		 */
		UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem() const;

		/**
		 * Input Mapping Contexts added by the Component from a UDanzmannInputProfile. 
		 */
		UPROPERTY()
		TArray<TObjectPtr<UInputMappingContext>> InputMappingContextsAdded;

		/**
		 * Input Actions binding handles.
		 */
		TMap<FGameplayTag, FDanzmannInputActionBindingHandle> InputActionsBindingHandles;
};
