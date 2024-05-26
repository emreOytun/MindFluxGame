// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ServerMenuPC.generated.h"

/**
 * 
 */
UCLASS()
class MINDFLUX_API AServerMenuPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ServerTravel(const FString& mapPath);
};
