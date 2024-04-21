// Fill out your copyright notice in the Description page of Project Settings.
#include "RunAnimInstance.h"

void URunAnimInstance::NativeInitializeAnimation() 
{
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}

	if (IsValid(Pawn)) {
		MovementComponent = Pawn->GetMovementComponent();
	}
}

void URunAnimInstance::NativeUpdateAnimation(float DeltaSecond) {
	if (Pawn && MovementComponent) {
		bIsInAir = MovementComponent->IsFalling();
		Speed = Pawn->GetVelocity().Size();
	}
}
