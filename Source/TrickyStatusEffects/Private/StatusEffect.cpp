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


bool UStatusEffect::Activate(AActor* TargetActor, AActor* Instigator, UStatusEffectsManagerComponent* ManagerComponent)
{
	if (!IsValid(TargetActor) || !ManagerComponent)
	{
		return false;
	}

	const UWorld* World = UObject::GetWorld();

	if (World && !World->IsPreviewWorld())
	{
		if (!StatusEffectData.bIsInfinite)
		{
			StartTimer(World, StatusEffectData.Duration);
		}

		StatusEffectData.TargetActor = TargetActor;
		StatusEffectData.Instigator = Instigator;
		StatusEffectData.OwningManager = ManagerComponent;
		return HandleEffectActivation();
	}

	return false;
}

void UStatusEffect::Deactivate(const EDeactivationReason Reason)
{
	HandleEffectDeactivation(Reason);
	OnStatusEffectDeactivated.Broadcast(this);
	this->ConditionalBeginDestroy();
}

void UStatusEffect::ReActivate()
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();

	switch (StatusEffectData.DurationReActivationBehavior)
	{
	case EReActivationBehavior::Custom:
		break;

	case EReActivationBehavior::Add:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			const float DeltaDuration = GetRemainingTime();

			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration + DeltaDuration);
		}
		break;

	case EReActivationBehavior::Reset:
		if (TimerManager.IsTimerActive(StatusEffectData.DurationTimerHandle))
		{
			TimerManager.ClearTimer(StatusEffectData.DurationTimerHandle);
			StartTimer(World, StatusEffectData.Duration);
		}
		break;
	}

	if (StatusEffectData.bIsStackable)
	{
		switch (StatusEffectData.StacksReActivationBehavior)
		{
		case EReActivationBehavior::Add:
			AddStacks(StatusEffectData.DeltaStacks);
			break;

		case EReActivationBehavior::Reset:
			StatusEffectData.CurrentStacks = StatusEffectData.InitialStacks;
			break;
		}
	}

	HandleEffectReactivation(StatusEffectData.DurationReActivationBehavior);
	OnStatusEffectReactivated.Broadcast(this);
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

float UStatusEffect::GetElapsedTime() const
{
	float ElapsedTime = -1.f;

	if (StatusEffectData.bIsInfinite)
	{
		return ElapsedTime;
	}

	const UWorld* World = GetWorld();

	if (World)
	{
		ElapsedTime = World->GetTimerManager().GetTimerElapsed(StatusEffectData.DurationTimerHandle);
	}

	return ElapsedTime;
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
	OnStacksAdded.Broadcast(this, StatusEffectData.CurrentStacks, Amount);
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
	OnStacksRemoved.Broadcast(this, StatusEffectData.CurrentStacks, Amount);

	if (StatusEffectData.CurrentStacks == 0)
	{
		Deactivate(EDeactivationReason::Stacks);
	}

	return true;
}

bool UStatusEffect::HandleEffectActivation_Implementation()
{
	return true;
}

void UStatusEffect::HandleEffectDeactivation_Implementation(const EDeactivationReason Reason)
{
}

void UStatusEffect::HandleEffectReactivation_Implementation(const EReActivationBehavior ReactivationBehavior)
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
	TimerDelegate.BindUObject(this, &UStatusEffect::Deactivate, EDeactivationReason::Time);
	World->GetTimerManager().SetTimer(StatusEffectData.DurationTimerHandle,
	                                  TimerDelegate,
	                                  Duration,
	                                  false);
}
