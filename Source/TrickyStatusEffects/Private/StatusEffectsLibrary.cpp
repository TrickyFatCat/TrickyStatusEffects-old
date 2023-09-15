// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "StatusEffectsLibrary.h"

#include "StatusEffectsManagerComponent.h"
#include "GameFramework/Actor.h"


UStatusEffectsManagerComponent* UStatusEffectsLibrary::GetStatusEffectsManager(AActor* TargetActor)
{
	if (!IsValid(TargetActor))
	{
		return nullptr;
	}

	return TargetActor->FindComponentByClass<UStatusEffectsManagerComponent>();
}

UStatusEffect* UStatusEffectsLibrary::ApplyStatusEffect(const TSubclassOf<UStatusEffect> EffectClass,
                                                        AActor* TargetActor,
                                                        AActor* Instigator)
{
	if (!IsValid(TargetActor))
	{
		return nullptr;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return nullptr;
	}

	return StatusEffectsManagerComponent->ApplyEffect(EffectClass, Instigator);
}

bool UStatusEffectsLibrary::RemoveAllStatusEffects(AActor* TargetActor, const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllEffects(bCustomReason);
}

bool UStatusEffectsLibrary::RemoveAllPositiveStatusEffects(AActor* TargetActor, const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllPositiveEffects(bCustomReason);
}

bool UStatusEffectsLibrary::RemoveAllNegativeStatusEffects(AActor* TargetActor, const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllNegativeEffects(bCustomReason);
}

bool UStatusEffectsLibrary::RemoveAllNeutralStatusEffects(AActor* TargetActor, const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllNegativeEffects(bCustomReason);
}


bool UStatusEffectsLibrary::RemoveStatusEffectOfClass(AActor* TargetActor,
                                                      TSubclassOf<UStatusEffect> EffectClass,
                                                      const bool bCustomReason,
                                                      const bool bRemoveAllStacks,
                                                      const int32 StacksAmount)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveEffectOfClass(EffectClass,
	                                                          bCustomReason,
	                                                          bRemoveAllStacks,
	                                                          StacksAmount);
}

bool UStatusEffectsLibrary::RemoveAllStatusEffectsOfClass(AActor* TargetActor,
                                                          TSubclassOf<UStatusEffect> EffectClass,
                                                          const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllEffectsOfClass(EffectClass, bCustomReason);
}

bool UStatusEffectsLibrary::RemoveStatusEffectOfClassByInstigator(AActor* TargetActor,
                                                                  TSubclassOf<UStatusEffect> EffectClass,
                                                                  const AActor* Instigator,
                                                                  const bool bCustomReason,
                                                                  const bool bRemoveAllStacks,
                                                                  const int32 StacksAmount)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveEffectOfClassByInstigator(EffectClass,
	                                                                      Instigator,
	                                                                      bCustomReason,
	                                                                      bRemoveAllStacks,
	                                                                      StacksAmount);
}

bool UStatusEffectsLibrary::RemoveAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
                                                                      TSubclassOf<UStatusEffect> EffectClass,
                                                                      const AActor* Instigator,
                                                                      const bool bCustomReason)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveAllEffectsOfClassByInstigator(EffectClass, Instigator, bCustomReason);
}

bool UStatusEffectsLibrary::RemoveStatusEffectByObject(AActor* TargetActor,
                                                       UStatusEffect* StatusEffect,
                                                       const bool bCustomReason,
                                                       const bool bRemoveAllStacks,
                                                       const int32 StacksAmount)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->RemoveEffectByObject(StatusEffect,
	                                                           bCustomReason,
	                                                           bRemoveAllStacks,
	                                                           StacksAmount);
}

bool UStatusEffectsLibrary::GetAllActiveStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& ActiveEffects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllActiveEffects(ActiveEffects);
}

bool UStatusEffectsLibrary::GetAllPositiveStatusEffects(AActor* TargetActor,
                                                        TArray<UStatusEffect*>& PositiveEffects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllPositiveEffects(PositiveEffects);
}

bool UStatusEffectsLibrary::GetAllNegativeStatusEffects(AActor* TargetActor,
                                                        TArray<UStatusEffect*>& NegativeEffects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllNegativeEffects(NegativeEffects);
}

bool UStatusEffectsLibrary::GetAllNeutralStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& NeutralEffects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	const UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllNegativeEffects(NeutralEffects);
}

bool UStatusEffectsLibrary::HasStatusEffectOfClass(AActor* TargetActor, TSubclassOf<UStatusEffect> EffectClass)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->HasEffectOfClass(EffectClass);
}

UStatusEffect* UStatusEffectsLibrary::GetStatusEffectOfClass(AActor* TargetActor,
                                                             TSubclassOf<UStatusEffect> EffectClass)
{
	if (!IsValid(TargetActor))
	{
		return nullptr;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return nullptr;
	}

	return StatusEffectsManagerComponent->GetEffectOfClass(EffectClass);
}

bool UStatusEffectsLibrary::GetAllStatusEffectsOfClass(AActor* TargetActor,
                                                       const TSubclassOf<UStatusEffect> EffectClass,
                                                       TArray<UStatusEffect*>& Effects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllEffectsOfClass(EffectClass, Effects);
}

bool UStatusEffectsLibrary::HasStatusEffectOfClassByInstigator(AActor* TargetActor,
                                                               TSubclassOf<UStatusEffect> EffectClass,
                                                               const AActor* Instigator)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->HasEffectOfClassByInstigator(EffectClass, Instigator);
}

UStatusEffect* UStatusEffectsLibrary::GetStatusEffectOfClassByInstigator(AActor* TargetActor,
                                                                         TSubclassOf<UStatusEffect> EffectClass,
                                                                         const AActor* Instigator)
{
	if (!IsValid(TargetActor))
	{
		return nullptr;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return nullptr;
	}

	return StatusEffectsManagerComponent->GetEffectOfClassByInstigator(EffectClass, Instigator);
}

bool UStatusEffectsLibrary::GetAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
                                                                   TSubclassOf<UStatusEffect> EffectClass,
                                                                   const AActor* Instigator,
                                                                   TArray<UStatusEffect*>& Effects)
{
	if (!IsValid(TargetActor))
	{
		return false;
	}

	UStatusEffectsManagerComponent* StatusEffectsManagerComponent = GetStatusEffectsManager(TargetActor);

	if (!StatusEffectsManagerComponent)
	{
		return false;
	}

	return StatusEffectsManagerComponent->GetAllEffectsOfClassByInstigator(EffectClass, Instigator, Effects);
}
