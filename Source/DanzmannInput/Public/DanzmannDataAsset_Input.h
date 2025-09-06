// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"

#include "DanzmannDataAsset_Input.generated.h"

/**
 * Base Data Asset class to store input information.
 */
UCLASS()
class DANZMANNINPUT_API UDanzmannDataAsset_Input : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
		/**
		 * Default Input Mapping Context.
		 * @note It can be useful when you'd like to add a default set
		 * of available inputs to a Pawn that has just been possessed, for example.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (AssetBundles = "ImmediatelyLoading"))
		TSoftObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;
	
		/**
		 * Available Input Mapping Contexts.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (ForceInlineRow, Categories = "InputMappingContext", AssetBundles = "ImmediatelyLoading"))
		TMap<FGameplayTag, TSoftObjectPtr<UInputMappingContext>> InputMappingContexts;

		/**
		 * Available Input Actions.
		 */
		UPROPERTY(EditDefaultsOnly, Meta = (ForceInlineRow, Categories = "InputAction", AssetBundles = "ImmediatelyLoading"))
		TMap<FGameplayTag, TSoftObjectPtr<UInputAction>> InputActions;
};
