// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MindFluxGameModeBase.generated.h"

class AFloorTile;
class ARunCharacter;

/**
 * 
 */
UCLASS()
class MINDFLUX_API AMindFluxGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FTimerHandle RespawnTimerHandle;

	UFUNCTION()
	void StartCharactersTicking();
	

	TArray<AFloorTile*> TilesArray;
	//int TotalTileNum = 0;
	FTransform TransformRespawn;

	void DestroyTiles();

	AFloorTile* AddFloorTileAfterRespawn(const bool bSpawnItems);

	UPROPERTY(EditAnywhere, Category = "Config")
	bool SetCountAsGameStateToStart;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<ARunCharacter> CharacterClass;

	UFUNCTION(BlueprintCallable)
	int32 CountPlayers();

	//ARunCharacter* Ch1;

	UPROPERTY(EditAnywhere, Category = "Config")
	int32 NumInitialFloorTiles = 5;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform NextSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	FTransform InitialSpawnPoint;

	UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
	TArray<float> LaneSwitchValues;

	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
	AFloorTile* AddFloorTile(const bool bSpawnItems);

protected:
	virtual void BeginPlay() override;
};
