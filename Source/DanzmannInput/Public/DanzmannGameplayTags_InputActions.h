// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Gameplay Tags used to represent Input Actions.
 */
namespace Danzmann::GameplayTags
{
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Move);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Look);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Jump);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Aim);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_ToggleRun);
}
