// Fill out your copyright notice in the Description page of Project Settings.

#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MindFluxGameModeBase.h"
#include "ARunPC.h"

// Sets default values
ARunCharacter::ARunCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm")); // We can create components we see in BP_Character to C++ code like that.
	CameraArm->TargetArmLength = 350.f;
	CameraArm->SocketOffset = FVector(0.f, 0.f, 100.f);
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->SetupAttachment(GetRootComponent());	// CameraArm should be under CapsuleComponent as we can see in BP_Character hiearchy.

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false; // Camera is not controlled by controller. It is controlled from CameraArm.
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);

	PortName = "/dev/ttyUSB0"; // Default port name, can be changed in editor
    BaudRate = B9600;          // Default baud rate, can be changed in editor
    Serial = nullptr;
	PortBlue = "/dev/rfcomm0";
	nanoBaudRate = B9600;
	BraceletPort = nullptr;
	FilePath = "/tmp/myfifo";
	//InitializeBluetooth();
	VisionFile = nullptr;
	//InitializeSerialPort();
	//InitializeVision();

}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	//UE_LOG(LogTemp, Warning, TEXT("BEGINPLAY CHARAC %d"), GetWorld()->IsServer());
	Super::BeginPlay();
	RunGameMode = Cast<AMindFluxGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	bReplicates = true;
	CountCharacters = true;
}
void ARunCharacter::ReadFileData()
{
    if (VisionFile && VisionFile->IsOpenVision())
    {
        char Buffer[3];
        ssize_t BytesRead = VisionFile->ReadVision(Buffer, sizeof(Buffer) - 1);
        if (BytesRead > 0)
        {
            Buffer[BytesRead - 1] = '\0';
            FString Data(Buffer);
            UE_LOG(LogTemp, Log, TEXT("Received data: %s"), *Data);
            ProcessVisionData(Data);
        }
    }
}

void ARunCharacter::InitializeSerialPort()
{
    Serial = new SerialPort(TCHAR_TO_UTF8(*PortName), BaudRate);

    if (Serial->Open())
    {
        UE_LOG(LogTemp, Warning, TEXT("Serial port opened successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to open serial port"));
    }

    // Start a timer to read data periodically
    //GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { ReadSerialData(); });
}

void ARunCharacter::InitializeVision()
{
    VisionFile = new SerialPort(TCHAR_TO_UTF8(*FilePath));
	UE_LOG(LogTemp, Log, TEXT("FIFO PATH %s"), *FilePath);
    if (VisionFile->OpenVision())
    {
        UE_LOG(LogTemp, Warning, TEXT("Vision file opened successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to open vision port"));
    }

    // Start a timer to read data periodically
    //GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { ReadSerialData(); });
}

void ARunCharacter::ReadSerialData()
{
    if (Serial && Serial->IsOpen())
    {
        char Buffer[6];
        ssize_t BytesRead = Serial->Read(Buffer, sizeof(Buffer) - 1);
        if (BytesRead > 0)
        {
            Buffer[BytesRead - 1] = '\0';
            FString Data(Buffer);
            UE_LOG(LogTemp, Log, TEXT("Received data: %s"), *Data);
            ProcessJoystickInput(Data);
        }
    }

    // Schedule the next read
    //GetWorld()->GetTimerManager().SetTimerForNextTick([this]() { ReadSerialData(); });
}

void ARunCharacter::ProcessVisionData(const FString& InputData)
{
	int32 command = FCString::Atoi(*InputData);
    switch (command)
    {
        case 1:
            MoveRight();
            break;
        case 2:
            MoveLeft();
            break;
        case 3:
            ARunCharacter::Jump();
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown command: %d"), command);
            break;
    }
	UE_LOG(LogTemp, Warning, TEXT("command: %d"), command);
}

void ARunCharacter::InitializeBluetooth()
{
	UE_LOG(LogTemp, Error, TEXT("Initialize bluetooth icindeyim"));

	BraceletPort = new SerialPort(TCHAR_TO_UTF8(*PortBlue), nanoBaudRate , 5);
    if (!BraceletPort->OpenBluetooth())  // Assuming rfcomm0 is the Bluetooth port
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to open Bluetooth port"));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Bluetooth port opened successfully"));
    }
}


void ARunCharacter::ReadBluetoothDataAsync()
{
        if (BraceletPort && BraceletPort->IsOpenBluetooth())
        {
            char Buffer[100] = {0};
            ssize_t BytesRead = BraceletPort->ReadBluetooth(Buffer, sizeof(Buffer) - 1);
            if (BytesRead > 0)
            {
                Buffer[BytesRead] = '\0';  // Ensure null-termination
                FString Data(Buffer);

                // Since we're in another thread, queue processing back on the main thread
                    ProcessBluetoothInputAsync(Data);

            }
        }
}
void ARunCharacter::ProcessJoystickInput(const FString& InputData)
{
    TArray<FString> ParsedData;
    InputData.ParseIntoArray(ParsedData, TEXT(","), true);

    if (ParsedData.Num() == 2)
    {
        FString Direction = ParsedData[0];
        int32 Jump = FCString::Atoi(*ParsedData[1]);

		if (Direction == "A")
        {
            MoveLeft();
        }
        else if (Direction == "D")
        {
            MoveRight();
        }
        if (Jump == 1)
        {
		ACharacter::Jump();
        }
        UE_LOG(LogTemp, Log, TEXT("Direction = %s, Jump = %d"),*ParsedData[0],Jump);
    }
}

void ARunCharacter::ProcessBluetoothInputAsync(const FString& InputData)
{
    // Asynchronously process the Bluetooth input
        TArray<FString> Components;
        InputData.ParseIntoArray(Components, TEXT(","), true);

        int32 X = 0, Y = 0, Z = 0;

        // Extract values from the parsed data
        for (FString& Component : Components)
        {
            Component = Component.TrimStartAndEnd();  // Trim whitespace

            if (Component.StartsWith(TEXT("X = ")))
            {
                FString XValue = Component.Mid(3);  // Adjust index to correctly skip 'X = '
                X = FCString::Atoi(*XValue);
            }
            else if (Component.StartsWith(TEXT("Y = ")))
            {
                FString YValue = Component.Mid(3);  // Adjust index to correctly skip 'Y = '
                Y = FCString::Atoi(*YValue);
            }
            else if (Component.StartsWith(TEXT("Z = ")))
            {
                FString ZValue = Component.Mid(3);  // Adjust index to correctly skip 'Z = '
                Z = FCString::Atoi(*ZValue);
            }
        }
		UE_LOG(LogTemp, Log, TEXT("X = %d, Y = %d, Z= %d"),X,Y,Z);
        // Queue the result to be handled on the main thread
            HandleBluetoothCommand(X, Y, Z);
}

void ARunCharacter::HandleBluetoothCommand(int32 X, int32 Y, int32 Z)
{
    if (X != 0)
    {
        // X controls horizontal movement: X > 0 -> Right, X < 0 -> Left
        if (X >= 1)
        {
            MoveRight();
        }
        else if (X <= -1)
        {
            MoveLeft();
        }
    }

    // Optionally handle Z for jumping here, ensure it's done in a way compatible with UE's threading model
     if (Z > 5)
    {
        ACharacter::Jump();
    }
}
void ARunCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	// Call the Super
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Add properties to replicated for the derived class
	DOREPLIFETIME(ARunCharacter, TotalCharacters);
}

// Called every frame
void ARunCharacter::Tick(float DeltaTime)
{
	//if (GetWorld()->IsServer())
		//UE_LOG(LogTemp, Warning, TEXT("TOTAL CHARACTERS %d IS_SERVER: %d, %d, %d"), TotalCharacters, GetWorld()->IsServer(), CountCharacters, RunGameMode->SetCountAsGameStateToStart);
	if (GetWorld()->IsServer() && CountCharacters) {
		if (RunGameMode->SetCountAsGameStateToStart) {
			TotalCharacters = RunGameMode->CountPlayers();

			if (TotalCharacters >= 1) {
				CountCharacters = false;
			}
			//UE_LOG(LogTemp, Warning, TEXT("TOTAL CHARACTERS %d IS_SERVER: %d"), TotalCharacters, GetWorld()->IsServer());
		}
		//UE_LOG(LogTemp, Warning, TEXT("TOTAL CHARACTERS %d"), TotalCharacters);
	}
	

	//if (GetWorld()->IsServer()) {
		//UE_LOG(LogTemp, Warning, TEXT("TICK CHARAC %d"), GetWorld()->IsServer());
		Super::Tick(DeltaTime);
		//ReadBluetoothDataAsync();
		//ReadSerialData();
		//ReadFileData();
		if (TotalCharacters >= 1) {
			FRotator ControlRot = GetControlRotation();
			ControlRot.Roll = 0.f;
			ControlRot.Pitch = 0.f;

			//UE_LOG(LogTemp, Warning, TEXT("TOTAL CHARACTERS %d IS_SERVER: %d"), TotalCharacters, GetWorld()->IsServer());
		
			// ControlRot.Vector() == For getting forward vector
			AddMovementInput(ControlRot.Vector());
		}
	//}
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ARunCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ARunCharacter::TouchEnded);

	PlayerInputComponent->BindAction("MoveLeft", IE_Pressed, this, &ARunCharacter::MoveLeft);
	PlayerInputComponent->BindAction("MoveRight", IE_Pressed, this, &ARunCharacter::MoveRight);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &ARunCharacter::MoveDown);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ARunCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ARunCharacter::StopJumping);
}


void ARunCharacter::AddCoin() {
	float ZPosition = GetActorLocation().Z;

	if (ZPosition > 200.f) {
		TotalCoins += 2; // Add two coins if the character is above 200 units in height
	}
	else {
		TotalCoins++; // Otherwise, add just one coin
	}
}

void ARunCharacter::Fly() {
	
	UE_LOG(LogTemp, Warning, TEXT("Got Fly Item"));
	ServerFly_OnTrigger_Implementation(0);

}

void ARunCharacter::EndFly()
{
	ServerFly_OnTrigger_Implementation(1);
}


bool ARunCharacter::ServerFly_OnTrigger_Validate(bool isEnd)
{
	return true;
}


void ARunCharacter::ServerFly_OnTrigger_Implementation(bool isEnd) {

	if (!isEnd) {

		

		// Disable gravity for the duration of the flight
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->GravityScale = 0.0f; // Disable gravity
		}

		// Define how high the character should fly and for how long.
		float FlyHeight = 300.0f; // Height above the ground
		float FlyDuration = 5.0f; // Duration of the flight in seconds

		FVector CurrentLocation = GetActorLocation();
		FVector FlyLocation = CurrentLocation + FVector(0.0f, 0.0f, FlyHeight);

		// Set the new location with flying height.
		SetActorLocation(FlyLocation, true);

		// Set a timer to end flying after FlyDuration seconds.
		GetWorldTimerManager().SetTimer(FlyTimerHandle, this, &ARunCharacter::EndFly, FlyDuration, false);
	}
	else {
		// Restore gravity to its original setting
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->GravityScale = 1.0f; // Restore gravity
		}

		// Return to the original location or reduce the Z-coordinate by the fly height.
		FVector CurrentLocation = GetActorLocation();
		FVector GroundLocation = CurrentLocation - FVector(0.0f, 0.0f, 300.0f); // Adjust height as needed

		SetActorLocation(GroundLocation, true);

		// Clear the timer
		GetWorldTimerManager().ClearTimer(FlyTimerHandle);
	}
}


void ARunCharacter::GetImageProcessing() {
	UE_LOG(LogTemp, Warning, TEXT("Got Image Processing Item"));
}

void ARunCharacter::SetHandwrist() {
	UE_LOG(LogTemp, Warning, TEXT("Got Handwrist Item"));
}


void ARunCharacter::MoveLeft() 
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVELEFT %d"), GetWorld()->IsServer());
	Server_OnTrigger(0);
}

void ARunCharacter::MoveRight() 
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVERIGHT %d"), GetWorld()->IsServer());
	Server_OnTrigger(1);
}

void ARunCharacter::MoveDown() 
{
	//UE_LOG(LogTemp, Warning, TEXT("MOVE DOWN WAS PRESSED"));
}

// float Value: Comes from timeline. It gives where it is in the timeline
// FMath::Lerp: It interpolates from current position to next position according to the given timeline value.
void ARunCharacter::ChangeLaneUpdate(float Value) {
	FVector Location = GetCapsuleComponent()->GetComponentLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

void ARunCharacter::ChangeLaneFinished() {
	CurrentLane = NextLane;
}

void ARunCharacter::OnDeath()
{
	bIsDead = false;
	TotalCharacters = 0; // Client sets it to 0 immediately also

	/*
	if (RestartTimerHandle.IsValid()) {
		GetWorldTimerManager().ClearTimer(RestartTimerHandle);
	}
	*/
	
	if (GetWorld()->IsServer()) {
		Client_OnTrigger();

		//RunGameMode->DestroyTiles();

		//RunGameMode->AddFloorTileAfterRespawn(false);
		//RunGameMode->TotalTileNum = RunGameMode->NumInitialFloorTiles;

		//RunGameMode->NextSpawnPoint = RunGameMode->InitialSpawnPoint;
		//	RunGameMode->AddFloorTileAfterRespawn(false);
		//	RunGameMode->AddFloorTile(false);
		//	RunGameMode->CreateInitialFloorTiles();

		//UE_LOG(LogTemp, Warning, TEXT("IS_SERVER %d, LEVEL: %s"), GetWorld()->IsServer(), *GetWorld()->GetName());
		// Reload the current level to restart the game
		//RunGameMode->RestartGame();
		//GetWorld()->ServerTravel(*GetWorld()->GetName());
		//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), true);
	
		//TotalCharacters = 0;
		RunGameMode->SetCountAsGameStateToStart = false;
		int32 PlayerControllerNum = 0;

		// Iterate through player controllers
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			//	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController)
			{
				
				// Get the controlled pawn (character)
				APawn* ControlledPawn = PlayerController->GetPawn();

				//if (ControlledPawn)
				//{
				PlayerControllerNum++;

				// Get the initial spawn location of the character
				//FVector InitialLocation = ControlledPawn->GetActorLocation();
				FVector InitialLocation(50.0f + PlayerControllerNum / 100, 0.0f, 112.000687f);

				// Destroy the current pawn
				if (ControlledPawn) ControlledPawn->Destroy();

				// Check if the CharacterClass is valid
				if (RunGameMode->CharacterClass != nullptr)
				{
					// Spawn the character using the CharacterClass
					ARunCharacter* NewCharacter = GetWorld()->SpawnActor<ARunCharacter>(RunGameMode->CharacterClass, InitialLocation, FRotator::ZeroRotator);
					NewCharacter->TotalCharacters = 0;
						
					if (NewCharacter)
					{
						// Possess the new character with the player controller
						PlayerController->Possess(NewCharacter);

						/*
						// Enable replication for the new character
						NewCharacter->SetReplicates(true);
						NewCharacter->SetReplicateMovement(true);

						

						// Synchronize initial state to the client
						// ...
						*/
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character using CharacterClass"));
					}
						
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("CharacterClass is not set"));
				}
			}
			//}
			//}
			//UE_LOG(LogTemp, Warning, TEXT("After dead count: %d"), PlayerControllerNum);

			GetWorldTimerManager().SetTimer(RunGameMode->RespawnTimerHandle, RunGameMode, &AMindFluxGameModeBase::StartCharactersTicking, 3.f, false);
			//RunGameMode->SetCountAsGameStateToStart = true;
		}
	}
}
void ARunCharacter::Death()
{
	if (!bIsDead) {

		const FVector Location = GetActorLocation();

		UWorld* World = GetWorld();

		if (World) {
			//bIsDead = true;
			DisableInput(nullptr);

			if (DeathParticleSystem) {
				UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
			}

			if (DeathSound) {
				UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
			}

			GetMesh()->SetVisibility(false);

			if (HasAuthority())
			{
				Client_OnTrigger();
				ServerRespawn();
			}
			else
			{
				ServerRespawn();
			}
			//World->GetTimerManager().SetTimer(RestartTimerHandle, this, &ARunCharacter::OnDeath, 1.f);

		}
	}
}
	

bool ARunCharacter::Server_OnTrigger_Validate(bool isRight)
{
	return true;
}

void ARunCharacter::Server_OnTrigger_Implementation(bool isRight)
{
	//UE_LOG(LogTemp, Warning, TEXT("SERVER %d"), GetWorld()->IsServer());
	if (isRight) {
		NextLane = FMath::Clamp(CurrentLane + 1, 0, 2);
		ChangeLane();
	}
	else {
		NextLane = FMath::Clamp(CurrentLane - 1, 0, 2);
		ChangeLane();
	}
}

void ARunCharacter::ServerRespawn_Implementation()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APawn* ControlledPawn = PlayerController->GetPawn();
		FVector InitialLocation(50.0f, 0.0f, 112.000687f);

		if (ControlledPawn)
		{
			auto PlayerToDestroy = Cast<ARunCharacter>(ControlledPawn);
			if (PlayerToDestroy)
			{
				ControlledPawn->Destroy();
			}
		}
		if (RunGameMode->CharacterClass != nullptr)
		{
			ARunCharacter* NewCharacter = GetWorld()->SpawnActor<ARunCharacter>(RunGameMode->CharacterClass, InitialLocation, FRotator::ZeroRotator);
			NewCharacter->TotalCharacters = 0;
			

			if (NewCharacter)
			{
				PlayerController->Possess(NewCharacter);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character using CharacterClass"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CharacterClass is not set"));
		}
	}

	GetWorldTimerManager().SetTimer(RunGameMode->RespawnTimerHandle, RunGameMode, &AMindFluxGameModeBase::StartCharactersTicking, 3.f, false);
}

bool ARunCharacter::ServerRespawn_Validate()
{
	return true; 
}

bool ARunCharacter::Client_OnTrigger_Validate()
{
	return true;
}

void ARunCharacter::Client_OnTrigger_Implementation()
{
	UWorld* World = GetWorld();
	if (World) {
		const FVector Location = GetActorLocation();

		//bIsDead = true;
		DisableInput(nullptr);

		if (DeathParticleSystem) {
			UGameplayStatics::SpawnEmitterAtLocation(World, DeathParticleSystem, Location);
		}

		if (DeathSound) {
			UGameplayStatics::PlaySoundAtLocation(World, DeathSound, Location);
		}
	}
}

void ARunCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchStartLocation = FVector2D(Location.X, Location.Y);
}

void ARunCharacter::TouchEnded(ETouchIndex::Type FingerIndex, FVector Location)
{
	TouchEndLocation = FVector2D(Location.X, Location.Y);

	
	if (TouchStartLocation.Y - TouchEndLocation.Y > 50.0f)
	{
		Jump();
	}
	else if (TouchStartLocation.X - TouchEndLocation.X > 50.0f){
		
		MoveLeft();
		
	}
	else if (TouchEndLocation.X - TouchStartLocation.X > 50.0f) {

		MoveRight();

	}
	else if (TouchEndLocation.Y - TouchStartLocation.Y > 50.0f)
	{
		MoveDown();
	}
}

