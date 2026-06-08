// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#include "DanzmannEnhancedInputComponent.h"

#include "DanzmannInputBinderInterface.h"
#include "DanzmannInputMappingContextConfig.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "InputMappingContext.h"

void UDanzmannEnhancedInputComponent::ApplyInputProfile(const UDanzmannInputProfile* InputProfile)
{
	AActor* Owner = GetOwner();

	if (!IsValid(InputProfile) || !IsValid(Owner))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem();

	if (!IsValid(EnhancedInputLocalPlayerSubsystem))
	{
		return;
	}

	// Clear any previously-applied state so apply is idempotent and runtime Profile swaps
	// don't stack the new IMCs on top of the old ones.
	ClearAppliedInputProfile();

	// Add Input Mapping Contexts
	for (const FDanzmannInputMappingContextConfig& Entry : InputProfile->InputMappingContexts)
	{
		if (IsValid(Entry.InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(Entry.InputMappingContext, Entry.Priority);
			InputMappingContextsAdded.Add(Entry.InputMappingContext);
		}
	}

	// Bind native Input Actions for owner
	if (Owner->Implements<UDanzmannInputBinderInterface>())
	{
		Cast<IDanzmannInputBinderInterface>(Owner)->BindInputActions(this, InputProfile);
	}

	// Bind native Input Actions for owner's Components
	TArray<UActorComponent*> Components;
	Owner->GetComponents(Components);

	for (UActorComponent* Component : Components)
	{
		if (!IsValid(Component) || !Component->Implements<UDanzmannInputBinderInterface>())
		{
			continue;
		}

		Cast<IDanzmannInputBinderInterface>(Component)->BindInputActions(this, InputProfile);
	}

	// Pawn's Player State can host binder Components too (e.g., an Ability System Component owned by
	// Player State instead of the Pawn)
	const APawn* OwnerPawn = Cast<APawn>(Owner);
	if (!IsValid(OwnerPawn))
	{
		return;
	}
	
	const APlayerState* PlayerState = OwnerPawn->GetPlayerState();
	if (!IsValid(PlayerState))
	{
		return;
	}
	
	TArray<UActorComponent*> PlayerStateComponents;
	PlayerState->GetComponents(PlayerStateComponents);

	for (UActorComponent* Component : PlayerStateComponents)
	{
		if (!IsValid(Component) || !Component->Implements<UDanzmannInputBinderInterface>())
		{
			continue;
		}

		Cast<IDanzmannInputBinderInterface>(Component)->BindInputActions(this, InputProfile);
	}
}

void UDanzmannEnhancedInputComponent::ClearAppliedInputProfile()
{
	// Remove Input Mapping Contexts. Subsystem may be null during teardown if the LocalPlayer is
	// already gone -- still reset the tracked list either way so a later re-apply starts clean
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem(); IsValid(EnhancedInputLocalPlayerSubsystem))
	{
		for (const UInputMappingContext* InputMappingContext : InputMappingContextsAdded)
		{
			if (IsValid(InputMappingContext))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
			}
		}
	}
	
	InputMappingContextsAdded.Reset();

	// Remove Input Action bindings
	for (const TPair<FGameplayTag, FDanzmannInputActionBindingHandle>& InputActionBindingHandles : InputActionsBindingHandles)
	{
		for (const TPair<ETriggerEvent, int32>& Handles : InputActionBindingHandles.Value.Handles)
		{
			RemoveInputActionBinding(Handles.Value);
		}
	}
	
	InputActionsBindingHandles.Empty();
}

void UDanzmannEnhancedInputComponent::RemoveInputActionBinding(const uint32 BindingHandle)
{
	RemoveBindingByHandle(BindingHandle);
}

void UDanzmannEnhancedInputComponent::RemoveInputActionBindings(TArray<uint32>& BindingHandles)
{
	for (const uint32 Handle : BindingHandles)
	{
		RemoveBindingByHandle(Handle);
	}

	BindingHandles.Reset();
}

void UDanzmannEnhancedInputComponent::RemoveInputActionBindings(const FGameplayTag& InputGameplayTag)
{
	if (InputActionsBindingHandles.Contains(InputGameplayTag))
	{
		TArray<uint32> BindingHandles;
		InputActionsBindingHandles.Find(InputGameplayTag)->Handles.GenerateValueArray(BindingHandles);
		RemoveInputActionBindings(BindingHandles);
		InputActionsBindingHandles.Remove(InputGameplayTag);
	}
}

void UDanzmannEnhancedInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearAppliedInputProfile();

	Super::EndPlay(EndPlayReason);
}

UEnhancedInputLocalPlayerSubsystem* UDanzmannEnhancedInputComponent::GetEnhancedInputLocalPlayerSubsystem() const
{
	AActor* Owner = GetOwner();
	
	if (!IsValid(Owner))
	{
		return nullptr;
	}

	const APlayerController* PlayerController = nullptr;
	
	if (Owner->IsA(APawn::StaticClass()))
	{
		PlayerController = Cast<APlayerController>(Cast<APawn>(Owner)->GetController());
	}
	else if (Owner->IsA(APlayerController::StaticClass()))
	{
		PlayerController = Cast<APlayerController>(Owner);
	}
	
	return IsValid(PlayerController) ? PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
}
