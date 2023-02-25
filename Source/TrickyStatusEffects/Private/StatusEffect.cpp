// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
	const UWorld* World = UObject::GetWorld();

	if (World && World->IsGameWorld() && !World->IsPreviewWorld() && !World->IsEditorWorld())
	{
		if (!StatusEffectData.bIsInfinite)
		{
			World->GetTimerManager().SetTimer(StatusEffectData.DurationTimerHandle,
			                                  this,
			                                  &UStatusEffect::DeactivateEffect,
			                                  StatusEffectData.Duration);
		}

		OnStatusEffectActivated.Broadcast(this);
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
}

void UStatusEffect::ActivateEffect_Implementation()
{
}

void UStatusEffect::DeactivateEffect_Implementation()
{
	this->BeginDestroy();
}
