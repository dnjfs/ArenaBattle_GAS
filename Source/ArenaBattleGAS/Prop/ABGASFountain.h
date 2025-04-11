// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Prop/ABFountain.h"
#include "AbilitySystemInterface.h"
#include "ABGASFountain.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASFountain : public AABFountain, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AABGASFountain();

	// IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void TimerAction();

protected:
	UPROPERTY(VisibleAnywhere, Category=Movement)
	TObjectPtr<class URotatingMovementComponent> RotatingMovement;
	
	UPROPERTY(EditAnywhere, Category=Timer)
	float ActionPeriod;

	UPROPERTY(EditAnywhere, Category=GAS)
	TObjectPtr<class UAbilitySystemComponent> ASC;

	// 시작할 때 발동할 어빌리티 정보들
	// 모든 GA들은 UGameplayAbility 클래스를 상속받기 때문에 TSubclassOf로 묶음
	UPROPERTY(EditAnywhere, Category=GAS)
	TArray<TSubclassOf<class UGameplayAbility>> StartAbilities;

	FTimerHandle ActionTimer;
};
