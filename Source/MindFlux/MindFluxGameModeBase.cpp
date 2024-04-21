// Copyright Epic Games, Inc. All Rights Reserved.

#include "MindFluxGameModeBase.h"
#include "FloorTile.h"
#include "RunCharacter.h"

void AMindFluxGameModeBase::BeginPlay()
{
	if (GetWorld()->IsServer()) {
		CreateInitialFloorTiles();
	
		/*
		// Create a transform with specific translation, rotation, and scale
		FVector Location(0.0f, 0.0f, -200.0f);
		FRotator Rotation(0.0f, 0.0f, 300.0f);
		FVector Scale(1.0f, 1.0f, 1.0f);
		FTransform MyTransform(Rotation, Location, Scale);


		UWorld* World = GetWorld();
		//Ch1 = World->SpawnActor<ARunCharacter>(CharacterClass, MyTransform);
		*/
	}
}

void AMindFluxGameModeBase::CreateInitialFloorTiles() 
{
	AFloorTile* Tile = AddFloorTile();
	if (Tile) {
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	
	for (float Val : LaneSwitchValues) {
		UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	}

	for (int i = 0; i < NumInitialFloorTiles - 1; ++i) {
		AddFloorTile();
	}
}

AFloorTile* AMindFluxGameModeBase::AddFloorTile()
{
	UWorld* World = GetWorld();
	if (World) {
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if (Tile) {
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}