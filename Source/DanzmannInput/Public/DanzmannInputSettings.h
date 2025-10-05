// Copyright (C) 2025 Vicente Danzmann. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "DanzmannInputSettings.generated.h"

class UDanzmannDataAsset_Input;

/**
 * Settings for defining input configuration per class.
 * The UDanzmannEnhancedInputComponent will read given Actor's input configuration from here.
 */
UCLASS(Config = "Input", DefaultConfig)
class DANZMANNINPUT_API UDanzmannInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	public:
		#if WITH_EDITOR
			/**
			 * @see more info in UDeveloperSettings.
			 */
			virtual FText GetSectionText() const override
			{
				return FText::FromString(TEXT("Danzmann Input"));
			}
		#endif
	
		/**
		 * @see more info in UDeveloperSettings.
		 */
		virtual FName GetCategoryName() const override
		{
			return FName("Plugins");
		}
	
		/**
		 * Get input configuration for given class. If we don't find an entry with the
		 * original class, we'll start to go up in the inheritance chain checking if any
		 * of the ancestors has an entry.
		 * @param Class Class to find an input configuration.
		 * @return Input configuration for the class or any of its ancestors -- nullptr in
		 * case we don't find anything.
		 */
		TSoftObjectPtr<UDanzmannDataAsset_Input> GetInputConfiguration(const UClass* Class) const;

	private:
		/**
		 * Input configurations for any classes derived from AActor. You can set a configuration
		 * for a specific class or for any of its ancestors.
		 * For example, suppose we have entries for AActor and ACharacter. If you'd like to
		 * get a configuration for a class derived of ACharacter, ACharacter configuration would
		 * be returned. Doing the same for APawn would return the AActor configuration, as APawn
		 * has no entry, but one of its ancestors (AActor) does. Priority is given to the most
		 * specific class with an entry.
		 * @note Use UDanzmannInputSettings::GetInputConfiguration() to access the configurations. 
		 */
		UPROPERTY(Config, EditAnywhere)
		TMap<TSoftClassPtr<AActor>, TSoftObjectPtr<UDanzmannDataAsset_Input>> InputConfigurations;
};
