// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#include "DanzmannEnhancedInputComponent.h"

#include "DanzmannInputSettings.h"
#include "Engine/AssetManager.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

void UDanzmannEnhancedInputComponent::LoadInputConfiguration(FDanzmannOnInputConfigurationReady Delegate)
{
	/*
	 * In case of already loaded, just return input configuration. 
	 */
	
	if (InputConfigurationStreamableHandle.IsValid() && InputConfigurationStreamableHandle->HasLoadCompleted())
	{
		UE_LOG(LogTemp, Warning, TEXT("Already loaded!"));
		Delegate.Execute(InputConfigurationStreamableHandle->GetLoadedAsset<UDanzmannDataAsset_Input>());
		return;
	}

	/*
	 * In case input configuration loading is in progress. 
	 */
	
	if (InputConfigurationStreamableHandle.IsValid() && InputConfigurationStreamableHandle->IsLoadingInProgress())
	{
		PendingInputConfigurationLoadedDelegates.Add(MoveTemp(Delegate));

		if (!InputConfigurationStreamableHandle->HasCompleteDelegate())
		{
			InputConfigurationStreamableHandle->BindCompleteDelegate(FStreamableDelegate::CreateUObject(this, &ThisClass::OnInputConfigurationLoaded));
		}
		return;
	}

	/*
	 * Request to load input configuration.
	 */
	
	const UDanzmannInputSettings* InputSettings = GetDefault<UDanzmannInputSettings>();
	const UClass* OwnerClass = GetOwner()->GetClass();
	
	if (const TSoftObjectPtr<UDanzmannDataAsset_Input> InputConfigurationToLoad = InputSettings->GetInputConfiguration(OwnerClass); !InputConfigurationToLoad.IsNull())
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		const FPrimaryAssetId InputConfigurationAssetId = UKismetSystemLibrary::GetPrimaryAssetIdFromSoftObjectReference(InputConfigurationToLoad);

		PendingInputConfigurationLoadedDelegates.Add(MoveTemp(Delegate));

		InputConfigurationStreamableHandle = AssetManager.PreloadPrimaryAssets(
			{InputConfigurationAssetId},
			{FName(TEXT("ImmediatelyLoading"))},
			false,
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnInputConfigurationLoaded)
		);
	}
}

void UDanzmannEnhancedInputComponent::OnInputConfigurationLoaded()
{
	const UDanzmannDataAsset_Input* InputConfiguration = InputConfigurationStreamableHandle->GetLoadedAsset<UDanzmannDataAsset_Input>();
	
	for (FDanzmannOnInputConfigurationReady& Delegate : PendingInputConfigurationLoadedDelegates)
	{
		Delegate.ExecuteIfBound(InputConfiguration);
	}
	
	PendingInputConfigurationLoadedDelegates.Empty();
}
