# DanzmannInput

Data Asset-driven Enhanced Input wrapper. An Input Profile describes a Pawn's bindings in data. Any Component on the Pawn (or its Player State) registers native or Gameplay Ability bindings against that profile by implementing a thin interface.

Pawn-scoped by design. Controller-scope inputs (UI / menus) are intentionally out of scope -- wire those with vanilla `APlayerController::SetupInputComponent()`.

> Set `DefaultInputComponentClass=/Script/DanzmannInput.DanzmannEnhancedInputComponent` in `Config/DefaultInput.ini` so the Enhanced Input Component subclass is used on Pawns and Player Controllers.

## Concepts

- `UDanzmannInputProfile` (`UDataAsset`): one immutable description of a Pawn's inputs. Three arrays, each authored in the Editor:
  - `NativeInputActions`: `FDanzmannInputActionConfig` entries (Gameplay Tag <-> `UInputAction`) bound to direct C++ callbacks;
  - `GameplayAbilityInputActions`: `FDanzmannInputActionConfig` entries whose pressed/released events route through a Gameplay Ability System Component. Every tag listed is bound regardless of whether the matching Gameplay Ability has been granted yet;
  - `InputMappingContexts`: `FDanzmannInputMappingContextConfig` entries (IMC + priority) added to the local player on apply;
  - `FindNativeInputActionForGameplayTag()`/`FindGameplayAbilityInputActionForGameplayTag()` resolve a tag to its `UInputAction`, optionally logging a miss.
- `UDanzmannEnhancedInputComponent` (`UEnhancedInputComponent`): the orchestrator that applies profiles and tracks every binding handle it installs;
  - `ApplyInputProfile(InputProfile)`: adds the profile's Input Mapping Contexts to the local player, then scans the Owner, the Owner's Components, and (for Pawns) the bound Player State's Components for `IDanzmannInputBinderInterface` implementers, calling `BindInputActions(EIC, Profile)` on each. Idempotent -- it calls `ClearAppliedInputProfile()` first so runtime swaps don't stack Input Mapping Contexts or leak stale bindings. A null profile is a no-op and does not clear existing state;
  - `ClearAppliedInputProfile()`: removes every Input Mapping Context and binding installed by the last apply. Safe to call when nothing was applied. Invoked automatically at the top of `ApplyInputProfile()` and from `EndPlay()`, and exposed publicly so an external owner can wipe state during teardown;
  - `BindNativeInputAction<>()`/`BindGameplayAbilityInputAction<>()`: templated helpers that look the tag up in the profile, call `BindAction()`, and record the handle keyed by `(tag, trigger)`;
  - `RemoveInputActionBindings()` (by tag or by handle array) and `IsInputActionBound(Tag)` cover targeted unbinding and queries.
- `IDanzmannInputProfileSourceInterface`: implements on a Pawn-side Component to source the active profile (`GetInputProfile()`). The Enhanced Input Component does not exist until possession, so the owning Pawn scans its Components for this interface during `SetupPlayerInputComponent()` and applies the returned profile itself -- implementers only source, they do not apply;
- `IDanzmannInputBinderInterface`: implement on any Component (or the Pawn) that wants to register bindings. `BindInputActions(EIC, Profile)` is called for each implementer found in the apply scan.
- Gameplay Tags (`Danzmann::GameplayTags` namespace): the plugin ships a starter set identifying common Input Actions -- `InputAction_{Move, Look_Mouse, Look_Gamepad, Jump, Run, Sprint, Crouch, Aim, Fire}`. Profiles pair these (or your own) tags with `UInputAction` assets; gameplay code refers to inputs by tag instead of by direct asset reference.

## Usage

```cpp
// 1. A Component on the Pawn sources the profile.
UCLASS()
class UMyPawnDataComponent : public UPawnComponent, public IDanzmannInputProfileSourceInterface
{
    GENERATED_BODY()

    virtual const UDanzmannInputProfile* GetInputProfile() const override { return InputProfile; }

    UPROPERTY(EditAnywhere) TObjectPtr<UDanzmannInputProfile> InputProfile;
};

// 2. Any Component registers bindings against the profile.
UCLASS()
class UMyMoverComponent : public UPawnComponent, public IDanzmannInputBinderInterface
{
    GENERATED_BODY()

    virtual void BindInputActions(UDanzmannEnhancedInputComponent* EIC, const UDanzmannInputProfile* Profile) override
    {
        EIC->BindNativeInputAction(Profile, Danzmann::GameplayTags::InputAction_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
    }

    void OnMove(const FInputActionValue& Value)
    {
        // ...
    }
};

// 3. The Pawn applies the profile in SetupPlayerInputComponent().
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UDanzmannEnhancedInputComponent* EIC = Cast<UDanzmannEnhancedInputComponent>(PlayerInputComponent))
    {
        TArray<UActorComponent*> Components;
        GetComponents(Components);
        for (UActorComponent* Component : Components)
        {
            if (Component->Implements<UDanzmannInputProfileSourceInterface>())
            {
                EIC->ApplyInputProfile(Cast<IDanzmannInputProfileSourceInterface>(Component)->GetInputProfile());
                break;
            }
        }
    }
}
```

To swap profiles at runtime, call `ApplyInputProfile(NewProfile)` again -- the clear-then-add pass tears the previous profile down for you.

## Apply pipeline at a glance

```
AMyPawn::SetupPlayerInputComponent()      -> find IDanzmannInputProfileSourceInterface, ApplyInputProfile(Profile)
  └─ EIC::ApplyInputProfile(Profile)
       ├─ ClearAppliedInputProfile()      -> remove prior Input Mapping Contexts + bindings (idempotent)
       ├─ add Profile->InputMappingContexts to the local player subsystem (with priority)
       ├─ Owner                           -> BindInputActions() if it implements IDanzmannInputBinderInterface
       ├─ Owner's Components              -> BindInputActions() on each implementer
       └─ Pawn's PlayerState Components   -> BindInputActions() on each implementer (skipped if PlayerState not bound yet)
EndPlay()                                 -> ClearAppliedInputProfile()
```

---

Part of the Danzmann plugin suite. See also [`DanzmannExperiences`](https://github.com/iVcente/DanzmannExperiences), [`DanzmannMovement`](https://github.com/iVcente/DanzmannMovement), and [`DanzmannAbilities`](https://github.com/iVcente/DanzmannAbilities).
