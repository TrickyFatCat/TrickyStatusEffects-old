// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimerManager.h"
#include "StatusEffect.generated.h"

struct FTimerHandle;
class AActor;
class UStatusEffectsManagerComponent;

UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Positive,
	Negative,
	Neutral
};

UENUM(BlueprintType)
enum class EReActivationBehavior : uint8
{
	None UMETA(ToolTip="No changes."),
	Custom UMETA(ToolTip="By default no changes, but can be overriden."),
	Reset UMETA(ToolTip="Reset the timer."),
	Add UMETA(ToolTip="Add default duration to remaining time.")
};

UENUM(BlueprintType)
enum class EStatusEffectUniqueness : uint8
{
	Normal UMETA(Tooltip="No limits."),
	PerInstigator UMETA(Tooltip="Instigator can apply only one status effect of this class on a target."),
	PerTarget UMETA(Tooltip="Only one status effect of this class can be applied to the target.")
};

/**Determines the reason of the effect deactivation.*/
UENUM(BlueprintType)
enum class EDeactivationReason : uint8
{
	Time UMETA(Tooltip="Used when the status effect deactivated when its timer finishes."),
	Stacks UMETA(Tooltip="Used when current number of stacks reaches 0."),
	Remove UMETA(Tooltip="Used when the status effect was removed regardless of stacks and remaining time."),
	Custom UMETA(
		Tooltip=
		"Used for calling custom logic when the status effect was removed regardless of stacks and remainng time.")
};

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	/**The actor which applied the status effect.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="StatusEffect")
	TObjectPtr<AActor> Instigator = nullptr;

	/**The target actor of the status effect.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="StatusEffect")
	TObjectPtr<AActor> TargetActor = nullptr;

	/**Status effects manager component which owns the status effect.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="StatusEffect")
	TObjectPtr<UStatusEffectsManagerComponent> OwningManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	EStatusEffectType EffectType = EStatusEffectType::Positive;

	/**Determines how many instances of the status effect can be created.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	EStatusEffectUniqueness EffectUniqueness = EStatusEffectUniqueness::Normal;

	/**Toggles if the status effect has infinite duration or not.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bIsInfinite = false;

	/**Duration of the effect.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="!bIsInfinite"))
	float Duration = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category="StatusEffect")
	FTimerHandle DurationTimerHandle;

	/**Determines how the duration will be recalculated when the status effect was reapplied.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsInfinite"))
	EReActivationBehavior DurationReActivationBehavior = EReActivationBehavior::None;

	/**Toggles if the status effect can be stacked.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bIsStackable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsStackable", ClampMin=1))
	int32 MaxStacks = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect",
		meta=(EditCondition="bIsStackable", ClampMin="1"))
	int32 InitialStacks = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsStackable"))
	int32 CurrentStacks = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="bIsStackable"))
	EReActivationBehavior StacksReActivationBehavior = EReActivationBehavior::Add;

	/**Amount of stacks added per re-activation*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect",
		meta=(EditCondition="bIsStackable && StacksReActivationBehavior==EReActivationBehavior::Add"))
	int32 DeltaStacks = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectActionPerformedSignature, class UStatusEffect*, StatusEffect)
;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStacksNumberChangedSignature,
                                               UStatusEffect*, StatusEffect,
                                               int32, NewValue,
                                               int32, DeltaValue);

/**
 *An object which encapsulates status effect logic. 
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
	/**Called when the status effect was deactivated.*/
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStatusEffectActionPerformedSignature OnStatusEffectDeactivated;

	/**Called when the status effect was reactivated.*/
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStatusEffectActionPerformedSignature OnStatusEffectReactivated;

	/**Called when current number of stacks was increased.*/
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksNumberChangedSignature OnStacksAdded;

	/**Called when current current number of stacks was decreased.*/
	UPROPERTY(BlueprintAssignable, Category="StatusEffect")
	FOnStacksNumberChangedSignature OnStacksRemoved;

	bool Activate(AActor* TargetActor, AActor* Instigator, UStatusEffectsManagerComponent* ManagerComponent);

	void Deactivate(const EDeactivationReason Reason);

	void ReActivate();

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	AActor* GetInstigator() const { return StatusEffectData.Instigator; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	AActor* GetTargetActor() const { return StatusEffectData.TargetActor; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	UStatusEffectsManagerComponent* GetOwningManager() const { return StatusEffectData.OwningManager; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EStatusEffectType GetEffectType() const { return StatusEffectData.EffectType; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EStatusEffectUniqueness GetUniqueness() const { return StatusEffectData.EffectUniqueness; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	bool GetIsInfinite() const { return StatusEffectData.bIsInfinite; }

	/**If InfiniteDuration == false, returns duration, else -1.*/
	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetDuration() const { return StatusEffectData.bIsInfinite ? -1.0 : StatusEffectData.Duration; }

	/**If  InfiniteDuration == false returns remaining time, else -1.*/
	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetRemainingTime() const;

	/**If  InfiniteDuration == false returns elapsed time, else -1.*/
	UFUNCTION(BlueprintPure, Category="StatusEffect")
	float GetElapsedTime() const;

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	EReActivationBehavior GetReactivationBehavior() const { return StatusEffectData.DurationReActivationBehavior; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	bool IsStackable() const { return StatusEffectData.bIsStackable; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	int32 GetMaxStacks() const { return StatusEffectData.MaxStacks; }

	UFUNCTION(BlueprintPure, Category="StatusEffect")
	int32 GetCurrentStacks() const { return StatusEffectData.CurrentStacks; }

	/**Increases the number of stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool AddStacks(int32 Amount = 1);

	/**Decreases the number of stacks.*/
	UFUNCTION(BlueprintCallable, Category="StatusEffect")
	bool RemoveStacks(int32 Amount = 1);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	FStatusEffectData StatusEffectData;

	/**Called when the status effect was activated.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	bool HandleEffectActivation();

	virtual bool HandleEffectActivation_Implementation();

	/**Called when the status effect was deactivated.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void HandleEffectDeactivation(const EDeactivationReason Reason);

	virtual void HandleEffectDeactivation_Implementation(const EDeactivationReason Reason);

	/**Called when the status effect was reactivated.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void HandleEffectReactivation(const EReActivationBehavior ReactivationBehavior);

	virtual void HandleEffectReactivation_Implementation(const EReActivationBehavior ReactivationBehavior);

	/**Called when number of stacks was increased.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void HandleStacksIncrease(const int32 Amount);

	virtual void HandleStacksIncrease_Implementation(const int32 Amount);

	/**Called when number of stacks was decreased.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void HandleStacksDecrease(const int32 Amount);

	virtual void HandleStacksDecrease_Implementation(const int32 Amount);

private:
	void StartTimer(const UWorld* World, const float Duration);
};
