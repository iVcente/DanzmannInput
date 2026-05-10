// Copyright (C) 2026 Vicente Danzmann. All Rights Reserved.

#include "DanzmannEnhancedInputComponent.h"

#include "DanzmannInputBinderInterface.h"
#include "DanzmannInputMappingContextConfig.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void UDanzmannEnhancedInputComponent::ProcessInputProfile(const UDanzmannInputProfile* InputProfile)
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
	
	// Add Input Mapping Contexts
	for (const FDanzmannInputMappingContextConfig& Entry : InputProfile->InputMappingContexts)
	{
		if (IsValid(Entry.InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(Entry.InputMappingContext, Entry.Priority);
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

void UDanzmannEnhancedInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Remove Input Mapping Contexts
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem(); IsValid(EnhancedInputLocalPlayerSubsystem))
	{
		for (const UInputMappingContext* InputMappingContext : InputMappingContextsAdded)
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
	
	// Remove Input Actions bindings
	for (const TPair<FGameplayTag, FDanzmannInputActionBindingHandle>& InputActionBindingHandles : InputActionsBindingHandles)
	{
		for (const TPair<ETriggerEvent, int32>& Handles : InputActionBindingHandles.Value.Handles)
		{
			RemoveInputActionBinding(Handles.Value);
		}
	}
	
	InputActionsBindingHandles.Empty();
	
	Super::EndPlay(EndPlayReason);
}

UEnhancedInputLocalPlayerSubsystem* UDanzmannEnhancedInputComponent::GetEnhancedInputLocalPlayerSubsystem() const
{
	AActor* Owner = GetOwner();
	
	if (!IsValid(Owner))
	{
		return nullptr;
	}
	
	APlayerController* PlayerController = nullptr;
	
	if (Owner->IsA(APawn::StaticClass()))
	{
		PlayerController = Cast<APlayerController>(Cast<APawn>(Owner)->GetController());
	}
	else if (Owner->IsA(APlayerController::StaticClass()))
	{
		PlayerController = Cast<APlayerController>(Owner);
	}
	else
	{
		return nullptr;
	}
	
	return IsValid(PlayerController) ? PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() : nullptr;
}
