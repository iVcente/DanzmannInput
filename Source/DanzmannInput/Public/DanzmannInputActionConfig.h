// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "DanzmannInputActionConfig.generated.h"

class UInputAction;

/**
 * A pairing of a Gameplay Tag to an Input Action. Let gameplay code refer to 
 * inputs by tag (e.g., "InputAction.Move") instead of by direct asset reference.
 */
USTRUCT(BlueprintType)
struct FDanzmannInputActionConfig
{
	GENERATED_BODY()

	/**
	 * Gameplay Tag representing an Input Action.
	 */
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputAction"))
	FGameplayTag InputGameplayTag;

	/**
	 * Input Action associated to Gameplay Tag.
	 */
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;
	
	#if WITH_EDITORONLY_DATA
		/**
		 * Editor Display Name to improve struct readability in the Editor.
		 * @note VisibleAnywhere (or similar) is required so the TitleProperty specifier code path can find this property.
		 * EditCondition and EditConditionHides is just to keep this property hidden to the user. 
		 */
		UPROPERTY(VisibleAnywhere, Meta = (EditCondition = false, EditConditionHides))
		FString EditorDisplayName;
	#endif

	/**
	 * Format EditorDisplayName here.
	 * @see more info in Class.h.
	 */
	void PostSerialize(const FArchive& Ar);
};

/**
 * Boilerplate so PostSerialize() is called.
 */
template<>
struct TStructOpsTypeTraits<FDanzmannInputActionConfig> : public TStructOpsTypeTraitsBase2<FDanzmannInputActionConfig>
{
	enum
	{
		WithPostSerialize = true,
	};
};
