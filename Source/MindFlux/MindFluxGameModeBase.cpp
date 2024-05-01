// Copyright Epic Games, Inc. All Rights Reserved.


#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "MindFluxGameModeBase.h"
#include "FloorTile.h"
#include "RunCharacter.h"

void AMindFluxGameModeBase::BeginPlay()
{
	if (GetWorld()->IsServer()) {
		//bUseSeamlessTravel = false;
		CreateInitialFloorTiles();
		//CountCharacters = true;
		SetCountAsGameStateToStart = true;
		//TotalTileNum 

		//GetWorld()->ServerTravel("MainLevel");
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


void AMindFluxGameModeBase::StartCharactersTicking() {
	SetCountAsGameStateToStart = true;
}


void AMindFluxGameModeBase::CreateInitialFloorTiles() 
{
	int TotalTileNum = 0;

	AFloorTile* Tile = AddFloorTile(false);
	Tile->TileNum = ++TotalTileNum;
	if (Tile) {
		LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
		LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
	}
	
	for (float Val : LaneSwitchValues) {
		//UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	}

	//InitialSpawnPoint = NextSpawnPoint;
	//UE_LOG(LogTemp, Warning, TEXT("TRANSOFRM: %d,%d,%d"), NextSpawnPoint.GetLocation.X, NextSpawnPoint.GetLocation.Y, NextSpawnPoint.GetLocation.Z);


	Tile = AddFloorTile(false);
	Tile->TileNum = ++TotalTileNum;

	Tile = AddFloorTile(false);
	Tile->TileNum = ++TotalTileNum;


	for (int i = 0; i < NumInitialFloorTiles - 3; ++i) {
		Tile = AddFloorTile(true);
		Tile->TileNum = ++TotalTileNum;
	}
}

/*
AFloorTile* AMindFluxGameModeBase::AddFloorTileAfterRespawn(const bool bSpawnItems) {
	FVector InitialLocation(50.0f, 0.0f, 112.000687f);
	UWorld* World = GetWorld();
	if (World) {
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, InitialLocation, FRotator::ZeroRotator);
		if (Tile) {
			if (bSpawnItems)
			{
				 Tile->SpawnItems();
			}

			NextSpawnPoint = Tile->GetAttachTransform();

			Tile->TileNum = ++TotalTileNum;
		}
		return Tile;
	}

	return nullptr;
}
*/


AFloorTile* AMindFluxGameModeBase::AddFloorTile(const bool bSpawnItems)
{
	UWorld* World = GetWorld();
	if (World) {
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);
		if (Tile) {
			//TilesArray.Push(Tile);
			if (bSpawnItems)
			{
				 Tile->SpawnItems();
			}

			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}

void AMindFluxGameModeBase::DestroyTiles() {
	// Iterate through the TilesArray and destroy each tile
	for (AFloorTile* Tile : TilesArray)
	{
		if (Tile && Tile->TileNum > NumInitialFloorTiles)
		{
			Tile->Destroy();
		}
	}

	// Clear the TilesArray
	TilesArray.Empty();
}

int32 AMindFluxGameModeBase::CountPlayers() {
	int32 Total = 0;
	// Iterate through player controllers
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			// Increment the character count if the player controller has a valid pawn
			if (PlayerController->GetPawn())
			{
				if (GetWorld()->IsServer()) {
					Total++;
				}
			}
		}
	}
	return Total;
}