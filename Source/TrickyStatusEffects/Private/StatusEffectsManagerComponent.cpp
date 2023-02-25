// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsManagerComponent.h"

#include "StatusEffect.h"

UStatusEffectsManagerComponent::UStatusEffectsManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	UActorComponent::SetComponentTickEnabled(false);
}

void UStatusEffectsManagerComponent::AddEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator)
{
	if (!EffectClass)
	{
		return;
	}

	UStatusEffect* NewEffect = EffectClass.GetDefaultObject();

	if (NewEffect->GetIsUnique() && HasEffectOfClass(EffectClass))
	{
		UE_LOG(LogTemp, Error, TEXT("Unique effect"));
		return;
	}

	NewEffect = NewObject<UStatusEffect>(this, EffectClass);

	if (!NewEffect)
	{
		return;
	}

	NewEffect->SetInstigator(Instigator);
	NewEffect->OnStatusEffectDeactivated.AddDynamic(this, &UStatusEffectsManagerComponent::HandleEffectDeactivation);
	ActiveEffects.Emplace(NewEffect);
}

bool UStatusEffectsManagerComponent::HasEffectOfClass(const TSubclassOf<UStatusEffect> EffectClass)
{
	bool bHasEffect = false;

	if (!EffectClass || ActiveEffects.Num() == 0)
	{
		return bHasEffect;
	}

	for (const auto& Effect : ActiveEffects)
	{
		if (!IsValid(Effect))
		{
			continue;
		}
		if (EffectClass == Effect->GetClass())
		{
			bHasEffect = true;
			break;
		}
	}

	return bHasEffect;
}

void UStatusEffectsManagerComponent::HandleEffectDeactivation(UStatusEffect* StatusEffect)
{
	ActiveEffects.RemoveSingle(StatusEffect);
}
