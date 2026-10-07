// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterBase.h"
#include "ABCharacterControlData.h"
#include "ABComboActionData.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/CapsuleComponent.h>
#include <Physics/ABCollision.h>
#include <Engine/DamageEvents.h>

// Sets default values
AABCharacterBase::AABCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 맵(TMap) 설정
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> ShoulderDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Shoulder.ABC_Shoulder")
	);

	if (ShoulderDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Shoulder,
			ShoulderDataRef.Object
		);
	}

	// 맵(TMap) 설정
	static ConstructorHelpers::FObjectFinder<UABCharacterControlData> QuaterDataRef(
		TEXT("/Game/ArenaBattle/CharacterControl/ABC_Quater.ABC_Quater")
	);

	if (QuaterDataRef.Succeeded())
	{
		CharacterControlManager.Add(
			ECharacterControlType::Quater,
			QuaterDataRef.Object
		);
	}

	// 콜리전 설정
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	// 몽타주 및 콤보 액션 데이터 애셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(
		TEXT("/Game/ArenaBattle/Animation/AM_ComboAttack.AM_ComboAttack")
	);

	if (ComboActionMontageRef.Succeeded())
	{
		ComboAttackMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UABComboActionData> ComboActionDataRef(
		TEXT("/Game/ArenaBattle/ComboData/ABA_ComboAction.ABA_ComboAction")
	);

	if (ComboActionDataRef.Succeeded())
	{
		ComboAttackData = ComboActionDataRef.Object;
	}

	// 죽음 몽타주 애셋 바인딩
	// 몽타주 및 콤보 액션 데이터 애셋 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Game/ArenaBattle/Animation/AM_Dead.AM_Dead")
	);

	if (DeadMontageRef.Succeeded())
	{
		DeadMontage = DeadMontageRef.Object;
	}
}

float AABCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage =
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 대미지를 받으면 죽음 처리
	SetDead();

	return DamageAmount;
}

void AABCharacterBase::SetDead()
{
	// 움직이지 않도록 처리
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// 죽는 모션 재생
	PlayDeadAnimation();

	// 콜리전 끄기
	SetActorEnableCollision(false);
}

void AABCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 현재 재생중인 모든 몽타주 중지
		AnimInstance->StopAllMontages(0.0f);

		// 재생
		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

void AABCharacterBase::SetCharacterControlData(const UABCharacterControlData* InCharacterControlData)
{
	// 데이터에서 속성을 가져와서 필요한 곳에 설정
	/*
	uint32 bUseControllerRotationYaw : 1;

	// 캐릭터 무브먼트에 설정할 회전 관련 속성.
	// 캐릭터 이동 시 컨트롤러와 같은 방향으로 움직일 것인지.
	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseOrientToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 RotationRate;*/
	// 
	// Pawn 설정
	bUseControllerRotationYaw = InCharacterControlData->bUseControllerRotationYaw;

	// 캐릭터 무브먼트 설정
	GetCharacterMovement()->bUseControllerDesiredRotation
		= InCharacterControlData->bUseControllerDesiredRotation;

	GetCharacterMovement()->bOrientRotationToMovement
		= InCharacterControlData->bUseOrientToMovement;

	GetCharacterMovement()->RotationRate
		= InCharacterControlData->RotationRate;

}

void AABCharacterBase::ProcessComboCommand()
{
	// 처음 공격 시작할 때
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	// 처음이 아닌 경우
	// 타이머 핸들의 유효성 여부로 다음 공격 분기 결정
	if (ComboTimerHandle.IsValid())
	{
		// 입력이 제대로 들어왔다고 판정
		bHasNextComboCommand = true;
	}
	else
	{
		bHasNextComboCommand = false;
	}
}

void AABCharacterBase::ComboActionBegin()
{
	//  현재 콤보 단계를 1로 설정
	CurrentCombo = 1;

	// 몽타주 재생
	// 몽타주 재생을 위해 애님 인스턴스 가져오기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		// 몽타주 재생 속도
		const float AttackSpeedRate = 1.0f;

		// 몽타주 재생
		AnimInstance->Montage_Play(ComboAttackMontage, AttackSpeedRate);

		// 몽타주 종료 이벤트에 등록
		FOnMontageEnded OnMontageEnded;
		OnMontageEnded.BindUObject(this, &AABCharacterBase::ComboActionEnded);

		AnimInstance->Montage_SetEndDelegate(OnMontageEnded, ComboAttackMontage);

		// 공격 모션 중에는 이동하지 않도록 설정
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		// 타이머 재사용을 위해 초기화
		ComboTimerHandle.Invalidate();

		// 타이머 설정
		SetComboCheckTimer();
	}
}

void AABCharacterBase::ComboActionEnded(UAnimMontage* TargetMontage, bool bInterrupted)
{
	// 확인
	ensureAlways(CurrentCombo > 0);

	// 콤보 단계 초기화
	CurrentCombo = 0;

	// 몽타주 재생이 종료되면 캐릭터 이동 복구
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AABCharacterBase::SetComboCheckTimer()
{
	// 현재 재생 중인 콤보 단계의 인덱스 계산(인덱스는 0부터니께 -1)
	const int32 ComboIndex = CurrentCombo - 1;

	// 인덱스 값 확인
	ensureAlways(
		ComboAttackData->EffectiveFrameCount.IsValidIndex(ComboIndex)
	);

	// 애니메이션 재생 속도
	const float AttackSpeedRate = 1.0f;

	// 콤보 공격 입력 시간(초단위) 계산 => 17 / 30
	float ComboEffectTime = (ComboAttackData->EffectiveFrameCount[ComboIndex] 
		/ ComboAttackData->FrameRate / AttackSpeedRate);

	// 타이머 설정
	if (ComboEffectTime > 0) // 4번=-1이니께 예외처리
	{
		GetWorld()->GetTimerManager().SetTimer(
			ComboTimerHandle,
			this,
			&AABCharacterBase::ComboCheck,
			ComboEffectTime,
			false // 반복 여부
		);
	}
	
}

void AABCharacterBase::ComboCheck()
{
	// 타이머 재사용을 위해 초기화
	ComboTimerHandle.Invalidate();

	// 콤보 타이머 이전에 공격 입력이 제대로 들어왔는지 확인(분기)
	if (bHasNextComboCommand)
	{
		// 몽타주점프 처리를 위해 애님 인스턴스 가져오기
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			// 다음 단계 콤보 설정
			// CurrentCombo + 1
			CurrentCombo = FMath::Clamp(
				CurrentCombo + 1,
				1,
				ComboAttackData->MaxComboCount // 1~4고정
			);

			// 점프할 섹션 이름 구성
			FName NextSection = *FString::Printf(TEXT("%s%d"),
				*ComboAttackData->MontageSectionNamePrefix, CurrentCombo);

			// 몽타주 섹션 점프
			AnimInstance->Montage_JumpToSection(NextSection, ComboAttackMontage);

			// 타이머 재설정(새로운 섹션(구간)으로 점프했기 때문)
			SetComboCheckTimer();

			// 콤보 처리에 사용한 값 초기화
			bHasNextComboCommand = false;

		}
	}
}

void AABCharacterBase::AttackHitCheck()
{
	// 트레이스를 활용한 충돌 확인.
	FHitResult OutHitResult;

	// 콜리전 쿼리 파라미터
	// SCENE_QUERY_STAT(Attack): Attack이라는 태그값을 만들어준다
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// 공격 범위
	const float AttackRange = 30.0f;

	// 트레이스에 사용할 구체의 반지름
	const float AttackRadius = 50.0f;

	// 트레이스 시작 위치
	// 액터 위치 + 캡슐 높이의 반지름만큼 앞으로 떨어진 위치.
	FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	// 트레이스 종료 위치
	// 시작위치 + 공격범위만큼 앞으로 떨어진 위치 
	FVector End = Start + GetActorForwardVector() * AttackRange;
	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity, // 회전안함
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	// 충돌이 감지되면 대미지 전달
	if (HitDetected)
	{
		// 전달할 대미지
		const float AttackDamage = 30.0f;

		// 대미지 이벤트 변수
		FDamageEvent DamageEnvent;

		// TakeDamage 함수를 호출하여 대미지 전달
		OutHitResult.GetActor()->TakeDamage(
			AttackDamage,
			DamageEnvent,
			GetController(),
			this
		);
	}

	// 시각적으로 충돌 여부를 확인할 수 있도록 디버깅 기능 활용
#if ENABLE_DRAW_DEBUG

	// 캡슐의 중심 위치
	// End - Start: Start위치에서 End위치로 향하는 벡터.
	// 시작 위치에서 절반 위치만큼 이동한 거리를 구한다. 
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;

	// 캡슐 높이의 절반
	float CapsuleHalfHeight = AttackRange * 0.5f;

	// 표시할 색상(맞았으면 빨간색, 안 맞았으면 녹색)
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	// 캡슐 그리기
	DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		AttackRadius,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		5.0f
		);

#endif
}
