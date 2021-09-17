// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGameModeCPP.h"
#include "TileFloorCPP.h"
#include "TilePlayerCharacterCPP.h"
#include "ProjectStructsCPP.h"
#include "Kismet/GameplayStatics.h"

ATIleGameModeCPP::ATIleGameModeCPP()
{
	this->DefaultPawnClass = ATilePlayerCharacterCPP::StaticClass();
}

void ATIleGameModeCPP::BeginPlay()
{
	Super::BeginPlay();
	
	// if(this->IsFloorPtrSet && this->TileFloorPtr)
	// 	this->SpawnPlayerAtStartingCoord();
}

//spawn player at location given the TileFloor's starting coord
void ATIleGameModeCPP::SpawnPlayerAtStartingCoord()
{
	if(!TileFloorPtr)
		return;
	
	FDCoords c = this->TileFloorPtr->GetStartingCoords();
	int h = this->TileFloorPtr->GetCameraHeight();
	
	FVector spawnLoc = this->TileFloorPtr->GetTileLocationVecAtCoord(c);
	spawnLoc.Z = h;
	
	APlayerController* playCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	FActorSpawnParameters p;
	p.Owner = playCon;

	ATilePlayerCharacterCPP* playChar = GetWorld()->SpawnActor<ATilePlayerCharacterCPP>(spawnLoc, FRotator(0.0, 0.0, 0.0), p);
	playChar->SetCharacterCoordsAndStartingLocation(spawnLoc, c);
	playChar->SetCurrentGameMode(this);
	
	playCon->Possess(playChar);
	playChar->SetCanMove(true);
}

ATileFloorCPP* ATIleGameModeCPP::GetTileFloorPtr() const
{
	return TileFloorPtr;
}

void ATIleGameModeCPP::SetTileFloorPtr(ATileFloorCPP* const FloorPtr)
{
	this->TileFloorPtr = FloorPtr;
	
	if(this->TileFloorPtr)
		this->IsFloorPtrSet = true;
}
