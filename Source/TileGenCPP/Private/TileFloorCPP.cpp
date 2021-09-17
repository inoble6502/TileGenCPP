// Fill out your copyright notice in the Description page of Project Settings.

#include "TileFloorCPP.h"
#include "TileGameModeCPP.h"
#include "GameplayTagsManager.h"
#include "TilePlayerCharacterCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileFloorCPP::ATileFloorCPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraHeightFromTileFloor = 60;
}

// Called when the game starts or when spawned
void ATileFloorCPP::BeginPlay()
{
	Super::BeginPlay();
	this->SetRefToThisOnGameMode();
}

// Called every frame
void ATileFloorCPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TileVecPoints #: " + FString::FromInt(this->TileVecPoints.Num()));
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TileCoords #: " + FString::FromInt(this->TileCoords.Num()));
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "TileDataStructArray  #: " + FString::FromInt(this->TileDataStructArray.Num()));
}

FDCoords ATileFloorCPP::GetStartingCoords() const
{
	return this->StartingCoord;
}

int ATileFloorCPP::GetCameraHeight() const
{
	return this->CameraHeightFromTileFloor;
}

//needs to return something to be overriden as a func and not an event in BP
bool ATileFloorCPP::MakeTileSideArray_Implementation()
{
	return false;
}

bool ATileFloorCPP::SetTileFloorAndCeilingISM_Implementation()
{
	return false;
}

//generate vector positions based on max dungeon dimensions and tile size
void ATileFloorCPP::SetUpTileVecPointsAndCoords()
{
	const int LastIndex = (this->MaxX * this->MaxY) - 1;
	int x, y;
	FVector v;

	for(int i = 0; i < LastIndex; i++)
	{
		x = i / this->MaxY;
		y = i % this->MaxY;
		v = {
			UKismetMathLibrary::Conv_IntToFloat(x * this->TileSize),
			UKismetMathLibrary::Conv_IntToFloat(y * this->TileSize),
			0.0
			};
		
		this->TileVecPoints.Add(v);
		this->TileCoords.Add({x, y});
	}

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "VecPoints Num: " + FString::FromInt(this->TileVecPoints.Num()));
}

//load data from datatable to construct map with
void ATileFloorCPP::SetUpDataFromTable()
{
	if(!this->DungeonTileDataTable)
		return;

	TArray<FName> RowNames;
	FString Context;
	
	RowNames = this->DungeonTileDataTable->GetRowNames();

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, "RowNames #: " + FString::FromInt(RowNames.Num()));

	this->NumberOfTiles = 0;
	this->TileDataStructArray.Empty();
	
	for(auto& name: RowNames)
	{
		FInputCSVTileData* d = this->DungeonTileDataTable->FindRow<FInputCSVTileData>(name, Context);
		if(d)
		{
			this->NumberOfTiles++;
			FTileData t = {
						d->TType,
		{d->TSideN, d->TSideE, d->TSideS, d->TSideW},
		{d->TDoorN, d->TDoorE, d->TDoorS, d->TDoorW},
		{d->TCoordX, d->TCoordY},
		{d->TTelX, d->TTelY},
		{d->Misc0, d->Misc1, d->Misc2, d->Misc3, d->Misc4, d->Misc5, d->Misc6, d->Misc7},
			};

			t.ISMIndexes.Init(-1, 10);
			this->TileDataStructArray.Add(t);
		}
	}
}

bool ATileFloorCPP::DoesTileAtIndexExist(const int& Index)
{
	if(Index > this->NumberOfTiles)
		return false;
	
	return this->TileDataStructArray[Index].TileType;
}

bool ATileFloorCPP::DoesTileAtCoordExist(const FDCoords& Coords)
{
	if(this->GetTileAtCoord(Coords)->TileType > 0)
		return true;

	return false;
}

void ATileFloorCPP::CreateIndividualTileGraphics(const int& Index)
{	
	if(this->TileDataStructArray[Index].TileType == 0)
		return;
	
	this->CreateTileFloor(Index);
	this->CreateTileCeiling(Index);
	
	this->CreateTileSide(0, Index); //north
	this->CreateTileSide(1, Index); //east
	this->CreateTileSide(2, Index); //south
	this->CreateTileSide(3, Index); //west
}

void ATileFloorCPP::CreateTileFloor(const int& Index)
{
	if(!this->ISMTileFloor)
		return;

	const FVector v = this->TileVecPoints[Index];
	const int32 r = this->ISMTileFloor->AddInstanceWorldSpace(FTransform(FRotator(0.0, 0.0, 0.0), v, FVector(1.0, 1.0, 1.0)));
	this->TileDataStructArray[Index].ISMIndexes[0] = r;
}

void ATileFloorCPP::CreateTileCeiling(const int& Index)
{
	if(!this->ISMTileCeiling)
		return;

	const FVector v = this->TileVecPoints[Index];
	const int32 r = this->ISMTileCeiling->AddInstanceWorldSpace(UKismetMathLibrary::Conv_VectorToTransform(v));
	this->TileDataStructArray[Index].ISMIndexes[1] = r;
}

void ATileFloorCPP::CreateTileSide(const int& Side, const int& Index)
{
	const FVector DoorTransforms_[4] =
		{
		{-50.0, -100.0, 0.0},
		{100.0, -50.0, 0.0},
		{50.0, 100.0, 0.0},
		{-100.0, 50.0, 0.0}
		};
	
	const int s = this->TileDataStructArray[Index].TileSides[Side];

	//create instances of meshes and add them to tiledata's array of indexes
	if(s == 3) 
	{
		//doorway
		this->TileDataStructArray[Index].ISMIndexes[Side + 2] =
			this->ISMTileSideArray[2]->AddInstanceWorldSpace(
				FTransform(
					FRotator(0.0, Side * 90.f, 0.0 ),
					FVector(this->TileVecPoints[Index]),
					FVector(1.0, 1.0, 1.0))
					);
		
		//door
		this->TileDataStructArray[Index].ISMIndexes[Side + 6] =
			this->ISMTileSideArray[3]->AddInstanceWorldSpace(
			FTransform(
				FRotator(0.0, Side * 90.f, 0.0 ),
				FVector(this->TileVecPoints[Index] + DoorTransforms_[Side]),
				FVector(1.0, 1.0, 1.0))
				);
	}
	else
	{
		//other sides values
		this->TileDataStructArray[Index].ISMIndexes[Side + 2] =
			this->ISMTileSideArray[s]->AddInstanceWorldSpace(
				FTransform(
				FRotator(0.0, Side * 90.f, 0.0),
				FVector(this->TileVecPoints[Index]),
				FVector(1.0, 1.0, 1.0))
				);
	}
}

FTileData* ATileFloorCPP::GetTileAtCoord(const FDCoords& Coord)
{
	const int i = this->TileCoords.Find(Coord);
	return &this->TileDataStructArray[i];
}

FTileData* ATileFloorCPP::GetTileAtIndex(const int& Index)
{
	return &this->TileDataStructArray[Index];
}

FVector ATileFloorCPP::GetTileLocationVecAtCoord(const FDCoords& Coord)
{	
	const int i = this->TileCoords.Find(Coord);

	if(!this->TileVecPoints.Num() || i > this->TileVecPoints.Num())
		return FVector(-1, -1, -1);
	
	return this->TileVecPoints[i];
}

int ATileFloorCPP::GetTileTypeAtIndex(const int& Index)
{
	return this->TileDataStructArray[Index].TileType;
}

int ATileFloorCPP::GetTileDoorIndex(const FTileData& Tile, const int& Direction)
{
	if(Direction > 3 || Direction < 0)
		return -1;

	return Tile.ISMIndexes[Direction + 6];
}

void ATileFloorCPP::PrintTileInfo(const FTileData& TileData)
{
	FString Message = "";

	Message += "Tile At Coord. X: " + FString::FromInt(TileData.Coords.CoordX)
	+ " Y: " + FString::FromInt(TileData.Coords.CoordY);

	Message += "\n";
	
	Message += "N: " + FString::FromInt(TileData.TileSides[0])
	+ " E: " + FString::FromInt(TileData.TileSides[1])
	+ " S: " + FString::FromInt(TileData.TileSides[2])
	+ " W: " + FString::FromInt(TileData.TileSides[3]);

	if(TileData.TileType == 4)
	{
		Message += "Teleport Coords. X: " + FString::FromInt(TileData.TeleportCoords.CoordX)
		+ " Y: " + FString::FromInt(TileData.TeleportCoords.CoordY);
		Message += "\n";
	}

	Message += "Misc: ";

	for(int i = 0; i < 8; i++)
	{
		Message += " [" + FString::FromInt(i) + "] : " + FString::FromInt(TileData.MiscData[i]) + "\n";
	}
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::White,
		Message
		);

}

void ATileFloorCPP::TeleportCharacterToCoord(ATilePlayerCharacterCPP* Character, const FDCoords& Coord)
{
	FVector newLoc = this->GetTileLocationVecAtCoord(Coord);
	newLoc.Z = this->CameraHeightFromTileFloor * 1.f;
	Character->MoveCharacterToLocationWithoutLerp(newLoc);
	Character->SetCoords(Coord);
	this->TriggerTileEffect(Character, Coord);
}

bool ATileFloorCPP::IsTileAtCoordPassableAtDirection(const int& Direction, const FDCoords& Coord)
{
	const int Side = this->GetTileAtCoord(Coord)->TileSides[Direction];

	switch(Side)
	{
		case 0:
		case 2:
		case 5:
		case 3:
			return true;
		default:
			return false;
	}
}

void ATileFloorCPP::TriggerTileEffect(ATilePlayerCharacterCPP* Character, const FDCoords& Coord)
{
	//do whatever tile effect here
	this->GetTileAtCoord(Coord);

	Character->SetCanMove(true);
}

void ATileFloorCPP::CreateTileFloorBasic()
{
	if(!this->ISMTileFloor)
		return;
	
	this->ISMTileFloor->AddInstanceWorldSpace(
		FTransform(
			FRotator(0.0, 0.0, 0.0),
			FVector(0.0, 0.0, 0.0),
			FVector(1.0, 1.0, 1.0)
			));
}

void ATileFloorCPP::ClearFloorInstances()
{
	if(!this->ISMTileFloor)
		return;

	this->ISMTileFloor->ClearInstances();
}

void ATileFloorCPP::ClearCeilingInstances()
{
	if(!this->ISMTileCeiling)
		return;

	this->ISMTileCeiling->ClearInstances();
}

void ATileFloorCPP::ClearSideInstances()
{
	int n = this->ISMTileSideArray.Num();

	if(!n)
		return;

	for(int i = 0; i < n; i++)
	{
		UInstancedStaticMeshComponent* t = this->ISMTileSideArray[i];
		
		if(t)
		{
			t->ClearInstances();
		}
	}	
}

void ATileFloorCPP::ResetFloor()
{
	this->ClearFloorInstances();
	this->ClearCeilingInstances();
	this->ClearSideInstances();

	this->TileVecPoints.Empty();
	this->TileCoords.Empty();
	this->TileDataStructArray.Empty();

	this->NumberOfTiles = 0;
}

void ATileFloorCPP::OnConstruction(const FTransform& Transform)
{
	//this->ResetFloor();
	
	this->MakeTileSideArray(); //implemented in BP
	this->SetTileFloorAndCeilingISM(); //implemented in BP
	this->SetUpTileVecPointsAndCoords();
	this->SetUpDataFromTable();
	
	const int n = this->NumberOfTiles - 1;
	
	for(int i = 0; i < n; i++)
	{
		this->CreateIndividualTileGraphics(i);
	}
}

void ATileFloorCPP::SetRefToThisOnGameMode()
{
	ATIleGameModeCPP *gm = Cast<ATIleGameModeCPP>(UGameplayStatics::GetGameMode(GetWorld()));

	if(!gm)
		return;

	gm->SetTileFloorPtr(this);
}