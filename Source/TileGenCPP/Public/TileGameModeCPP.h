// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TileGameModeCPP.generated.h"

/**
 * 
 */
UCLASS()
class TILEGENCPP_API ATIleGameModeCPP : public AGameMode
{
	GENERATED_BODY()
	
public:
	ATIleGameModeCPP();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void SpawnPlayerAtStartingCoord();

protected:
	bool IsFloorPtrSet;

	UPROPERTY(BlueprintReadWrite)
	class ATileFloorCPP* TileFloorPtr;

public:
	ATileFloorCPP* GetTileFloorPtr() const;

	UFUNCTION(BlueprintCallable)
	void SetTileFloorPtr(ATileFloorCPP* const FloorPtr);
};
