// Fill out your copyright notice in the Description page of Project Settings.


#include "FinishFlag.h"
#include "Components/BoxComponent.h"
#include "MindFluxGameModeBase.h"
#include "RunCharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFinishFlag::AFinishFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(SceneComponent);
	TriggerBox->SetBoxExtent(FVector(32.f, 500.f, 200.f));
	TriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

}

// Called when the game starts or when spawned
void AFinishFlag::BeginPlay()
{
	Super::BeginPlay();

	RunGameMode = Cast<AMindFluxGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	//check(RunGameMode);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFinishFlag::OnTriggerBoxOverlap);
	
}

void AFinishFlag::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld()->IsServer())
	{
		ARunCharacter* Character = Cast<ARunCharacter>(OtherActor);
		RunGameMode->CreateFinishWidgetForAllPlayers(OtherActor->GetName());
	
		//FString UrlString = TEXT("ServerLevel");
		//GetWorld()->ServerTravel(UrlString);
	}
}

