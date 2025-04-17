// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterNonPlayer.h"
#include "AbilitySystemComponent.h"
#include "Attribute/ABCharacterAttributeSet.h"

AABGASCharacterNonPlayer::AABGASCharacterNonPlayer()
{
	// NPC는 ASC를 직접 생성
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UABCharacterAttributeSet>(TEXT("AttributeSet"));

	Level = 1;
}

UAbilitySystemComponent* AABGASCharacterNonPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterNonPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// NPC는 Owner와 Avatar를 자기 자신으로 설정
	ASC->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(InitStatEffect, Level, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		// 게임플레이 이펙트를 자기 자신에게 적용하여 NPC의 스탯 초기화
		ASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}
