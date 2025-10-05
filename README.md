# DanzmannInput
An extension of `UEnhancedInputComponent` that proposes setting up inputs according to Data Assets and binding them by Gameplay Tags. The goal is to decouple input configuration from an Actor as much as possible, focusing on defining the relationship of Actor and input configuration in the `Project Settings`. The plugin internally makes use of the Asset Manager to handle loading/unloading of the input configurations.

## Setup & Usage Example

> Make sure you have added the `DanzmannInput` module to your project's Build.cs file. Also, enable `DanzmannInput` in your `.uproject` file.

Then we need to create Gameplay Tags that represent Input Actions and Input Mapping Contexts. Feel free to modify `DanzmannGameplayTags_InputActions` and `DanzmannGameplayTags_InputMappingContexts` files for your needs -- take the content already present as an example.

Below is a suggestion on how to set up a default Input Mapping Context for your Pawns using their base class, so whenever any Pawn is possessed the player will be able to control them right away:
```cpp
// MyBaseCharacter.h
...
#pragma region Input

	public:
	    /**
	     * Set up default Input Mapping Context.
	     * @see more info APawn.
	     */
	    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	private:
		/**
		 * Add default Input Mapping Context.
		 * @param InputConfiguration Input configuration containing input data.
		 */
		void AddDefaultInputMappingContext(const UDanzmannDataAsset_Input* InputConfiguration) const;

#pragma endregion Input
```

```cpp
// MyBaseCharacter.cpp
...
#pragma region Input

	void AMyBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);
	
		// Try to add a default Input Mapping Context
		UDanzmannEnhancedInputComponent* EnhancedInputComponent = Cast<UDanzmannEnhancedInputComponent>(InputComponent);
		EnhancedInputComponent->LoadInputConfiguration(FDanzmannOnInputConfigurationReady::CreateUObject(this, &ThisClass::AddDefaultInputMappingContext));
	}
	
	void AMyBaseCharacter::AddDefaultInputMappingContext(const UDanzmannDataAsset_Input* InputConfiguration) const
	{
		const UDanzmannEnhancedInputComponent* EnhancedInputComponent = Cast<UDanzmannEnhancedInputComponent>(InputComponent);
	
		if (IsValid(InputConfiguration) && InputConfiguration->DefaultInputMappingContext.IsValid())
		{
			const ULocalPlayer* LocalPlayer = Cast<APlayerController>(GetController())->GetLocalPlayer();
			UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputConfiguration->DefaultInputMappingContext.Get(), 0);
		}
	}

#pragma endregion Input
```

The binding of the Input Actions is set up on the derived classes. The idea is to bind all Input Actions contained in the input configuration Data Asset and use multiple Input Mapping Contexts to enable/disable actions:
```cpp
// MyDerivedCharacter.h
...
#pragma region Input

	public:
		/**
		 * Set up binding for Input Actions.
		 * @see more info APawn.
		 */
		virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	private:
		/**
		 * Setup input bindings according to InputConfiguration.
		 */
		void SetupInputBindings(const UDanzmannDataAsset_Input* InputConfiguration);

		/**
		 * Callback to when we'd like to perform the Move action according to given input.
		 * @param Instance Contain value for Input Action.
		 */
		void Input_Move(const FInputActionInstance& Instance);

		/**
		 * Callback to when we'd like to perform the start Jump action according to given input.
		 * @param Instance Contain value for Input Action.
		 */
		void Input_ActivateJump(const FInputActionInstance& Instance);

		/**
		 * Callback to when we'd like to perform the stop the Jump action according to given input.
		 * @param Instance Contain value for Input Action.
		 */
		void Input_DeactivateJump(const FInputActionInstance& Instance);

#pragma endregion Input
```

```cpp
// MyDerivedCharacter.cpp
...
#pragma region Input

	void AMyDerivedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);

		UDanzmannEnhancedInputComponent* EnhancedInputComponent = Cast<UDanzmannEnhancedInputComponent>(PlayerInputComponent);
		EnhancedInputComponent->LoadInputConfiguration(FDanzmannOnInputConfigurationReady::CreateUObject(this, &ThisClass::SetupInputBindings));
	}

	void AMyDerivedCharacter::SetupInputBindings(const UDanzmannDataAsset_Input* InputConfiguration)
	{
		UDanzmannEnhancedInputComponent* EnhancedInputComponent = Cast<UDanzmannEnhancedInputComponent>(InputComponent);

		EnhancedInputComponent->BindActionByGameplayTag(Danzmann::GameplayTags::InputAction_Move, InputConfiguration, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindActionByGameplayTag(Danzmann::GameplayTags::InputAction_Jump, InputConfiguration, ETriggerEvent::Started, this, &ThisClass::Input_ActivateJump);
		EnhancedInputComponent->BindActionByGameplayTag(Danzmann::GameplayTags::InputAction_Jump, InputConfiguration, ETriggerEvent::Completed, this, &ThisClass::Input_DeactivateJump);
	}

	void AMyDerivedCharacter::Input_Move(const FInputActionInstance& Instance)
	{
		const FVector2D AxisValue = Instance.GetValue().Get<FVector2D>();

		const FRotator MovementRotation = FRotator(0.0f, GetControlRotation().Yaw, 0.0f);
	
		if (AxisValue.X != 0.0f)
		{
		 	AddMovementInput(MovementRotation.RotateVector(FVector::RightVector), AxisValue.X);
		}
		if (AxisValue.Y != 0.0f)
		{
		 	AddMovementInput(MovementRotation.RotateVector(FVector::ForwardVector), AxisValue.Y);
		}
	}

	void AMyDerivedCharacter::Input_ActivateJump(const FInputActionInstance& Instance)
	{
		Jump();
	}

	void AMyDerivedCharacter::Input_DeactivateJump(const FInputActionInstance& Instance)
	{
		StopJumping();
	}

```

The C++ side is done. Let's make the final adjustments in the editor. Create your Input Actions and Input Mapping Contexts.

Then, create a Data Asset that inherits from `UDanzmannDataAsset_Input`. You can create one Data Asset for each input configuration, for example:

<img width="1920" height="592" alt="image" src="https://github.com/user-attachments/assets/b0e04a4a-1053-403d-a672-29f340ec5b36"/>

Now, in `Project Settings` and under `Plugins > Danzmann Input Settings` map your input configuration to a class that will be using it when possessed:

<img width="1920" height="362" alt="image" src="https://github.com/user-attachments/assets/a189b6ab-bc38-482c-ad49-93e33d5c2331"/>

Finally, we need to configure the Asset Manager to be aware of our input configurations. Go to Project Settings and under `Game > Asset Manager` add an entry to `Primary Asset Types to Scan` like this:

<img width="1920" height="950" alt="image" src="https://github.com/user-attachments/assets/a927e4bd-dfe8-4f6f-b68e-9cc26e35dd9c"/>

Just be sure to update the `Directories` variable with the path that contains your input configuration Data Assets.

Aaand that's about it, a bit of setup boilerplate, but from now on, managing inputs and their configurations should be quite simple!
 
