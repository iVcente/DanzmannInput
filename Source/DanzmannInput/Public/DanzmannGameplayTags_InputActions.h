// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Gameplay Tags used to represent Input Actions.
 */
namespace Danzmann::GameplayTags
{
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Move);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Look_Mouse);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Look_Gamepad);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Jump);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Run);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Sprint);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Crouch);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Aim);
	DANZMANNINPUT_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputAction_Fire);
}
