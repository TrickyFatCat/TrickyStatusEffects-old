// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffect.h"

UStatusEffect::UStatusEffect()
{
}

void UStatusEffect::PostInitProperties()
{
	UObject::PostInitProperties();

	if (IsStackable())
	{
		StatusEffectData.CurrentStacks = FMath::Clamp(StatusEffectData.InitialStacks, 1, StatusEffectData.MaxStacks);
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

	OnStatusEffectDeactivated.Clear();
}


void UStatusEffect::StartEffect()
{
	const UWorld* World = UObject::GetWorld();

	if (World && !World->IsPreviewWorld())
	{
		if (!StatusEffectData.bInfiniteDuration)
		{
			StartTimer(World, StatusEffectData.Duration);
		}

		ActivateEffect();
	}
}

void UStatusEffect::FinishEffect(const EDeactivationReason Reason)
{
	DeactivateEffect(Reason);
	OnStatusEffectDeactivated.Broadcast(this);
	this->ConditionalBeginDestroy();
}

void UStatusEffect::ReStartEffect()
{
	const UWorld* World = GetWorld();

	ReActivateEffect();

	if (!World)
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();

	switch (StatusEffectData.ReStartBehavior)
	{
	case ERestartBehavior::Custom:
		break;

	case ERestartBehavior::Add:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			const float DeltaDuration = GetRemainingTime();

			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration + DeltaDuration);
		}
		break;

	case ERestartBehavior::Reset:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration);
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
		FinishEffect(EDeactivationReason::Stacks);
	}

	return true;
}

void UStatusEffect::ActivateEffect_Implementation()
{
}

void UStatusEffect::DeactivateEffect_Implementation(const EDeactivationReason Reason)
{
}

void UStatusEffect::ReActivateEffect_Implementation()
{
}

void UStatusEffect::StacksIncreased_Implementation(const int32 Amount)
{
}

void UStatusEffect::StacksDecreased_Implementation(const int32 Amount)
{
}

void UStatusEffect::StartTimer(const UWorld* World, const float Duration)
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &UStatusEffect::FinishEffect, EDeactivationReason::Time);
	World->GetTimerManager().SetTimer(StatusEffectData.DurationTimerHandle,
	                                  TimerDelegate,
	                                  Duration,
	                                  false);
}
