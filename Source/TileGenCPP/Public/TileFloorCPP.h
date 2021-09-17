// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStructsCPP.h"
#include "GameFramework/Actor.h"
#include "TileFloorCPP.generated.h"

UCLASS()
class TILEGENCPP_API ATileFloorCPP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileFloorCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TileSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MaxY;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CameraHeightFromTileFloor;
	UPROPERTY(BlueprintReadWrite)
	int NumberOfTiles;

	UPROPERTY(BlueprintReadWrite)
	TArray<UInstancedStaticMeshComponent*> ISMTileSideArray;
	
	TArray<struct FDCoords> TileCoords;
	TArray<struct FTileData> TileDataStructArray;
	TArray<FVector> DoorTransforms;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FDCoords StartingCoord;

	UPROPERTY(BlueprintReadWrite)
	UInstancedStaticMeshComponent* ISMTileCeiling;

	UPROPERTY(BlueprintReadWrite)
	UInstancedStaticMeshComponent* ISMTileFloor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UDataTable* DungeonTileDataTable;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FDCoords GetStartingCoords() const;
	int GetCameraHeight() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (ForceAsFunction))
	bool MakeTileSideArray();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Meta = (ForceAsFunction))
	bool SetTileFloorAndCeilingISM();

	UFUNCTION(BlueprintCallable)
	void SetUpTileVecPointsAndCoords();

	UFUNCTION(BlueprintCallable)
	void SetUpDataFromTable();

	UFUNCTION(BlueprintCallable)
	bool DoesTileAtIndexExist(const int& Index);

	UFUNCTION(BlueprintCallable)
    bool DoesTileAtCoordExist(const FDCoords& Coords);

	UFUNCTION(BlueprintCallable)
	void CreateIndividualTileGraphics(const int& Index);
	
	void CreateTileFloor(const int& Index);
	void CreateTileCeiling(const int& Index);
	void CreateTileSide(const int& Side, const int& Index);

	struct FTileData* GetTileAtCoord(const struct FDCoords& Coord);
	struct FTileData* GetTileAtIndex(const int& Index);
	FVector GetTileLocationVecAtCoord(const struct FDCoords& Coord);
	int GetTileTypeAtIndex(const int& Index);
	int GetTileDoorIndex(const struct FTileData& Tile, const int& Direction);
	void PrintTileInfo(const struct FTileData& TileData);

	void TeleportCharacterToCoord(class ATilePlayerCharacterCPP* Character, const FDCoords& Coord);
	bool IsTileAtCoordPassableAtDirection(const int& Direction, const FDCoords& Coord);
	void TriggerTileEffect(class ATilePlayerCharacterCPP* Character, const FDCoords& Coord);

	UFUNCTION(BlueprintCallable)
	void CreateTileFloorBasic();

	void ClearFloorInstances();
	void ClearCeilingInstances();
	void ClearSideInstances();

	void ResetFloor();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable)
	void SetRefToThisOnGameMode();

	TArray<FVector> TileVecPoints;
};