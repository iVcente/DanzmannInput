// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#include "DanzmannInputProfile.h"

#include "DanzmannInputActionConfig.h"
#include "DanzmannLogDanzmannInput.h"

const UInputAction* UDanzmannInputProfile::FindNativeInputActionForGameplayTag(const FGameplayTag& InputGameplayTag, const bool bLogNotFound) const
{
	for (const FDanzmannInputActionConfig& Action : NativeInputActions)
	{
		if (Action.InputGameplayTag == InputGameplayTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDanzmannInput, Error, TEXT("[%hs] Can't find native Input Action for tag \"%s\" on Input Profile \"%s\"."), __FUNCTION__, *InputGameplayTag.GetTagName().ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UDanzmannInputProfile::FindGameplayAbilityInputActionForGameplayTag(const FGameplayTag& InputGameplayTag, const bool bLogNotFound) const
{
	for (const FDanzmannInputActionConfig& Action : GameplayAbilityInputActions)
	{
		if (Action.InputGameplayTag == InputGameplayTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogDanzmannInput, Error, TEXT("[%hs] Can't find Gameplay Ability Input Action for tag \"%s\" on Input Profile \"%s\"."), __FUNCTION__, *InputGameplayTag.GetTagName().ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
