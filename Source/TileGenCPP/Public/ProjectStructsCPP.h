// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectStructsCPP.generated.h"

/* ensure struct names are
 * different from BP counterparts
 */

USTRUCT(BlueprintType)
struct FDCoords
{
	GENERATED_USTRUCT_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CoordX; 
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CoordY;

	bool operator==(const FDCoords& Pos) const;
};

USTRUCT(BlueprintType) 
struct FInputCSVTileData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TSideN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TSideE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TSideS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TSideW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TDoorN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TDoorE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TDoorS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TDoorW;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TCoordY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TCoordX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TTelY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TTelX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Misc7;
};

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_USTRUCT_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TileType;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> TileSides;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> TileDoorValues;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDCoords Coords;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDCoords TeleportCoords;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> MiscData;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> ISMIndexes;
};