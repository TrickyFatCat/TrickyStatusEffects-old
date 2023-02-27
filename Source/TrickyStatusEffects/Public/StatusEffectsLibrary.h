// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StatusEffect.h"
#include "StatusEffectsLibrary.generated.h"

// class UStatusEffect;
class UStatusEffectsManagerComponent;
class AActor;

/**
 * 
 */
UCLASS()
class TRICKYSTATUSEFFECTS_API UStatusEffectsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffectsManagerComponent* GetStatusEffectsManager(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* AddStatusEffect(const TSubclassOf<UStatusEffect> EffectClass,
	                                      AActor* TargetActor,
	                                      AActor* Instigator,
	                                      const int32 StacksAmount);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllPositiveStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllNegativeStatusEffects(AActor* TargetActor, const bool bCustomReason = false);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectOfClass(AActor* TargetActor,
	                                      TSubclassOf<UStatusEffect> EffectClass,
	                                      const bool bCustomReason = false,
	                                      const bool bRemoveAllStacks = true,
	                                      const int32 StacksAmount = 1);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffectsOfClass(AActor* TargetActor,
	                                          TSubclassOf<UStatusEffect> EffectClass,
	                                          const bool bCustomReason = false);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                                  TSubclassOf<UStatusEffect> EffectClass,
	                                                  const AActor* Instigator,
	                                                  const bool bCustomReason = false,
	                                                  const bool bRemoveAllStacks = true,
	                                                  const int32 StacksAmount = 1);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
	                                                      TSubclassOf<UStatusEffect> EffectClass,
	                                                      const AActor* Instigator,
	                                                      const bool bCustomReason = false);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool RemoveStatusEffectByObject(AActor* TargetActor,
	                                       UStatusEffect* StatusEffect,
	                                       const bool bCustomReason = false,
	                                       const bool bRemoveAllStacks = true,
	                                       const int32 StacksAmount = 1);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllActiveStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& ActiveEffects);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllPositiveStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& PositiveEffects);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool GetAllNegativeStatusEffects(AActor* TargetActor, TArray<UStatusEffect*>& NegativeEffects);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool HasStatusEffectOfClass(AActor* TargetActor, TSubclassOf<UStatusEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* GetStatusEffectOfClass(AActor* TargetActor, TSubclassOf<UStatusEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static bool HasStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                               TSubclassOf<UStatusEffect> EffectClass,
	                                               const AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool GetAllStatusEffectsOfClass(AActor* TargetActor,
	                                TSubclassOf<UStatusEffect> EffectClass,
	                                TArray<UStatusEffect*>& Effects);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	static UStatusEffect* GetStatusEffectOfClassByInstigator(AActor* TargetActor,
	                                                         TSubclassOf<UStatusEffect> EffectClass,
	                                                         const AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffects")
	bool GetAllStatusEffectsOfClassByInstigator(AActor* TargetActor,
	                                            TSubclassOf<UStatusEffect> EffectClass,
	                                            const AActor* Instigator,
	                                            TArray<UStatusEffect*>& Effects);
};


