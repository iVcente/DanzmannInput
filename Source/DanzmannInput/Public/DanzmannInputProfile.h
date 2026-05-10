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
 * Input profile containing Input Actions and Input Mapping Contexts that
 * should be bound and added, respectively.
 */
UCLASS(BlueprintType, Const)
class DANZMANNINPUT_API UDanzmannInputProfile : public UDataAsset
{
	GENERATED_BODY()

	public:
		/**
		 * Find the Input Action given InputGameplayTag.
		 * @param InputGameplayTag Tag to look up.
		 * @param bLogNotFound Log an error if the tag is missing.
		 * @return Matching Input Action, or nullptr if not found.
		 */
		const UInputAction* FindInputActionForGameplayTag(const FGameplayTag& InputGameplayTag, bool bLogNotFound = true) const;

		/**
		 * Input Actions to be bound.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = EditorDisplayName))
		TArray<FDanzmannInputActionConfig> InputActions;

		/**
		 * Input Mapping Contexts (with priorities) to be added.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = EditorDisplayName))
		TArray<FDanzmannInputMappingContextConfig> InputMappingContexts;
};
