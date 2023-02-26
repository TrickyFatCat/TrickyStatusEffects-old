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
				TEXT("%s | %hhd | %f | %d/%d"),
				*Effect->GetName(),
				Effect->GetEffectType(),
				Effect->GetRemainingTime(),
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

	if (Effect->GetUniqueness() == EStatusEffectUniqueness::PerTarget && HasEffectOfClass(EffectClass))
	{
		Effect = GetEffectOfClass(EffectClass);
		
		if (Effect->IsStackable())
		{
			Effect->AddStacks(1);
		}

		Effect->ReActivateEffect();
		return;
	}

	CreateEffect(EffectClass, Instigator);
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

	if (NewEffect->GetUniqueness() != EStatusEffectUniqueness::PerTarget)
	{
		NewEffect->SetInstigator(Instigator);
	}

	NewEffect->SetTargetActor(GetOwner());
	NewEffect->OnStatusEffectDeactivated.AddDynamic(this, &UStatusEffectsManagerComponent::HandleEffectDeactivation);
	ActiveEffects.Emplace(NewEffect);
	NewEffect->StartEffect();
}
