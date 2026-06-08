// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "DanzmannInputProfile.generated.h"

class UInputAction;
struct FDanzmannInputActionConfig;
struct FDanzmannInputMappingContextConfig;

/**
 * Input Profile containing Input Actions and Input Mapping Contexts that
 * should be bound and added, respectively.
 */
UCLASS(BlueprintType, Const)
class DANZMANNINPUT_API UDanzmannInputProfile : public UDataAsset
{
	GENERATED_BODY()

	public:
		/**
		 * Find a native Input Action given Input Gameplay Tag.
		 * @param InputGameplayTag Tag to look up.
		 * @param bLogNotFound Log an error if the tag is missing.
		 * @return Matching Input Action, or nullptr if not found.
		 */
		const UInputAction* FindNativeInputActionForGameplayTag(const FGameplayTag& InputGameplayTag, bool bLogNotFound = true) const;

		/**
		 * Find a Gameplay Ability Input Action given Input Gameplay Tag.
		 * @param InputGameplayTag Tag to look up.
		 * @param bLogNotFound Log an error if the tag is missing.
		 * @return Matching Input Action, or nullptr if not found.
		 */
		const UInputAction* FindGameplayAbilityInputActionForGameplayTag(const FGameplayTag& InputGameplayTag, bool bLogNotFound = true) const;

		/**
		 * Native Input Actions to be bound.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = EditorDisplayName))
		TArray<FDanzmannInputActionConfig> NativeInputActions;

		/**
		 * Gameplay Ability Input Actions to be bound. Every tag listed here will be bound
		 * to the owning Pawn's or Player State's Ability System Component, regardless of whether
		 * the matching Gameplay Ability has been granted yet.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = EditorDisplayName))
		TArray<FDanzmannInputActionConfig> GameplayAbilityInputActions;

		/**
		 * Input Mapping Contexts (with priorities) to be added.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = EditorDisplayName))
		TArray<FDanzmannInputMappingContextConfig> InputMappingContexts;
};
