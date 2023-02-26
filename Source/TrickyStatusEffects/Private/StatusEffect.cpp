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

void UStatusEffect::SetInstigator(AActor* Instigator)
{
	StatusEffectData.Instigator = Instigator;
}

void UStatusEffect::SetTargetActor(AActor* TargetActor)
{
	StatusEffectData.TargetActor = TargetActor;
}

bool UStatusEffect::GetIsUnique() const
{
	return StatusEffectData.bIsUnique;
}

AActor* UStatusEffect::GetInstigator() const
{
	return StatusEffectData.Instigator;
}

EStatusEffectType UStatusEffect::GetEffectType() const
{
	return StatusEffectData.EffectType;
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

bool UStatusEffect::IsStackable() const
{
	return StatusEffectData.MaxStacks > 1;
}

int32 UStatusEffect::GetMaxStacks() const
{
	return StatusEffectData.MaxStacks;
}

int32 UStatusEffect::GetCurrentStacks() const
{
	return StatusEffectData.CurrentStacks;
}

bool UStatusEffect::AddStacks(int32 Amount)
{
	if (StatusEffectData.CurrentStacks >= StatusEffectData.MaxStacks || StatusEffectData.MaxStacks == 1)
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
	if (StatusEffectData.CurrentStacks <= 0 || StatusEffectData.MaxStacks == 1)
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
