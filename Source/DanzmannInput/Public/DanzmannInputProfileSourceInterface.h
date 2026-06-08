// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "DanzmannInputProfileSourceInterface.generated.h"

class UDanzmannInputProfile;

UINTERFACE(MinimalAPI, Meta = (CannotImplementInterfaceInBlueprint))
class UDanzmannInputProfileSourceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implemented by Components that own the Input Profile a Pawn should use. The Enhanced Input Component
 * does not exist until possession, so the owning Pawn scans its Components for this interface during
 * APawn::SetupPlayerInputComponent() and applies the returned Input Profile itself. Implementers only
 * source the profile -- they do not perform the setup.
 */
class DANZMANNINPUT_API IDanzmannInputProfileSourceInterface
{
	GENERATED_BODY()

	public:
		/**
		 * Get the Input Profile the owning Pawn should apply to its Enhanced Input Component.
		 * @return Input Profile to apply (may be null).
		 */
		virtual const UDanzmannInputProfile* GetInputProfile() const = 0;
};
