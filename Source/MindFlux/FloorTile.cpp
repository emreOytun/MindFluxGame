// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorTile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RunCharacter.h"
#include "MindFluxGameModeBase.h"

// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorMesh"));
	FloorMesh->SetupAttachment(SceneComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Attach Point"));
	AttachPoint->SetupAttachment(SceneComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Center Lane"));
	CenterLane->SetupAttachment(SceneComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Lane"));
	RightLane->SetupAttachment(SceneComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Lane"));
	LeftLane->SetupAttachment(SceneComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FloorTriggerBox"));
	FloorTriggerBox->SetupAttachment(SceneComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("BEGINPLAY TILE %d"), GetWorld()->IsServer());
	Super::BeginPlay();
	
	RunGameMode = Cast<AMindFluxGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//check(RunGameMode);
	
	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);
}

// Called every frame
//void AFloorTile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	++count;
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("OnTriggerBoxOverlap %d Count %d CharcId: %s"), GetWorld()->IsServer(), count, *RunCharacter->GetName());
	if (RunCharacter) {
		//if (GetWorld()->IsServer() && count == 2) {
		if (GetWorld()->IsServer()) {
			RunGameMode->AddFloorTile(true);
			GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 5.f, false);
		}
	}
}

void AFloorTile::SpawnItems()
{
	if (IsValid(SmallObstacleClass) && IsValid(BigObstacleClass)) {
		SpawnLaneItem(CenterLane);
		SpawnLaneItem(RightLane);
		SpawnLaneItem(LeftLane);
	}
	
}

void AFloorTile::
SpawnLaneItem(UArrowComponent* Lane)
{
	const float RandVal = FMath::FRandRange(0.f, 1.f);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FTransform& SpawnLocation = Lane->GetComponentTransform();

	if (UKismetMathLibrary::InRange_FloatFloat(RandVal,0.5f,0.75f,true,true))
	{
		
		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(SmallObstacleClass, SpawnLocation, SpawnParameters);
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(RandVal, 0.75f, 1.f, true, true))
	{

		AObstacle* Obstacle = GetWorld()->SpawnActor<AObstacle>(BigObstacleClass, SpawnLocation, SpawnParameters);
	}
}


/*
void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("ONTRIGGER %d"), GetWorld()->IsServer());
	++count;
	ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor);
	if (RunCharacter) {
		UE_LOG(LogTemp, Warning, TEXT("CLIENT %d"), GetWorld()->IsServer());
		Server_OnTrigger();
	}
}

bool AFloorTile::Server_OnTrigger_Validate() 
{
	return true;
}

void AFloorTile::Server_OnTrigger_Implementation() 
{
	UE_LOG(LogTemp, Warning, TEXT("SERVER"));
	RunGameMode->AddFloorTile();
	if (count == 2) {
		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 5.f, false);
	}
}
*/

void AFloorTile::DestroyFloorTile()
{
	UE_LOG(LogTemp, Warning, TEXT("DestroyFloorTile %d"), GetWorld()->IsServer());
	this->Destroy();
	if (DestroyHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

}