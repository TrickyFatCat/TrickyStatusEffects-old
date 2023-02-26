// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusEffectsManagerComponent.generated.h"

class UStatusEffect;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TRICKYSTATUSEFFECTS_API UStatusEffectsManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusEffectsManagerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="StatusEffectsManager")
	void AddEffect(TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);

	bool HasEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

	UStatusEffect* GetEffectOfClass(TSubclassOf<UStatusEffect> EffectClass);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffectsManager")
	TArray<UStatusEffect*> ActiveEffects;

	UFUNCTION()
	void HandleEffectDeactivation(UStatusEffect* StatusEffect);

private:
	void CreateEffect(const TSubclassOf<UStatusEffect> EffectClass, AActor* Instigator);
};
