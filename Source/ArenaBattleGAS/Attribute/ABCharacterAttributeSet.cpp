// Fill out your copyright notice in the Description page of Project Settings.


#include "Attribute/ABCharacterAttributeSet.h"
#include "ArenaBattleGAS.h"

UABCharacterAttributeSet::UABCharacterAttributeSet() :
	AttackRange(100.0f),
	AttackRadius(50.f),
	AttackRate(30.0f),
	MaxAttackRange(300.0f),
	MaxAttackRadius(150.0f),
	MaxAttackRate(100.0f),
	MaxHealth(100.0f)
{
	InitHealth(GetMaxHealth());
}

void UABCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// NewValue가 레퍼런스로 넘어오므로 값이 변경되기 전에 Clamp()로 범위를 지정해주는 등 직접 수정할 수 있음

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UABCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		ABGAS_LOG(LogABGAS, Log, TEXT("Health : %f -> %f"), OldValue, NewValue);
	}
}
