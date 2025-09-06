// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#include "DanzmannInputSettings.h"

TSoftObjectPtr<UDanzmannDataAsset_Input> UDanzmannInputSettings::GetInputConfiguration(const UClass* Class) const
{
	/*
	 * Try to find input configuration for Class.
	 */
	
	if (InputConfigurations.Contains(Class))
	{
		return InputConfigurations.FindRef(Class);
	}

	/*
	 * In case we haven't found a key with Class on InputConfigurationPerClass,
	 * let's check if its parent class has an entry -- up to UObject, which is our top level.
	 * If no entry is found until we reach top level, then it means we don't have an input
	 * configuration for the original Class parameter and neither for any of its ancestors.
	 */

	const UClass* Parent = Class->GetSuperClass();
	
	// We reached the top level
	if (Parent == UObject::StaticClass())
	{
		return nullptr;
	}

	// Let's try to find an entry for its parent class
	return GetInputConfiguration(Parent);
}
