// Fill out your copyright notice in the Description page of Project Settings.


#include "TilePlayerCharacterCPP.h"
#include "TileGameModeCPP.h"
#include "TileFloorCPP.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "Camera/CameraComponent.h"
#include "ProjectStructsCPP.h"

// Sets default values
ATilePlayerCharacterCPP::ATilePlayerCharacterCPP()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->CameraDefaultRelativeLocation = {-100.0, 0.0, -5.0};
	
	this->CanMove = false;
	this->PlayerCoords = {0, 0};
	this->HeightFromTileFloor = 60;
	
	this->bUseControllerRotationYaw = false;
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;

	this->Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	this->Cam->SetupAttachment(this->GetCapsuleComponent());
	this->Cam->Activate();
	this->Cam->SetRelativeLocation(this->CameraDefaultRelativeLocation);
	this->Cam->SetAspectRatio(2.5);
	this->Cam->SetFieldOfView(95.0);
	this->Cam->bUsePawnControlRotation;

	this->PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	this->PointLightComponent->SetupAttachment(this->GetCapsuleComponent());
	this->PointLightComponent->Activate();
	this->PointLightComponent->SetIntensity(500.0);
}

// Called when the game starts or when spawned
void ATilePlayerCharacterCPP::BeginPlay()
{
	Super::BeginPlay();
	this->SetCharacterDirectionAndRotation(2);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

void ATilePlayerCharacterCPP::SetCurrentGameMode(ATIleGameModeCPP* const GMode)
{
	this->CurrentGameMode = GMode;
}

// Called every frame
void ATilePlayerCharacterCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATilePlayerCharacterCPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	this->InputComponent->BindAction(
		"DungeonMoveTileForward",
		IE_Pressed,
		this,
		&ATilePlayerCharacterCPP::DoForwardMovement
		);

	this->InputComponent->BindAction(
		"DungeonTurnLeft",
		IE_Pressed,
		this,
		&ATilePlayerCharacterCPP::TurnLeft
		);

	this->InputComponent->BindAction(
		"DungeonTurnRight",
		IE_Pressed,
		this,
		&ATilePlayerCharacterCPP::TurnRight
		);

	this->InputComponent->BindAction(
		"DungeonTurnAround",
		IE_Pressed,
		this,
		&ATilePlayerCharacterCPP::TurnAround
		);
}

void ATilePlayerCharacterCPP::OnConstruction(const FTransform& Transform)
{
	//character is constantly floating
	this->GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
}

void ATilePlayerCharacterCPP::SetCoords(const FDCoords& Coord)
{
	this->PlayerCoords = Coord;
}

void ATilePlayerCharacterCPP::SetCanMove(const bool& Move)
{
	this->CanMove = Move;
}

void ATilePlayerCharacterCPP::SetCameraAndCharacterMovementComponents(UCameraComponent* CamCom,
	UCharacterMovementComponent* CharMoveCom)
{
	// this->Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// this->Cam->bAutoActivate = true;
	// this->Cam->Activate();
	// this->Cam->SetupAttachment(this->GetCapsuleComponent());
	// this->Cam->Activate();
	// this->Cam->bAutoActivate = true;
	// this->Cam->SetRelativeLocation(this->CameraDefaultRelativeLocation);
	// this->Cam->SetAspectRatio(2.5);
	// this->Cam->SetFieldOfView(95.0);
	// this->Cam->bUsePawnControlRotation;
	//
	// this->CharMove->Activate();
	// this->CharMove = CharMoveCom;
	// this->CharMove->Activate();
	// this->CharMove->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
	// this->CharMove->Activate();
}

//mod to account for negative results
int ATilePlayerCharacterCPP::MyMod(int A, int B)
{
	const int r = A % B;
	return (r < 0) ? (B + r) : r;
}

//set character rotation given direction
void ATilePlayerCharacterCPP::SetCharacterDirectionAndRotation(const int& Direction)
{
	this->CurrentDirection = Direction;
	this->CurrentRotation = FRotator(0.0, (this->MyMod(this->CurrentDirection, 4) - 1) * 90.f, 0.0);
	this->RotateCharacterWithoutLerp();
}

void ATilePlayerCharacterCPP::RotateCharacterWithoutLerp()
{
	this->SetActorRotation(FRotator(this->CurrentRotation.Pitch, (this->CurrentDirection - 1) * 90.f, this->CurrentRotation.Roll));
	this->CurrentRotation = this->GetActorRotation();
	this->CanMove = true;
}

void ATilePlayerCharacterCPP::RotateCharacter()
{
	FRotator newRot = this->CurrentRotation;
	newRot.Yaw = (this->CurrentDirection - 1) * 90.f;
	this->SetActorRelativeRotation(newRot);
	this->CurrentRotation = this->GetActorRotation();
	this->CanMove = true;
}

void ATilePlayerCharacterCPP::MoveCharacterToLocationWithoutLerp(const FVector& NewLocation)
{
	if(this->SetActorLocation(NewLocation))
		this->CurrentLocation = NewLocation;
}

void ATilePlayerCharacterCPP::SetCharacterCoordsAndStartingLocation(const FVector& StartingLocation, const FDCoords& StartingCoord)
{
	this->MoveCharacterToLocationWithoutLerp(StartingLocation);
	this->PlayerCoords = StartingCoord;
}

void ATilePlayerCharacterCPP::TryMoveCharacterATileInDirection(const int& Direction)
{
	if(!CurrentGameMode)
		return;

	if(this->CurrentGameMode->GetTileFloorPtr()->IsTileAtCoordPassableAtDirection(Direction, this->PlayerCoords))
		MoveFromCurrentTileToTheNext(Direction);
	else
		this->CanMove = false;
}

void ATilePlayerCharacterCPP::MoveFromCurrentTileToTheNext(const int& Direction)
{
	if(!CurrentGameMode)
		return;
	
	this->CanMove = false;

	FDCoords newCoord;

	switch(Direction)
	{
		case 0:
			newCoord = {this->PlayerCoords.CoordX, this->PlayerCoords.CoordY - 1};
			break;
		case 1:
			newCoord = {this->PlayerCoords.CoordX + 1, this->PlayerCoords.CoordY};
			break;
		case 2:
			newCoord = {this->PlayerCoords.CoordX, this->PlayerCoords.CoordY + 1};
			break;
		case 3:
			newCoord = {this->PlayerCoords.CoordX - 1, this->PlayerCoords.CoordY};
			break;
		default:
			return;
	}

	//check if current tile allows for moving in a direction
	if(this->CurrentGameMode->GetTileFloorPtr()->GetTileAtCoord(newCoord)->TileType != 0)
	{
		FVector newLoc = this->CurrentGameMode->GetTileFloorPtr()->GetTileLocationVecAtCoord(newCoord);
		newLoc.Z = this->CurrentGameMode->GetTileFloorPtr()->GetCameraHeight();
		this->MoveCharacterFromCurrentLocation(newLoc);
		this->PlayerCoords = newCoord;
	}
}

void ATilePlayerCharacterCPP::MoveCharacterFromCurrentLocation(const FVector& NewLocation)
{
	if(!CurrentGameMode)
		return;
	
	this->CanMove = false;

	this->SetActorLocation(NewLocation);

	this->CurrentLocation = this->GetActorLocation();
	this->CurrentGameMode->GetTileFloorPtr()->TriggerTileEffect(this, PlayerCoords);
}

void ATilePlayerCharacterCPP::TeleportToTileAtCoord(const FDCoords& Coord)
{
	if(Coord.CoordX > 31 || Coord.CoordY > 31)
		return;

	if(this->CurrentGameMode->GetTileFloorPtr()->DoesTileAtCoordExist(Coord))
	{
		FVector newLoc = this->CurrentGameMode->GetTileFloorPtr()->GetTileLocationVecAtCoord(Coord);
		newLoc.Z = this->CurrentGameMode->GetTileFloorPtr()->GetCameraHeight() * 1.f;
		this->MoveCharacterToLocationWithoutLerp(newLoc);
		this->PlayerCoords = Coord;
		this->CurrentGameMode->GetTileFloorPtr()->TriggerTileEffect(this, Coord);
	}
}

void ATilePlayerCharacterCPP::DoForwardMovement()
{	
	if(!this->CanMove)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "DoForwardMovement");

	this->CanMove = false;
	this->TryMoveCharacterForwardATile();
}

void ATilePlayerCharacterCPP::TryMoveCharacterForwardATile()
{
	if(!this->CurrentGameMode)
		return;

	if(this->CurrentGameMode->GetTileFloorPtr()->IsTileAtCoordPassableAtDirection(this->CurrentDirection, this->PlayerCoords))
		this->MoveFromCurrentTileToTheNext(this->CurrentDirection);
	else
		this->CanMove = true;
}

void ATilePlayerCharacterCPP::StepForward()
{
	this->DoForwardMovement();
}

void ATilePlayerCharacterCPP::TurnLeft()
{
	if(!this->CanMove)
		return;
	
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TurnLeft");
	
	this->CanMove = false;

	this->CurrentDirection = this->MyMod(this->CurrentDirection - 1, 4);
	this->RotateCharacter();
}

void ATilePlayerCharacterCPP::TurnRight()
{
	if(!this->CanMove)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TurnRight");
	
	this->CanMove = false;

	this->CurrentDirection = (this->CurrentDirection + 1) % 4;
	this->RotateCharacter();
}

void ATilePlayerCharacterCPP::TurnAround()
{	
	if(!this->CanMove)
		return;

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TurnAround");

	this->CanMove = false;

	this->CurrentDirection = (this->CurrentDirection + 2) % 4;
	this->RotateCharacter();
}