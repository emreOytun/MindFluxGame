// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARunPC.generated.h"

/**
 * 
 */
UCLASS()
class MINDFLUX_API AARunPC : public APlayerController
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadWrite,EditAnywhere)
    int32 TotalHeart = 3;

    // RPC to notify server that the player is ready
    UFUNCTION(BlueprintCallable, Category = "UI", Server, Reliable, WithValidation)
    void ServerSetPlayerReady();
    void ServerSetPlayerReady_Implementation();
    bool ServerSetPlayerReady_Validate();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OnReadyButtonClicked();

protected:
    virtual void BeginPlay() override;
};
