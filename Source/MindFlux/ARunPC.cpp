// Fill out your copyright notice in the Description page of Project Settings.


#include "ARunPC.h"
#include "ServerMenuGameModeBase.h"

void AARunPC::BeginPlay()
{
    Super::BeginPlay();
}

void AARunPC::ServerSetPlayerReady_Implementation()
{
    AServerMenuGameModeBase* GameMode = Cast<AServerMenuGameModeBase>(GetWorld()->GetAuthGameMode());
    if (GameMode)
    {
        GameMode->PlayerReady(this);
    }
}

bool AARunPC::ServerSetPlayerReady_Validate()
{
    // Add any validation logic here if needed
    return true;
}

void AARunPC::OnReadyButtonClicked()
{
    if (IsLocalController())
    {
        UE_LOG(LogTemp, Warning, TEXT("CLIENT READY BUTTON SENDING REQUEST"));

        ServerSetPlayerReady();
    }
}
