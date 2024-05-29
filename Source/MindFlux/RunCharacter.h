// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "RunCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHeartDelegate, int, HearthCount);

UCLASS()
class MINDFLUX_API ARunCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleInstanceOnly)
	class AMindFluxGameModeBase* RunGameMode;

public:

	UPROPERTY(EditAnywhere, Category = "Config")
	bool CountCharacters;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 TotalCoins = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FHeartDelegate HeartDelegate;

	// Count the total number of characters
	UPROPERTY(Replicated)
	int32 TotalCharacters;

	// Override Replicate Properties function
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(Client, Reliable)
	void Client_OnTrigger();
	bool Client_OnTrigger_Validate();
	void Client_OnTrigger_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OnTrigger(bool isRight);
	bool Server_OnTrigger_Validate(bool isRight);
	void Server_OnTrigger_Implementation(bool isRight);


	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerRespawn();
	void ServerRespawn_Implementation();
	bool ServerRespawn_Validate();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Assets")
	class UParticleSystem* DeathParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Assets")
	class USoundBase* DeathSound;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 0;

	UFUNCTION(BlueprintImplementableEvent, Category = "Lane")
	void ChangeLane();

	UFUNCTION(BlueprintImplementableEvent, Category = "Widget")
	void CreateFinishWidget(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneUpdate(float Value);

	UFUNCTION(BlueprintCallable, Category = "Lane")
	void ChangeLaneFinished();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void AddCoin();

	UFUNCTION()
	void Fly();

	UFUNCTION()
	void GetImageProcessing();

	UFUNCTION()
	void SetHandwrist();

	// Sets default values for this character's properties
	ARunCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void MoveDown();

	UPROPERTY()
	FTimerHandle RestartTimerHandle;

	// Touch input handlers
	virtual void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	virtual void TouchEnded(const ETouchIndex::Type FingerIndex, const FVector Location);


	FVector2D TouchStartLocation;
	FVector2D TouchEndLocation;

	UPROPERTY()
	bool bIsDead = false;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
