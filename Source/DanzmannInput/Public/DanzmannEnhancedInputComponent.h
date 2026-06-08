// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DanzmannInputActionBindingHandle.h"
#include "DanzmannInputProfile.h"
#include "EnhancedInputComponent.h"
#include "GameplayTags.h"
#include "InputAction.h"

#include "DanzmannEnhancedInputComponent.generated.h"

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

/**
 * Enhanced Input Component extension with helpers for binding actions sourced from a UDanzmannInputProfile.
 * It also stores handles for all bound Input Actions. 
 * ApplyInputProfile() should be called during APawn::SetupPlayerInputComponent().
 */
UCLASS()
class DANZMANNINPUT_API UDanzmannEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

	public:
		/**
		 * Try to add Input Mapping Contexts and start the setup for input bindings for owner and its Components.
		 * Internally calls ClearAppliedInputProfile() first so apply is idempotent (clear-then-add) and
		 * runtime profile swaps don't leak Mapping Contexts or stale bindings onto the local player.
		 * @param InputProfile Input Profile containing Input Actions and Input Mapping Contexts.
		 * @note A null InputProfile is a no-op and does not clear existing state. Use
		 * ClearAppliedInputProfile() explicitly to wipe.
		 */
		void ApplyInputProfile(const UDanzmannInputProfile* InputProfile);

		/**
		 * Remove every Input Mapping Context and Input Action binding previously installed through
		 * ApplyInputProfile(). Idempotent -- safe to call when nothing was applied. Invoked
		 * automatically at the top of ApplyInputProfile() and from EndPlay(). Exposed publicly so
		 * external owners (e.g., UDanzmannPawnDataComponent during teardown) can wipe state without
		 * re-applying.
		 */
		void ClearAppliedInputProfile();
		
		/**
		 * Bind a native Input Action looked up by Gameplay Tag in the Input Profile.
		 * @tparam TOwner Owning UObject.
		 * @tparam TCallback Member function pointer matching one of the BindAction() signatures.
		 * @param InputProfile Input Profile to look the tag up in.
		 * @param InputGameplayTag Tag identifying the desired Input Action.
		 * @param TriggerEvent Trigger event to bind to.
		 * @param Owner Owner object that we'd like to bind to.
		 * @param Callback Member function on Owner to call when the trigger fires.
		 * @param bLogIfNotFound Log an error if InputGameplayTag is missing from the Profile.
		 */
		template<class TOwner, typename TCallback>
		void BindNativeInputAction(const UDanzmannInputProfile* InputProfile, const FGameplayTag& InputGameplayTag, ETriggerEvent TriggerEvent, TOwner* Owner, TCallback Callback, const bool bLogIfNotFound = true)
		{
			if (!IsValid(InputProfile))
			{
				return;
			}

			if (const UInputAction* InputAction = InputProfile->FindNativeInputActionForGameplayTag(InputGameplayTag, bLogIfNotFound); IsValid(InputAction))
			{
				const FEnhancedInputActionEventBinding& EventBinding = BindAction(InputAction, TriggerEvent, Owner, Callback);
				TMap<ETriggerEvent, uint32>& TriggerHandles = InputActionsBindingHandles.FindOrAdd(InputGameplayTag).Handles;
				ensureMsgf(!TriggerHandles.Contains(EventBinding.GetTriggerEvent()), TEXT("[%hs] Duplicate native Input Action bind for tag \"%s\" / trigger %d. Prior binding handle is being overwritten and will leak."), __FUNCTION__, *InputGameplayTag.GetTagName().ToString(), static_cast<int32>(EventBinding.GetTriggerEvent()));
				TriggerHandles.Add(EventBinding.GetTriggerEvent(), EventBinding.GetHandle());
			}
		}

		/**
		 * Bind a Gameplay Ability Input Action looked up by Gameplay Tag in the Input Profile.
		 * @tparam TOwner Owning UObject.
		 * @tparam TCallback Member function pointer matching one of the BindAction() signatures.
		 * @param InputProfile Input Profile to look the tag up in.
		 * @param InputGameplayTag Tag identifying the desired Input Action.
		 * @param TriggerEvent Trigger event to bind to.
		 * @param Owner Owner object that we'd like to bind to.
		 * @param Callback Member function on Owner to call when the trigger fires.
		 * @param bLogIfNotFound Log an error if InputGameplayTag is missing from the Profile.
		 */
		template<class TOwner, typename TCallback>
		void BindGameplayAbilityInputAction(const UDanzmannInputProfile* InputProfile, const FGameplayTag& InputGameplayTag, ETriggerEvent TriggerEvent, TOwner* Owner, TCallback Callback, const bool bLogIfNotFound = true)
		{
			if (!IsValid(InputProfile))
			{
				return;
			}

			if (const UInputAction* InputAction = InputProfile->FindGameplayAbilityInputActionForGameplayTag(InputGameplayTag, bLogIfNotFound); IsValid(InputAction))
			{
				const FEnhancedInputActionEventBinding& EventBinding = BindAction(InputAction, TriggerEvent, Owner, Callback, InputGameplayTag);
				TMap<ETriggerEvent, uint32>& TriggerHandles = InputActionsBindingHandles.FindOrAdd(InputGameplayTag).Handles;
				ensureMsgf(!TriggerHandles.Contains(EventBinding.GetTriggerEvent()), TEXT("[%hs] Duplicate Gameplay Ability Input Action bind for tag \"%s\" / trigger %d. Prior binding handle is being overwritten and will leak."), __FUNCTION__, *InputGameplayTag.GetTagName().ToString(), static_cast<int32>(EventBinding.GetTriggerEvent()));
				TriggerHandles.Add(EventBinding.GetTriggerEvent(), EventBinding.GetHandle());
			}
		}

		/**
		 * Remove binding for a handle.
		 * @param BindingHandle Binding handle to remove.
		 */
		void RemoveInputActionBinding(const uint32 BindingHandle);
		
		/**
		 * Remove every binding from BindingHandles, and then reset the array.
		 * @param BindingHandles Bind handles to remove.
		 */
		void RemoveInputActionBindings(TArray<uint32>& BindingHandles);
		
		/**
		 * Remove all bindings for an Input Action associated to a Gameplay Tag.
		 * @param InputGameplayTag Input Gameplay Tag to remove bindings.
		 */
		void RemoveInputActionBindings(const FGameplayTag& InputGameplayTag);

		/**
		 * Check if Input Action is already bound according to an Input Gameplay Tag.
		 * @param InputGameplayTag Gameplay Tag representing Input Action.
		 * @return Whether Input Action is bound or not.
		 */
		bool IsInputActionBound(const FGameplayTag& InputGameplayTag) const
		{
			return InputActionsBindingHandles.Contains(InputGameplayTag);
		}
	
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
