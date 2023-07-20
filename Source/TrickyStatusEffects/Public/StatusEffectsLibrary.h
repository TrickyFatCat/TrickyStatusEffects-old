// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatusEffect.h"

#include "StatusEffectsLibrary.generated.h"

class UStatusEffectsManagerComponent;
class AActor;

/**
 * A utility library for applying, removing, and getting status effects.
 */
UCLASS()
class TRICKYSTATUSEFFECTS_API UStatusEffectsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**Returns the status effects manager component of the given actor if it was found.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffectsManagerComponent* GetStatusEffectsManager(AActor* TargetActor);

	/**Applies status effect or reapplies already applied effect.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* ApplyStatusEffect(const TSubclassOf<UStatusEffect> EffectClass,
	                                        AActor* TargetActor,
	                                        AActor* Instigator);

	/**Removes all status effects regardless of time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	/**Removes all positive status effects regardless of time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllPositiveStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	/**Removes all negative status effects regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllNegativeStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	/**Removes all neutral status effects regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllNeutralStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	/**Removes the first found status effect of a given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectOfClass(AActor* TargetActor,
	                                      TSubclassOf<UStatusEffect> EffectClass,
	                                      const bool bCustomReason = false,
	                                      const bool bRemoveAllStacks = true,
	                                      const int32 StacksAmount = 1);

	/**Removes all status effects of a given class regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffectsOfClass(AActor* TargetActor,
	                                          TSubclassOf<UStatusEffect> EffectClass,
	                                          const bool bCustomReason = false);

	/**Removes the first found status effect of a given class of specific instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                                  TSubclassOf<UStatusEffect> EffectClass,
	                                                  const AActor* Instigator,
	                                                  const bool bCustomReason = false,
	                                                  const bool bRemoveAllStacks = true,
	                                                  const int32 StacksAmount = 1);

	/**Removes all status effects of a given class of specific instigator regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
	                                                      TSubclassOf<UStatusEffect> EffectClass,
	                                                      const AActor* Instigator,
	                                                      const bool bCustomReason = false);

	/**Removes a specific instance of the status effect.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectByObject(AActor* TargetActor,
	                                       UStatusEffect* StatusEffect,
	                                       const bool bCustomReason = false,
	                                       const bool bRemoveAllStacks = true,
	                                       const int32 StacksAmount = 1);

	/**Returns all active status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllActiveStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& ActiveEffects);

	/**Returns all active positive status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllPositiveStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& PositiveEffects);

	/**Returns all active negative status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllNegativeStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& NegativeEffects);

	/**Returns all active neutral status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllNeutralStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& NeutralEffects);

	/**Checks if the status effect of a given class is active.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool HasStatusEffectOfClass(AActor* TargetActor, TSubclassOf<UStatusEffect> EffectClass);

	/**Returns the status effect instance of the given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* GetStatusEffectOfClass(AActor* TargetActor, TSubclassOf<UStatusEffect> EffectClass);

	/**Returns all status effect instances of the given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool GetAllStatusEffectsOfClass(AActor* TargetActor,
	                                TSubclassOf<UStatusEffect> EffectClass,
	                                TArray<UStatusEffect*>& Effects);

	/**Checks if the status effect of a given class and instigator is active.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool HasStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                               TSubclassOf<UStatusEffect> EffectClass,
	                                               const AActor* Instigator);


	/**Returns the status effect instance of the given class and instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* GetStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                                         TSubclassOf<UStatusEffect> EffectClass,
	                                                         const AActor* Instigator);

	/**Returns all status effect instances of the given class and instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool GetAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
	                                            TSubclassOf<UStatusEffect> EffectClass,
	                                            const AActor* Instigator,
	                                            TArray<UStatusEffect*>& Effects);
};
