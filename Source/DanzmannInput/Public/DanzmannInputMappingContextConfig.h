// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "DanzmannInputMappingContextConfig.generated.h"

class UInputMappingContext;

/**
 * An Input Mapping Context paired with its priority.
 */
USTRUCT(BlueprintType)
struct FDanzmannInputMappingContextConfig
{
	GENERATED_BODY()

	/**
	 * Input Mapping Context asset.
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	/**
	 * Priority for given Input Mapping Context.
	 */
	UPROPERTY(EditAnywhere)
	int32 Priority = 0;
		
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
struct TStructOpsTypeTraits<FDanzmannInputMappingContextConfig> : public TStructOpsTypeTraitsBase2<FDanzmannInputMappingContextConfig>
{
	enum
	{
		WithPostSerialize = true,
	};
};