// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffect.h"

UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	UActorComponent::SetComponentTickEnabled(false);

#if WITH_EDITORONLY_DATA
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
#endif
}

#if WITH_EDITORONLY_DATA
void UStatusEffectsManagerComponent::TickComponent(float DeltaTime,
                                                   ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	if (bDebugEnabled)
	{
		PrintDebugData(DeltaTime);
	}
}
#endif

UStatusEffect* UStatusEffectsManagerComponent::AddEffect(const TSubclassOf<UStatusEffect> EffectClass,
                                                         AActor* Instigator)
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
		Effect->AddStacks(1);
		Effect->ReStartEffect();
		return Effect;
	}

	return CreateEffect(EffectClass, Instigator);
}

bool UStatusEffectsManagerComponent::RemoveAllEffects()
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
		Effect->FinishEffect();
	}

	ActiveEffects.Empty();

	return true;
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClass(TSubclassOf<UStatusEffect> EffectClass,
                                                         const bool bIgnoreStacks)
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

	bIgnoreStacks ? Effect->FinishEffect() : Effect->RemoveStacks();
	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass)
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

		StatusEffect->FinishEffect();
		bSuccess = true;
	}

	return bSuccess;
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                     const AActor* Instigator,
                                                                     const bool bIgnoreStacks)
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

	bIgnoreStacks ? Effect->FinishEffect() : Effect->RemoveStacks();
	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                         const AActor* Instigator)
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

		StatusEffect->FinishEffect();
		bSuccess = true;
	}

	return bSuccess;
}

bool UStatusEffectsManagerComponent::RemoveEffectByObject(UStatusEffect* StatusEffect, const bool bIgnoreStacks)
{
	if (!IsValid(StatusEffect) || ActiveEffects.Num() == 0 || !ActiveEffects.Contains(StatusEffect))
	{
		return false;
	}

	bIgnoreStacks ? StatusEffect->FinishEffect() : StatusEffect->RemoveStacks();
	return true;
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
				*UEnum::GetDisplayValueAsText(Effect->GetRestartBehavior()).ToString(),
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

	if (!NewEffect)
	{
		return nullptr;
	}

	NewEffect->SetInstigator(Instigator);
	NewEffect->SetTargetActor(GetOwner());
	NewEffect->OnStatusEffectDeactivated.AddDynamic(this, &UStatusEffectsManagerComponent::HandleEffectDeactivation);
	ActiveEffects.Emplace(NewEffect);
	NewEffect->StartEffect();
	OnStatusEffectAdded.Broadcast(NewEffect, GetOwner(), Instigator);

	return NewEffect;
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
