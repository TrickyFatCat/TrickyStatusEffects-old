// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StatusEffect.generated.h"

struct FTimerHandle;
class AActor;

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Positive,
	Negative
};

UENUM(BlueprintType)
enum class EReActivationBehavior : uint8
{
	Custom,
	Reset,
	Add
};

UENUM()
enum class EStatusEffectUniqueness : uint8
{
	Normal,
	PerInstigator,
	PerTarget
};

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="StatusEffect")
	AActor* Instigator = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="StatusEffect")
	AActor* TargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	EStatusEffectType EffectType = EStatusEffectType::Positive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	EStatusEffectUniqueness EffectUniqueness = EStatusEffectUniqueness::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bInfiniteDuration = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="!bInfiniteDuration"))
	float Duration = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category="StatusEffect")
	FTimerHandle DurationTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="!bInfiniteDuration"))
	EReActivationBehavior ReActivationBehavior = EReActivationBehavior::Reset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bIsStackable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsStackable", ClampMin=1))
	int32 MaxStacks = 1;

	UPROPERTY(VisibleAnywhere, Category="StatusEffect")
	int32 CurrentStacks = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectDeactivatedSignature, class UStatusEffect*, StatusEffect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStacksAddedSignature, UStatusEffect*, StatusEffect, int32, Amount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStacksRemovedSignature, UStatusEffect*, StatusEffect, int32, Amount);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TRICKYSTATUSEFFECTS_API UStatusEffect : public UObject
{
	GENERATED_BODY()

public:
	UStatusEffect();

protected:
	virtual void PostInitProperties() override;

	virtual void BeginDestroy() override;

public:
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStatusEffectDeactivatedSignature OnStatusEffectDeactivated;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksAddedSignature OnStacksAdded;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksRemovedSignature OnStacksRemoved;

	void StartEffect();

	void FinishEffect();
	
	void ReActivateEffect();

	AActor* GetInstigator() const { return StatusEffectData.Instigator; }

	void SetInstigator(AActor* Instigator) { StatusEffectData.Instigator = Instigator; }

	AActor* GetTargetActor() const { return StatusEffectData.TargetActor; }
	
	void SetTargetActor(AActor* TargetActor) { StatusEffectData.TargetActor = TargetActor; }

	EStatusEffectType GetEffectType() const { return StatusEffectData.EffectType; }
	
	EStatusEffectUniqueness GetUniqueness() const { return StatusEffectData.EffectUniqueness; }

	float GetRemainingTime() const;

	bool IsStackable() const { return StatusEffectData.bIsStackable; }

	int32 GetMaxStacks() const { return StatusEffectData.MaxStacks; }

	int32 GetCurrentStacks() const { return StatusEffectData.CurrentStacks; }

	bool AddStacks(int32 Amount = 1);

	bool RemoveStacks(int32 Amount = 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatusEffect")
	FStatusEffectData StatusEffectData;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void ActivateEffect();

	virtual void ActivateEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void DeactivateEffect();

	virtual void DeactivateEffect_Implementation();
};
