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
enum class EReactivationBehavior : uint8
{
	Custom,
	Reset,
	Add
};

UENUM(BlueprintType)
enum class EStatusEffectUniqueness : uint8
{
	Normal,
	PerInstigator,
	PerTarget
};

UENUM(BlueprintType)
enum class EDeactivationReason : uint8
{
	Time,
	Stacks,
	Remove,
	Custom
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
	EReactivationBehavior ReStartBehavior = EReactivationBehavior::Reset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsStackable", ClampMin=1))
	int32 MaxStacks = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect",
		meta=(EditCondition="bIsStackable", ClampMin="1"))
	int32 InitialStacks = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffect")
	int32 CurrentStacks = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectDeactivatedSignature, class UStatusEffect*, StatusEffect);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectReactivatedSignature, class UStatusEffect*, StatusEffect);

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
	FOnStatusEffectReactivatedSignature OnStatusEffectReactivated;
	
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksAddedSignature OnStacksAdded;

	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksRemovedSignature OnStacksRemoved;

	void StartEffect();

	void FinishEffect(const EDeactivationReason Reason);

	void ReStartEffect();

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	AActor* GetInstigator() const { return StatusEffectData.Instigator; }

	void SetInstigator(AActor* Instigator) { StatusEffectData.Instigator = Instigator; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	AActor* GetTargetActor() const { return StatusEffectData.TargetActor; }

	void SetTargetActor(AActor* TargetActor) { StatusEffectData.TargetActor = TargetActor; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EStatusEffectType GetEffectType() const { return StatusEffectData.EffectType; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EStatusEffectUniqueness GetUniqueness() const { return StatusEffectData.EffectUniqueness; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetRemainingTime() const;

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EReactivationBehavior GetRestartBehavior() const { return StatusEffectData.ReStartBehavior; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	bool IsStackable() const { return StatusEffectData.bIsStackable; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	int32 GetMaxStacks() const { return StatusEffectData.MaxStacks; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	int32 GetCurrentStacks() const { return StatusEffectData.CurrentStacks; }

	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool AddStacks(int32 Amount = 1);

	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool RemoveStacks(int32 Amount = 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	FStatusEffectData StatusEffectData;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void ActivateEffect();

	virtual void ActivateEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void DeactivateEffect(const EDeactivationReason Reason);

	virtual void DeactivateEffect_Implementation(const EDeactivationReason Reason);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void ReactivateEffect(const EReactivationBehavior ReactivationBehavior);

	virtual void ReactivateEffect_Implementation(const EReactivationBehavior ReactivationBehavior);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void StacksIncreased(const int32 Amount);

	virtual void StacksIncreased_Implementation(const int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void StacksDecreased(const int32 Amount);

	virtual void StacksDecreased_Implementation(const int32 Amount);

private:
	void StartTimer(const UWorld* World, const float Duration);
};
