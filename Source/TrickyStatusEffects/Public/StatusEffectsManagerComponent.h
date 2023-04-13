// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsManagerComponent.generated.h"

class UStatusEffect;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatusEffectAppliedSignature,
                                               UStatusEffect*, StatusEffect,
                                               AActor*, Target,
                                               AActor*, Instigator);

/**
 * An actor component which handles status effects applied to its owner.
 */
UCLASS(ClassGroup=(TrickyStatusEffects), meta=(BlueprintSpawnableComponent), NotBlueprintable, BlueprintType)
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/**Called when a new status effect applied or old effect reapplied.*/
	UPROPERTY(BlueprintAssignable, Category="StatusEffectsManager")
	FOnStatusEffectAppliedSignature OnStatusEffectApplied;

	/**Applies a new status effect or reapplies already applied effect.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	UStatusEffect* ApplyEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);

	/**Removes all status effects regardless of time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffects(const bool bCustomReason = false);

	/**Removes all positive status effects regardless of time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllPositiveEffects(const bool bCustomReason = false);

	/**Removes all negative status effects regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllNegativeEffects(const bool bCustomReason = false);

	/**Removes all neutral status effects regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllNeutralEffects(const bool bCustomReason = false);

	/**Removes the first found status effect of a given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectOfClass(TSubclassOf<UStatusEffect> EffectClass,
	                         const bool bCustomReason = false,
	                         const bool bRemoveAllStacks = true,
	                         const int32 StacksAmount = 1);

	/**Removes all status effects of a given class regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass, const bool bCustomReason = false);

	/**Removes the first found status effect of a given class of specific instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
	                                     const AActor* Instigator,
	                                     const bool bCustomReason = false,
	                                     const bool bRemoveAllStacks = true,
	                                     const int32 StacksAmount = 1);

	/**Removes all status effects of a given class of specific instigator regardless of remaining time and stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
	                                         const AActor* Instigator,
	                                         const bool bCustomReason = false);

	/**Removes a specific instance of the status effect.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectByObject(UStatusEffect* StatusEffect,
	                          const bool bCustomReason = false,
	                          const bool bRemoveAllStacks = true,
	                          const int32 StacksAmount = 1);

	/**Returns all active status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllActiveEffects(TArray<UStatusEffect*>& ActiveEffects) const;

	/**Returns all active positive status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllPositiveEffects(TArray<UStatusEffect*>& PositiveEffects) const;

	/**Returns all active negative status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllNegativeEffects(TArray<UStatusEffect*>& NegativeEffects) const;

	/**Returns all active neutral status effects.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllNeutralEffects(TArray<UStatusEffect*>& NeutralEffects) const;
	
	/**Checks if the status effect of a given class is active.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool HasEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

	/**Returns the status effect instance of the given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	UStatusEffect* GetEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

	/**Returns all status effect instances of the given class.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllEffectsOfClass(const TSubclassOf<UStatusEffect> EffectClass, TArray<UStatusEffect*>& Effects);

	/**Checks if the status effect of a given class and instigator is active.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool HasEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

	/**Returns the status effect instance of the given class and instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	UStatusEffect* GetEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

	/**Returns all status effect instances of the given class and instigator.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool GetAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
	                                      const AActor* Instigator,
	                                      TArray<UStatusEffect*>& Effects);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffectsManager")
	TArray<UStatusEffect*> ActiveEffects;

	UFUNCTION()
	void HandleEffectDeactivation(UStatusEffect* StatusEffect);

private:
#if WITH_EDITORONLY_DATA
	/**Toggles debug information on screen in editor.*/
	UPROPERTY(EditAnywhere, Category="StatusEffectsManager", AdvancedDisplay)
	bool bDebugEnabled = false;

	void PrintDebugData(const float DeltaTime);
#endif

	UStatusEffect* CreateEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);

	static bool FinishEffect(UStatusEffect* Effect,
	                         const bool bCustomReason = false,
	                         const bool bRemoveAllStacks = true,
	                         const int32 StacksAmount = 1);

	int32 GetNumberOfEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass) const;

	int32 GetNumberOfEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);
};
