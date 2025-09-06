// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Gameplay Tags used to represent Input Mapping Contexts.
 */
namespace Danzmann::GameplayTags
{
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Walking);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Running);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Sprinting);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Crouching);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Sliding);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputMappingContext_Aiming);
}
