// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/ABGASFountain.h"
#include "ABGASFountain.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ArenaBattleGAS.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
//#include "GA/ABGA_Rotate.h" // 헤더의 의존성 제거
#include "Tag/ABGameplayTag.h"
#include "Abilities/GameplayAbility.h"

AABGASFountain::AABGASFountain()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotateMovement"));
	ActionPeriod = 3.0f;
}

UAbilitySystemComponent* AABGASFountain::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASFountain::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	RotatingMovement->bAutoActivate = false;
	RotatingMovement->Deactivate();

	// 어빌리티 시스템 초기화 (Owner: 데이터를 관리할 주체, Avatar: 동작 대상)
	ASC->InitAbilityActorInfo(this, this);

	/*
	// 2. 게임플레이 어빌리티 시스템으로 구현
	FGameplayAbilitySpec RotateSkillSpec(UABGA_Rotate::StaticClass());
	ASC->GiveAbility(RotateSkillSpec);
	*/

	for (const auto& StartAbility : StartAbilities)
	{
		FGameplayAbilitySpec StartSpec(StartAbility);
		ASC->GiveAbility(StartSpec);
	}
}

void AABGASFountain::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(ActionTimer, this, &AABGASFountain::TimerAction, ActionPeriod, true, 0.0f);
}

void AABGASFountain::TimerAction()
{
	ABGAS_LOG(LogABGAS, Log, TEXT("Begin"));

	/*
	// 1. 기존 방법으로 구현
	if (!RotatingMovement->IsActive())
	{
		RotatingMovement->Activate(true);
	}
	else
	{
		RotatingMovement->Deactivate();
	}
	*/

	/*
	// 2. 게임플레이 어빌리티 시스템으로 구현
	FGameplayAbilitySpec* RotateGASpec = ASC->FindAbilitySpecFromClass(UABGA_Rotate::StaticClass());
	if (!RotateGASpec)
	{
		ABGAS_LOG(LogABGAS, Error, TEXT("No Rotate Spec Found!"));
		return;
	}

	if (!RotateGASpec->IsActive())
	{
		ASC->TryActivateAbility(RotateGASpec->Handle);
	}
	else
	{
		ASC->CancelAbilityHandle(RotateGASpec->Handle);
	}
	*/

	// 3. 게임플레이 어빌리티 시스템으로 구현
	FGameplayTagContainer TargetTag(ABTAG_ACTOR_ROTATE);

	// 현재 해당 태그 동작이 활성화 되어 있는지 확인
	if (!ASC->HasMatchingGameplayTag(ABTAG_ACTOR_ISROTATING))
	{
		// 태그로 어빌리티 발동
		ASC->TryActivateAbilitiesByTag(TargetTag); // 여러 태그를 넣을 수 있어 컨테이너로 지정
	}
	else
	{
		// 태그로 어빌리티 취소
		ASC->CancelAbilities(&TargetTag);
	}
}
