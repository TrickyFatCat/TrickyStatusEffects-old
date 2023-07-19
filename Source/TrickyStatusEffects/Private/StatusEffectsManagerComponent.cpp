// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "GameFramework/Actor.h"
#include "StatusEffect.h"
#include "Engine/Engine.h"

UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	UActorComponent::SetComponentTickEnabled(false);

#if WITH_EDITORONLY_DATA
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
#endif
}

void UStatusEffectsManagerComponent::TickComponent(float DeltaTime,
                                                   ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if WITH_EDITORONLY_DATA
	if (bDebugEnabled)
	{
		PrintDebugData(DeltaTime);
	}
#endif
}

UStatusEffect* UStatusEffectsManagerComponent::ApplyEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator)
{
	if (!EffectClass)
	{
		return nullptr;
	}

	UStatusEffect* Effect = EffectClass.GetDefaultObject();
	const EStatusEffectUniqueness EffectUniqueness = Effect->GetUniqueness();

	switch (EffectUniqueness)
	{
	case EStatusEffectUniqueness::Normal:
		break;

	case EStatusEffectUniqueness::PerInstigator:
		Effect = GetEffectOfClassByInstigator(EffectClass, Instigator);
		break;

	case EStatusEffectUniqueness::PerTarget:
		Effect = GetEffectOfClass(EffectClass);
		break;
	}

	if (IsValid(Effect) && EffectUniqueness != EStatusEffectUniqueness::Normal)
	{
		Effect->ReActivate();
		return Effect;
	}

	return CreateEffect(EffectClass, Instigator);
}

bool UStatusEffectsManagerComponent::RemoveAllEffects(const bool bCustomReason)
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}

		Effect->OnStatusEffectDeactivated.Clear();
		FinishEffect(Effect, bCustomReason);
	}

	ActiveEffects.Empty();

	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllPositiveEffects(const bool bCustomReason)
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Positive)
		{
			continue;
		}

		Effect->OnStatusEffectDeactivated.Clear();
		FinishEffect(Effect, bCustomReason);
	}

	ActiveEffects.Empty();

	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllNegativeEffects(const bool bCustomReason)
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Negative)
		{
			continue;
		}

		Effect->OnStatusEffectDeactivated.Clear();
		FinishEffect(Effect, bCustomReason);
	}

	ActiveEffects.Empty();

	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllNeutralEffects(const bool bCustomReason)
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Neutral)
		{
			continue;
		}

		Effect->OnStatusEffectDeactivated.Clear();
		FinishEffect(Effect, bCustomReason);
	}

	ActiveEffects.Empty();

	return true;
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClass(TSubclassOf<UStatusEffect> EffectClass,
                                                         const bool bCustomReason,
                                                         const bool bRemoveAllStacks,
                                                         const int32 StacksAmount)
{
	if (!EffectClass)
	{
		return false;
	}

	UStatusEffect* Effect = GetEffectOfClass(EffectClass);

	if (!IsValid(Effect))
	{
		return false;
	}

	return FinishEffect(Effect, bCustomReason, bRemoveAllStacks, StacksAmount);
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass,
                                                             const bool bCustomReason)
{
	bool bSuccess = false;

	if (!EffectClass || !HasEffectOfClass(EffectClass))
	{
		return bSuccess;
	}

	const int32 Number = GetNumberOfEffectsOfClass(EffectClass);

	if (Number <= 0)
	{
		return bSuccess;
	}

	for (int32 i = 0; i < Number; ++i)
	{
		UStatusEffect* StatusEffect = GetEffectOfClass(EffectClass);

		if (!IsValid(StatusEffect))
		{
			continue;
		}

		FinishEffect(StatusEffect, bCustomReason);
		bSuccess = true;
	}

	return bSuccess;
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                     const AActor* Instigator,
                                                                     const bool bCustomReason,
                                                                     const bool bRemoveAllStacks,
                                                                     const int StacksAmount)
{
	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return false;
	}

	UStatusEffect* Effect = GetEffectOfClassByInstigator(EffectClass, Instigator);

	if (!IsValid(Effect))
	{
		return false;
	}

	return FinishEffect(Effect, bCustomReason, bRemoveAllStacks, StacksAmount);
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                         const AActor* Instigator,
                                                                         const bool bCustomReason)
{
	bool bSuccess = false;

	if (!EffectClass || ActiveEffects.Num() == 0 || !HasEffectOfClassByInstigator(EffectClass, Instigator))
	{
		return bSuccess;
	}

	const int32 Number = GetNumberOfEffectsOfClassByInstigator(EffectClass, Instigator);

	if (Number <= 0)
	{
		return bSuccess;
	}

	for (int32 i = 0; i < Number; ++i)
	{
		UStatusEffect* StatusEffect = GetEffectOfClassByInstigator(EffectClass, Instigator);

		if (!IsValid(StatusEffect))
		{
			continue;
		}

		FinishEffect(StatusEffect, bCustomReason);
		bSuccess = true;
	}

	return bSuccess;
}

bool UStatusEffectsManagerComponent::RemoveEffectByObject(UStatusEffect* StatusEffect,
                                                          const bool bCustomReason,
                                                          const bool bRemoveAllStacks,
                                                          const int32 StacksAmount)
{
	if (!IsValid(StatusEffect) || ActiveEffects.Num() == 0 || !ActiveEffects.Contains(StatusEffect))
	{
		return false;
	}

	return FinishEffect(StatusEffect, bCustomReason, bRemoveAllStacks, StacksAmount);
}

bool UStatusEffectsManagerComponent::GetAllActiveEffects(TArray<UStatusEffect*>& Effects) const
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	Effects = ActiveEffects;
	return true;
}

bool UStatusEffectsManagerComponent::GetAllPositiveEffects(TArray<UStatusEffect*>& PositiveEffects) const
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	PositiveEffects.Empty();

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Positive)
		{
			continue;
		}

		PositiveEffects.Emplace(Effect);
	}

	return PositiveEffects.Num() > 0;
}

bool UStatusEffectsManagerComponent::GetAllNegativeEffects(TArray<UStatusEffect*>& NegativeEffects) const
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	NegativeEffects.Empty();

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Negative)
		{
			continue;
		}

		NegativeEffects.Emplace(Effect);
	}

	return NegativeEffects.Num() > 0;
}

bool UStatusEffectsManagerComponent::GetAllNeutralEffects(TArray<UStatusEffect*>& NeutralEffects) const
{
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}
	if (ActiveEffects.Num() == 0)
	{
		return false;
	}

	NeutralEffects.Empty();

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetEffectType() != EStatusEffectType::Negative)
		{
			continue;
		}

		NeutralEffects.Emplace(Effect);
	}

	return NeutralEffects.Num() > 0;
}

bool UStatusEffectsManagerComponent::HasEffectOfClass(const TSubclassOf<UStatusEffect> EffectClass)
{
	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return false;
	}

	return IsValid(GetEffectOfClass(EffectClass));
}

UStatusEffect* UStatusEffectsManagerComponent::GetEffectOfClass(TSubclassOf<UStatusEffect> EffectClass)
{
	UStatusEffect* StatusEffect = nullptr;

	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return StatusEffect;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}
		if (EffectClass == Effect->GetClass())
		{
			StatusEffect = Effect;
			break;
		}
	}
	return StatusEffect;
}

bool UStatusEffectsManagerComponent::GetAllEffectsOfClass(const TSubclassOf<UStatusEffect> EffectClass,
                                                          TArray<UStatusEffect*>& Effects)
{
	if (!EffectClass || ActiveEffects.Num() == 0 || !HasEffectOfClass(EffectClass))
	{
		return false;
	}

	Effects.Empty();

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetClass() != EffectClass)
		{
			continue;
		}

		Effects.Emplace(Effect);
	}

	return Effects.Num() > 0;
}

bool UStatusEffectsManagerComponent::HasEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                  const AActor* Instigator)
{
	if (!EffectClass || !IsValid(Instigator))
	{
		return false;
	}

	return IsValid(GetEffectOfClassByInstigator(EffectClass, Instigator));
}

UStatusEffect* UStatusEffectsManagerComponent::GetEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                            const AActor* Instigator)
{
	UStatusEffect* StatusEffect = nullptr;

	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return StatusEffect;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}
		if (EffectClass == Effect->GetClass() && Effect->GetInstigator() == Instigator)
		{
			StatusEffect = Effect;
			break;
		}
	}

	return StatusEffect;
}

bool UStatusEffectsManagerComponent::GetAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                      const AActor* Instigator,
                                                                      TArray<UStatusEffect*>& Effects)
{
	if (!EffectClass || ActiveEffects.Num() == 0 || !HasEffectOfClassByInstigator(EffectClass, Instigator))
	{
		return false;
	}

	Effects.Empty();

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetClass() != EffectClass || Effect->GetInstigator() != Instigator)
		{
			continue;
		}

		Effects.Emplace(Effect);
	}

	return Effects.Num() > 0;
}

void UStatusEffectsManagerComponent::HandleEffectDeactivation(UStatusEffect* StatusEffect)
{
	ActiveEffects.RemoveSingle(StatusEffect);
}

#if WITH_EDITORONLY_DATA
void UStatusEffectsManagerComponent::PrintDebugData(const float DeltaTime)
{
	if (ActiveEffects.Num() > 0 && GEngine)
	{
		for (int32 i = 0; i < ActiveEffects.Num(); ++i)
		{
			const UStatusEffect* Effect = ActiveEffects[i];
			const float RemainingTime = FMath::RoundToInt(Effect->GetRemainingTime() * 100) / 100.f;
			const FString InstigatorName = IsValid(Effect->GetInstigator())
				                               ? Effect->GetInstigator()->GetName()
				                               : "NONE";
			FString Message = FString::Printf(
				TEXT(
					"| Effect: %s |\n| Instigator: %s |\n| Uniqueness: %s |\n| Restart Behavior: %s |\n| Time Remaining: %s |\n| Stacks: %d/%d|"),
				*Effect->GetName(),
				*InstigatorName,
				*UEnum::GetDisplayValueAsText(Effect->GetUniqueness()).ToString(),
				*UEnum::GetDisplayValueAsText(Effect->GetReactivationBehavior()).ToString(),
				*FString::SanitizeFloat(RemainingTime, 2),
				Effect->GetCurrentStacks(),
				Effect->GetMaxStacks());

			FColor Color;

			switch (Effect->GetEffectType())
			{
			case EStatusEffectType::Positive:
				Color = FColor::Emerald;
				break;

			case EStatusEffectType::Negative:
				Color = FColor::Orange;
				break;
			}

			GEngine->AddOnScreenDebugMessage(i, DeltaTime, Color, Message, true, FVector2D{1.25f, 1.25f});
		}
	}
}
#endif

UStatusEffect* UStatusEffectsManagerComponent::CreateEffect(const TSubclassOf<UStatusEffect> EffectClass,
                                                            AActor* Instigator)
{
	if (!EffectClass)
	{
		return nullptr;
	}

	UStatusEffect* NewEffect = NewObject<UStatusEffect>(this, EffectClass);

	if (!NewEffect || !NewEffect->Activate(GetOwner(), Instigator, this))
	{
		return nullptr;
	}

	ActiveEffects.Emplace(NewEffect);
	NewEffect->OnStatusEffectDeactivated.AddDynamic(this, &UStatusEffectsManagerComponent::HandleEffectDeactivation);
	OnStatusEffectApplied.Broadcast(NewEffect, GetOwner(), Instigator);

	return NewEffect;
}

bool UStatusEffectsManagerComponent::FinishEffect(UStatusEffect* Effect,
                                                  const bool bCustomReason,
                                                  const bool bRemoveAllStacks,
                                                  const int32 StacksAmount)
{
	if (bRemoveAllStacks)
	{
		Effect->Deactivate(bCustomReason ? EDeactivationReason::Custom : EDeactivationReason::Remove);
		return true;
	}

	return Effect->RemoveStacks(StacksAmount);
}

int32 UStatusEffectsManagerComponent::GetNumberOfEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass) const
{
	int32 Number = 0;

	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return Number;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetClass() != EffectClass)
		{
			continue;
		}

		++Number;
	}

	return Number;
}

int32 UStatusEffectsManagerComponent::GetNumberOfEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                            const AActor* Instigator)
{
	int32 Number = 0;

	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return Number;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetClass() != EffectClass || Effect->GetInstigator() != Instigator)
		{
			continue;
		}

		++Number;
	}

	return Number;
}
