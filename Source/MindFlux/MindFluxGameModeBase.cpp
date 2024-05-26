// Copyright Epic Games, Inc. All Rights Reserved.

#include "MindFluxGameModeBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "FloorTile.h"
#include "FinishFlag.h"
#include "RunCharacter.h"

void AMindFluxGameModeBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGIN PLAY GAME MODE"));

	if (GetWorld()->IsServer()) {
		//bUseSeamlessTravel = false;
		CurrentLevelName = GetWorld()->GetMapName();
		CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
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

	
	if (auto MapDetail = MapDetails.Find(CurrentLevelName))
	{
		AFloorTile* Tile = nullptr;
		for (int i = 0; i < MapDetail->MaxTileCount; ++i)
		{
	
			i > NumInitialFloorTiles - 3 ? Tile = AddFloorTile(*MapDetail,true) : Tile = AddFloorTile(*MapDetail,false);
			Tile->TileNum = ++TotalTileNum;
		

			if (Tile) {
				LaneSwitchValues.Add(Tile->LeftLane->GetComponentLocation().Y);
				LaneSwitchValues.Add(Tile->CenterLane->GetComponentLocation().Y);
				LaneSwitchValues.Add(Tile->RightLane->GetComponentLocation().Y);
			}
		}
		if (Tile)
		{
			GetWorld()->SpawnActor<AActor>(MapDetail->Flag, NextSpawnPoint);
		}
	}

	
	//for (float Val : LaneSwitchValues) {
		//UE_LOG(LogTemp, Warning, TEXT("LANE VALUE: %f"), Val);
	//}

	//InitialSpawnPoint = NextSpawnPoint;
	//UE_LOG(LogTemp, Warning, TEXT("TRANSOFRM: %d,%d,%d"), NextSpawnPoint.GetLocation.X, NextSpawnPoint.GetLocation.Y, NextSpawnPoint.GetLocation.Z);


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


AFloorTile* AMindFluxGameModeBase::AddFloorTile(FMapsDetail MapDetail,const bool bSpawnItems)
{
	UWorld* World = GetWorld();
	if (World) {
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(MapDetail.Tile, NextSpawnPoint);
		if (Tile) {
			//TilesArray.Push(Tile);
			if (bSpawnItems)
			{
				 Tile->SpawnItems(MapDetail);
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

void AMindFluxGameModeBase::CreateFinishWidgetForAllPlayers(FString Name)
{
	// Tüm PlayerController'ları dolaş
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerController = Iterator->Get();
		if (PlayerController)
		{
			ARunCharacter* PlayerCharacter = Cast<ARunCharacter>(PlayerController->GetPawn());
			if (PlayerCharacter)
			{
				// PlayerCharacter üzerinde işlevi çağır
				PlayerCharacter->CreateFinishWidget(Name);
			}
		}
	}
}
