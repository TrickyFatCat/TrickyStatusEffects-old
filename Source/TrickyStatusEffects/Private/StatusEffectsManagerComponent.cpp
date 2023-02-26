// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffect.h"

UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	UActorComponent::SetComponentTickEnabled(true);
}

void UStatusEffectsManagerComponent::TickComponent(float DeltaTime,
                                                   ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveEffects.Num() > 0 && GEngine)
	{
		for (int32 i = 0; i < ActiveEffects.Num(); ++i)
		{
			const UStatusEffect* Effect = ActiveEffects[i];
			FString Message = FString::Printf(
				TEXT("%s | %s | %s | %d/%d"),
				*Effect->GetName(),
				*UEnum::GetDisplayValueAsText(Effect->GetUniqueness()).ToString(),
				*FString::SanitizeFloat(Effect->GetRemainingTime(), 3),
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
			GEngine->AddOnScreenDebugMessage(i, DeltaTime, Color, Message);
		}
	}
}

void UStatusEffectsManagerComponent::AddEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator)
{
	if (!EffectClass)
	{
		return;
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
		Effect->ReActivateEffect();
		return;
	}

	CreateEffect(EffectClass, Instigator);
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClass(TSubclassOf<UStatusEffect> EffectClass)
{
	if (!EffectClass)
	{
		return false;
	}

	UStatusEffect* Effect = GetEffectOfClass(EffectClass)

	if (!IsValid(Effect))
	{
		return false;
	}

	Effect->FinishEffect();
	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass)
{
	bool bSuccess = false;
	
	if (!EffectClass || !HasEffectOfClass(EffectClass))
	{
		return bSuccess;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect) || Effect->GetClass() != EffectClass)
		{
			continue;
		}

		Effect->FinishEffect();
		bSuccess = true;
	}

	return bSuccess;
}

bool UStatusEffectsManagerComponent::RemoveEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                    AActor* Instigator)
{
	if (!EffectClass || !IsValid(Instigator))
	{
		return false;
	}

	UStatusEffect* Effect = GetEffectOfClassByInstigator(EffectClass, Instigator);

	if (!IsValid(Effect))
	{
		return false;
	}

	Effect->FinishEffect();
	return true;
}

bool UStatusEffectsManagerComponent::RemoveAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
                                                                         AActor* Instigator)
{
	bool bSuccess = false;

	if (!EffectClass || !IsValid(Instigator) || !HasEffectOfClassByInstigator(EffectClass, Instigator))
	{
		return bSuccess;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (Effect->GetClass() != EffectClass || Effect->GetInstigator() != Instigator)
		{
			continue;
		}

		Effect->FinishEffect();
		bSuccess = true;
	}

	return bSuccess;
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

	if (!EffectClass || !IsValid(Instigator))
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

void UStatusEffectsManagerComponent::CreateEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator)
{
	if (!EffectClass)
	{
		return;
	}

	UStatusEffect* NewEffect = NewObject<UStatusEffect>(this, EffectClass);

	if (!NewEffect)
	{
		return;
	}

	NewEffect->SetInstigator(Instigator);
	NewEffect->SetTargetActor(GetOwner());
	NewEffect->OnStatusEffectDeactivated.AddDynamic(this, &UStatusEffectsManagerComponent::HandleEffectDeactivation);
	ActiveEffects.Emplace(NewEffect);
	NewEffect->StartEffect();
}
