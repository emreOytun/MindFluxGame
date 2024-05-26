// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ServerMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MINDFLUX_API AServerMenuGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	// Function to handle player ready state
	void PlayerReady(APlayerController* PlayerController);

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintNativeEvent, Category = "GameMode")
	FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal);
	virtual FString InitNewPlayer_Implementation(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal);

	UFUNCTION(BlueprintNativeEvent, Category = "GameMode")
	void PostLogin(APlayerController* NewPlayer);
	virtual void PostLogin_Implementation(APlayerController* NewPlayer);

	UFUNCTION(BlueprintNativeEvent, Category = "GameMode")
	void Logout(AController* Exiting);
	virtual void Logout_Implementation(AController* Exiting);
};
