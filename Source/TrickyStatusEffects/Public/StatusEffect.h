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
enum class EReActivationBehavior: uint8
{
	None,
	Reset,
	Add
};

USTRUCT(BlueprintType)
struct FStatusEffectData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="StatusEffect")
	AActor* Instigator = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	EStatusEffectType EffectType = EStatusEffectType::Positive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="!bIsInfinite"))
	EReActivationBehavior ReActivationBehavior = EReActivationBehavior::Reset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bIsUnique = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	bool bInfiniteDuration = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect", meta=(EditCondition="!bIsInfinite"))
	float Duration = 5.0f;

	UPROPERTY(BlueprintReadOnly, Category="StatusEffect")
	FTimerHandle DurationTimerHandle;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="StatusEffect")
	int32 MaxStacks = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StatusEffect")
	int32 CurrentStacks = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatusEffectDeactivatedSignature, class UStatusEffect*, StatusEffect);

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="StatusEffect")
	FStatusEffectData StatusEffectData;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void ActivateEffect();

	virtual void ActivateEffect_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="StatusEffect")
	void DeactivateEffect();

	virtual void DeactivateEffect_Implementation();

private:
	void FinishEffect();
};
