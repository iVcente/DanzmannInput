// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputMappingContext.h"

#include "DanzmannDataAsset_Input.generated.h"

/**
 * Base Data Asset class to store input information for all possessable Pawns.
 */
UCLASS()
class DANZMANNINPUT_API UDanzmannDataAsset_Input : public UDataAsset
{
	GENERATED_BODY()

	public:
		/**
		 * Input Mapping Contexts available for given Pawn.
		 */
		UPROPERTY(EditDefaultsOnly)
		TMap<FGameplayTag, TObjectPtr<UInputMappingContext>> InputMappingContexts;

		/**
		 * Input Actions available for given Pawn.
		 */
		UPROPERTY(EditDefaultsOnly)
		TMap<FGameplayTag, TObjectPtr<UInputAction>> InputActions;
};
