// Fill out your copyright notice in the Description page of Project Settings.

#include "FishBobber.h"
#include "Fisherman.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AFishBobber::AFishBobber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("MovementComponent"));
	//ProjectileMovementComponent->bAutoActivate = false;

}

// Called when the game starts or when spawned6
void AFishBobber::BeginPlay()
{
	Super::BeginPlay();
	_fisherman = Cast<AFisherman>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//call the delayed tick after 5 seconds
	GetWorld()->GetTimerManager().SetTimer(delayTimerHandle, this, &AFishBobber::StartDelayedTick, MIN_WAIT_TIME_SECONDS, false);
	
}

//set our catch weight, if it's below our minimum weight (15) set it to that
void AFishBobber::SetCatchWeight(float Weight) {
	if (Weight < MIN_WEIGHT) {
		Weight = MIN_WEIGHT;
	}
	catchWeight = Weight;
}

//TODO: Bobber's Velocity isn't being set properly with setInlocalspace. We can do a manual set and update, but that still reverts it to the forward vector.
//Doing it with Velocity = velocity and then updating it assigns a velocty but the object doesn't move
void AFishBobber::Launch(FVector velocity) {
	ProjectileMovementComponent->Velocity = velocity;
	ProjectileMovementComponent->UpdateComponentVelocity();
	//ProjectileMovementComponent->Activate();

	UE_LOG(LogTemp, Warning, TEXT("OwnedObject: %s"), *ProjectileMovementComponent->GetOwner()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Passed In Velocity: %s"), *velocity.ToString());
}

// Called every frame
void AFishBobber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_canStartTicking && timeSinceLastCheck >= TIME_BETWEEN_CHECK_SECONDS) {
		timeSinceLastCheck = 0;
		CalculateFishChance();
	}
	else {
		timeSinceLastCheck += DeltaTime;
	}
}

void AFishBobber::StartDelayedTick() {
	_canStartTicking = true;
}

//Ticked every 1 second to check if we caught a fish
void AFishBobber::CalculateFishChance() {
	_rolledWeight = FMath::FRandRange(0, MAX_WEIGHT);
	if (_rolledWeight <= catchWeight) {
		//let the player know they can reel in the fish now
		UE_LOG(LogTemp, Warning, TEXT("FISH CAUGHT"));
		_rolledSize = FMath::FRandRange(MIN_SIZE_MODIFIER, MAX_SIZE_MODIFIER);
		_fisherman->SetReadyToReel(true, this);
		_canStartTicking = false;
		//play some sound
	}
}

//let the player know we have a fish and are ready to start the minigame
void AFishBobber::PullLine() {
	_fisherman->ReelLine(_rolledWeight * _rolledSize);
	_rolledWeight = 0;
	_rolledSize = 0;
	_fisherman->SetReadyToReel(false, this);
	//this bobber is done doing its job
	Destroy();
}

//we failed to pull in the fish in time to start the minigame
void AFishBobber::ResetLine() {
	_rolledWeight = 0;
	_rolledSize = 0;
	_fisherman->SetReadyToReel(false, this);
	_fisherman->HandleResult(false, 0, "");
	Destroy();
}
