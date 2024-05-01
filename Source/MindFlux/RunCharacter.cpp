// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MindFluxGameModeBase.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm")); // We can create components we see in BP_Character to C++ code like that.
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());	// CameraArm should be under CapsuleComponent as we can see in BP_Character hiearchy.

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false; // Camera is not controlled by controller. It is controlled from CameraArm.
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGINPLAY CHARAC %d"), GetWorld()->IsServer());
	Super::BeginPlay();
	RunGameMode = Cast<AMindFluxGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;
	if (RestartTimerHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}

	if(GetWorld()->IsServer()){

		// Reload the current level to restart the game
		UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
	}

}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	//if (GetWorld()->IsServer()) {
		UE_LOG(LogTemp, Warning, TEXT("TICK CHARAC %d"), GetWorld()->IsServer());
		Super::Tick(DeltaTime);

		FRotator ControlRot = GetControlRotation();
		ControlRot.Roll = 0.f;
		ControlRot.Pitch = 0.f;

		// ControlRot.Vector() == For getting forward vector
		AddMovementInput(ControlRot.Vector());
	//}
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &ARunCharacter::MoveDown);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);
}


void ARunCharacter::MoveLeft() 
{
	UE_LOG(LogTemp, Warning, TEXT("MOVELEFT %d"), GetWorld()->IsServer());
	Server_OnTrigger(0);
}

void ARunCharacter::MoveRight() 
{
	UE_LOG(LogTemp, Warning, TEXT("MOVERIGHT %d"), GetWorld()->IsServer());
	Server_OnTrigger(1);
}

void ARunCharacter::MoveDown() 
{
	UE_LOG(LogTemp, Warning, TEXT("MOVE DOWN WAS PRESSED"));
}

// float Value: Comes from timeline. It gives where it is in the timeline
// FMath::Lerp: It interpolates from current position to next position according to the given timeline value.
void ARunCharacter::ChangeLaneUpdate(float Value) {
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished() {
	CurrentLane = NextLane;
}

void ARunCharacter::Death()
{
	if (!bIsDead) {

		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (World) {
			bIsDead = true;
			DisableInput(nullptr);

			if (DeathParticleSystem) {
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}

			if (DeathSound) {
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}

			GetMesh()->SetVisibility(false);
			World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);
		}
	}

}

bool ARunCharacter::Server_OnTrigger_Validate(bool isRight)
{
	return true;
}

void ARunCharacter::Server_OnTrigger_Implementation(bool isRight)
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER %d"), GetWorld()->IsServer());
	if (isRight) {
		NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
		ChangeLane();
	}
	else {
		NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
		ChangeLane();
	}
}