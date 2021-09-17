// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectStructsCPP.h"
#include "GameFramework/Character.h"
#include "TilePlayerCharacterCPP.generated.h"

UCLASS()
class TILEGENCPP_API ATilePlayerCharacterCPP : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATilePlayerCharacterCPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CanMove;
	int CurrentDirection;
	FVector CurrentLocation;
	FRotator CurrentRotation;
	struct FDCoords PlayerCoords;
	int HeightFromTileFloor;

	FVector CameraDefaultRelativeLocation;
	float DPadUpHoldTime;

	class UCameraComponent* Cam;
	class UPointLightComponent* PointLightComponent;
	class ATIleGameModeCPP* CurrentGameMode;

public:
	//ATIleGameModeCPP* CurrentGameMode1() const;
	void SetCurrentGameMode(ATIleGameModeCPP* const CurrentGameMode);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	void SetCoords(const struct FDCoords& Coord);
	void SetCanMove(const bool& Move);

	UFUNCTION(BlueprintCallable)
	void SetCameraAndCharacterMovementComponents(UCameraComponent* CamCom, UCharacterMovementComponent* CharMoveCom);

	static int MyMod(int A, int B);

	UFUNCTION(BlueprintCallable)
	void SetCharacterDirectionAndRotation(const int& Direction);
	
	void RotateCharacterWithoutLerp();
	void RotateCharacter();
	void MoveCharacterToLocationWithoutLerp(const FVector& NewLocation);
	void SetCharacterCoordsAndStartingLocation(const FVector& StartingLocation, const FDCoords& StartingCoord);
	void TryMoveCharacterATileInDirection(const int& Direction);
	void MoveFromCurrentTileToTheNext(const int& Direction);
	void MoveCharacterFromCurrentLocation(const FVector& NewLocation);
	void TeleportToTileAtCoord(const struct FDCoords& Coord);

	UFUNCTION(BlueprintCallable)
	void DoForwardMovement();
	void TryMoveCharacterForwardATile();

	UFUNCTION(BlueprintCallable)
	void StepForward();
	UFUNCTION(BlueprintCallable)
	void TurnLeft();
	UFUNCTION(BlueprintCallable)
	void TurnRight();
	UFUNCTION(BlueprintCallable)
	void TurnAround();
};
