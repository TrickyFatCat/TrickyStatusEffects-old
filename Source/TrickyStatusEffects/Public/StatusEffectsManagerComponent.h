// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsManagerComponent.generated.h"

class UStatusEffect;
class AActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStatusEffectAddedSignature, UStatusEffect*, StatusEffect, AActor*, Target, AActor*, Instigator);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatusEffectsManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category="StatusEffectsManager")
	FOnStatusEffectAddedSignature OnStatusEffectAdded;
	
	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	void AddEffect(TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffects();

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectOfClass(TSubclassOf<UStatusEffect> EffectClass, const bool bIgnoreStacks = true);

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass,
	                                     const AActor* Instigator,
	                                     const bool bIgnoreStacks = true);

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveAllEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	bool RemoveEffectByObject(UStatusEffect* StatusEffect, const bool bIgnoreStacks = true);

	bool HasEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

	UStatusEffect* GetEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

	bool HasEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

	UStatusEffect* GetEffectOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffectsManager")
	TArray<UStatusEffect*> ActiveEffects;

	UFUNCTION()
	void HandleEffectDeactivation(UStatusEffect* StatusEffect);

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category="StatusEffectsManager", AdvancedDisplay)
	bool bDebugEnabled = false;
	
	void PrintDebugData(const float DeltaTime);
#endif
	
	void CreateEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);

	int32 GetNumberOfEffectsOfClass(TSubclassOf<UStatusEffect> EffectClass) const;

	int32 GetNumberOfEffectsOfClassByInstigator(TSubclassOf<UStatusEffect> EffectClass, const AActor* Instigator);

	
};
