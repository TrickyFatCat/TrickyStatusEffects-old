// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffect.h"

#include "StatusEffectsManagerComponent.h"
#include "Engine/World.h"

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
		if (!StatusEffectData.bIsInfinite)
		{
			StartTimer(World, StatusEffectData.Duration);
		}

		HandleEffectActivation();
	}
}

void UStatusEffect::FinishEffect(const EDeactivationReason Reason)
{
	HandleEffectDeactivation(Reason);
	OnStatusEffectDeactivated.Broadcast(this);
	this->ConditionalBeginDestroy();
}

void UStatusEffect::ReStartEffect()
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();

	switch (StatusEffectData.ReActivationBehavior)
	{
	case EReactivationBehavior::Custom:
		break;

	case EReactivationBehavior::Add:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			const float DeltaDuration = GetRemainingTime();

			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration + DeltaDuration);
		}
		break;

	case EReactivationBehavior::Reset:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration);
		}
		break;
	}
	
	HandleEffectReactivation(StatusEffectData.ReActivationBehavior);
	OnStatusEffectReactivated.Broadcast(this);
}

void UStatusEffect::SetOwningManager(UStatusEffectsManagerComponent* OwningManager)
{
	if (IsValid(StatusEffectData.OwningManager))
	{
		return;
	}

	StatusEffectData.OwningManager = OwningManager;
}

float UStatusEffect::GetRemainingTime() const
{
	float RemainingTime = -1.f;

	if (StatusEffectData.bIsInfinite)
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
	HandleStacksIncrease(Amount);
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
	HandleStacksDecrease(Amount);
	OnStacksRemoved.Broadcast(this, Amount);

	if (StatusEffectData.CurrentStacks == 0)
	{
		FinishEffect(EDeactivationReason::Stacks);
	}

	return true;
}

void UStatusEffect::HandleEffectActivation_Implementation()
{
}

void UStatusEffect::HandleEffectDeactivation_Implementation(const EDeactivationReason Reason)
{
}

void UStatusEffect::HandleEffectReactivation_Implementation(const EReactivationBehavior ReactivationBehavior)
{
}

void UStatusEffect::HandleStacksIncrease_Implementation(const int32 Amount)
{ 
}

void UStatusEffect::HandleStacksDecrease_Implementation(const int32 Amount)
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
