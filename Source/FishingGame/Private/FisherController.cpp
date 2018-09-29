// Fill out your copyright notice in the Description page of Project Settings.

//Does this class do anything?
#include "FisherController.h"
#include "Fisherman.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
AFisherman* AFisherController::GetControlledFisherman() const {
	return Cast<AFisherman>(GetPawn());
}

// Sets default values
AFisherController::AFisherController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ControlledFisherman = GetControlledFisherman();
}

//Set true if a fish is caught
void AFisherController::SetIsCaught(bool Caught)
{

}

// Called when the game starts or when spawned
void AFisherController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFisherController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (ControlledFisherman->_isCharging) {
		//ControlledFisherman->ChargeThrow();
	//}
}

/*
// Called to bind functionality to input
void AFisherController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/

