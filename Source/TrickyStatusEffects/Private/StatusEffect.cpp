// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
}

void UStatusEffect::PostInitProperties()
{
	UObject::PostInitProperties();

	const UWorld* World = UObject::GetWorld();

	if (World && !World->IsPreviewWorld())
	{
		if (!StatusEffectData.bInfiniteDuration)
		{
			World->GetTimerManager().SetTimer(StatusEffectData.DurationTimerHandle,
			                                  this,
			                                  &UStatusEffect::FinishEffect,
			                                  StatusEffectData.Duration);
		}

		ActivateEffect();
	}
}

void UStatusEffect::BeginDestroy()
{
	UObject::BeginDestroy();

	const UWorld* World = GetWorld();

	if (World)
	{
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	}

	OnStatusEffectDeactivated.Broadcast(this);
	OnStatusEffectDeactivated.Clear();
}

void UStatusEffect::SetInstigator(AActor* Instigator)
{
	StatusEffectData.Instigator = Instigator;
}

void UStatusEffect::FinishEffect()
{
	DeactivateEffect();
	this->ConditionalBeginDestroy();
}

bool UStatusEffect::GetIsUnique() const
{
	return StatusEffectData.bIsUnique;
}

void UStatusEffect::ActivateEffect_Implementation()
{
}

void UStatusEffect::DeactivateEffect_Implementation()
{
}