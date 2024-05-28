// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Obstacle.h"
#include "MindFluxGameModeBase.h"
#include "CoinItem.h"
#include "FlyItem.h"
#include "IM_Item.h"
#include "HandwristItem.h"
#include "FloorTile.generated.h"


class ACoinItem;
class AFlyItem;
class AIM_Item;
class AHadnwristItem;
class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class AObstacle;

UCLASS()
class MINDFLUX_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	int TileNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Config")
	TSubclassOf<AObstacle> SmallObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AObstacle> BigObstacleClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<ACoinItem> CointItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AFlyItem> FlyItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AIM_Item> IM_ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TSubclassOf<AHandwristItem> HandWristItemClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* RightLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FloorTriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float SpawnPercent1 = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float SpawnPercent2 = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float SpawnPercent3 = 0.5f;

	UFUNCTION(BlueprintCallable)
	void SpawnItems(FMapsDetail MapDetail);

	int count = 0;

	// Sets default values for this actor's properties
	AFloorTile();

	FORCEINLINE const FTransform& GetAttachTransform() const 
	{
		return AttachPoint->GetComponentTransform();
	}

protected:

	UPROPERTY(VisibleInstanceOnly)
	class AMindFluxGameModeBase* RunGameMode;

	UFUNCTION()
	void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	int SpawnLaneItem(FMapsDetail MapDetail, UArrowComponent* Lane, int totalBigObstacle);
	
	UPROPERTY()
	FTimerHandle DestroyHandle;

	UFUNCTION()
	void DestroyFloorTile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

};
