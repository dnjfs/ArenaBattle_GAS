// Fill out your copyright notice in the Description page of Project Settings.


#include "GA/ABGA_Skill.h"
#include "Character/ABGASCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UABGA_Skill::UABGA_Skill()
{

}

void UABGA_Skill::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 5.4 버전부터 비용과 쿨다운이 정상 동작되기 위해 필요한 함수
	// GameplayAbility.h 코드 주석을 보면 ActivateAbility() 함수에서 반드시 불려야 한다고 쓰여있음
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
		return;
	}

	AABGASCharacterPlayer* TargetCharacter = Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (!TargetCharacter)
	{
		return;
	}

	ActiveSkillActionMontage = TargetCharacter->GetSkillActionMontage();
	if (!ActiveSkillActionMontage)
	{
		return;
	}

	TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("SkillMontage"), ActiveSkillActionMontage, 1.0f);
	PlayMontageTask->OnCompleted.AddDynamic(this, &UABGA_Skill::OnCompleteCallback);
	PlayMontageTask->OnInterrupted.AddDynamic(this, &UABGA_Skill::OnInterruptedCallback);

	PlayMontageTask->ReadyForActivation();
}

void UABGA_Skill::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	AABGASCharacterPlayer* TargetCharacter = Cast<AABGASCharacterPlayer>(ActorInfo->AvatarActor.Get());
	if (TargetCharacter)
	{
		TargetCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UABGA_Skill::OnCompleteCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}

void UABGA_Skill::OnInterruptedCallback()
{
	bool bReplicatedEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicatedEndAbility, bWasCancelled);
}
