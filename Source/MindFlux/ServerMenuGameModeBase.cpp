// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerMenuGameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void AServerMenuGameModeBase::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY GAME MODE1"));
}

void AServerMenuGameModeBase::PlayerReady(APlayerController* PlayerController)
{
    UE_LOG(LogTemp, Warning, TEXT("PLAYER READY GAME MODE1"));

    // Handle the player's ready state
    if (GetWorld()->IsServer()) {
        if (PlayerController)
        {
            UE_LOG(LogTemp, Log, TEXT("Player is ready: %s"), *PlayerController->GetName());

            FString UrlString = TEXT("RoadLevel");
            GetWorld()->ServerTravel(UrlString);

            // Implementation for checking all players ready and starting game
            // You might keep track of all ready players and check if all are ready
            // If all players are ready, you can then transition to the game level
        }
    }
}


FString AServerMenuGameModeBase::InitNewPlayer_Implementation(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
    // Call the base implementation
    Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

    // Custom initialization code for the new player
    UE_LOG(LogTemp, Log, TEXT("Player %s has connected"), *NewPlayerController->GetName());

    // Return a custom string
    return FString::Printf(TEXT("Player %s initialized"), *NewPlayerController->GetName());
}

void AServerMenuGameModeBase::PostLogin_Implementation(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    // Custom logic after player has successfully logged in
    UE_LOG(LogTemp, Log, TEXT("PostLogin: %s"), *NewPlayer->GetName());
}

void AServerMenuGameModeBase::Logout_Implementation(AController* Exiting)
{
    Super::Logout(Exiting);

    // Custom logic for when a player logs out
    UE_LOG(LogTemp, Log, TEXT("Logout: %s"), *Exiting->GetName());
}
