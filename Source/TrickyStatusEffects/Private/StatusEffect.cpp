// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
}

void UStatusEffect::PostInitProperties()
{
	UObject::PostInitProperties();
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


void UStatusEffect::StartEffect()
{
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

void UStatusEffect::FinishEffect()
{
	DeactivateEffect();
	this->ConditionalBeginDestroy();
}

void UStatusEffect::ReActivateEffect()
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();

	switch (StatusEffectData.ReActivationBehavior)
	{
	case EReActivationBehavior::Custom:
		break;

	case EReActivationBehavior::Add:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			const float DeltaDuration = GetRemainingTime();
			
			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			TimerManager.SetTimer(StatusEffectData.DurationTimerHandle,
			                      this,
			                      &UStatusEffect::FinishEffect,
			                      StatusEffectData.Duration + DeltaDuration);
		}
		break;

	case EReActivationBehavior::Reset:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			TimerManager.SetTimer(StatusEffectData.DurationTimerHandle,
			                      this,
			                      &UStatusEffect::FinishEffect,
			                      StatusEffectData.Duration);
		}
		break;
	}
}

float UStatusEffect::GetRemainingTime() const
{
	float RemainingTime = -1.f;

	if (StatusEffectData.bInfiniteDuration)
	{
		return RemainingTime;
	}

	const UWorld* World = GetWorld();

	if (World)
	{
		RemainingTime = World->GetTimerManager().GetTimerRemaining(StatusEffectData.DurationTimerHandle);
	}

	return RemainingTime;
}

bool UStatusEffect::AddStacks(int32 Amount)
{
	if (StatusEffectData.CurrentStacks >= StatusEffectData.MaxStacks || !StatusEffectData.bIsStackable)
	{
		return false;
	}

	const int32 Delta = StatusEffectData.MaxStacks - StatusEffectData.CurrentStacks;

	if (Amount > Delta)
	{
		Amount = Delta;
	}

	StatusEffectData.CurrentStacks = FMath::Min(StatusEffectData.CurrentStacks + Amount, StatusEffectData.MaxStacks);
	OnStacksAdded.Broadcast(this, Amount);
	return true;
}

bool UStatusEffect::RemoveStacks(int32 Amount)
{
	if (StatusEffectData.CurrentStacks <= 0 || !StatusEffectData.bIsStackable)
	{
		return false;
	}

	if (Amount > StatusEffectData.CurrentStacks)
	{
		Amount = StatusEffectData.CurrentStacks;
	}

	StatusEffectData.CurrentStacks = FMath::Max(StatusEffectData.CurrentStacks - Amount, 0);
	OnStacksRemoved.Broadcast(this, Amount);

	if (StatusEffectData.CurrentStacks == 0)
	{
		FinishEffect();
	}

	return true;
}

void UStatusEffect::ActivateEffect_Implementation()
{
}

void UStatusEffect::DeactivateEffect_Implementation()
{
}
