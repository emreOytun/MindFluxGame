// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MindFluxGameModeBase.generated.h"

class AFloorTile;
class ARunCharacter;
class AObstacle;
class AFinishFlag;

/**
 * 
 */


USTRUCT(BlueprintType)
struct FMapsDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxTileCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SmallObstacleMinRange = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SmallObstacleMaxRange = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BigObstacleMinRange = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BigObstacleMaxRange = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AObstacle> SmallObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AObstacle> BigObstacle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFinishFlag> Flag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AFloorTile> Tile;

};
UCLASS()
class MINDFLUX_API AMindFluxGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	FTimerHandle RespawnTimerHandle;

	UFUNCTION()
	void StartCharactersTicking();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString CurrentLevelName;

	TArray<AFloorTile*> TilesArray;
	//int TotalTileNum = 0;
	FTransform TransformRespawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> PausableLevels;
	
	void DestroyTiles();

	AFloorTile* AddFloorTileAfterRespawn(const bool bSpawnItems);

	UPROPERTY(EditAnywhere, Category = "Config")
	bool SetCountAsGameStateToStart;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnywhere, Category = "Config")
	TSubclassOf<ARunCharacter> CharacterClass;

	UPROPERTY(EditAnywhere, Category = "MapConfig")
	TMap<FString,FMapsDetail> MapDetails;

	UFUNCTION(BlueprintCallable)
	int32 CountPlayers();

	UFUNCTION()
	void CreateFinishWidgetForAllPlayers(FString Name);

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
	AFloorTile* AddFloorTile(FMapsDetail MapDetail,const bool bSpawnItems);

protected:
	virtual void BeginPlay() override;
};
