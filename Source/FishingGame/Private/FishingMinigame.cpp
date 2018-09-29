// Fill out your copyright notice in the Description page of Project Settings.

#include "FishingMinigame.h"
#include "Fisherman.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

//TODO: Run a timer down from X seconds.
//Every tick we are outside the distance, add to a value
//if the value is over the threshold after the X seconds, we return a failure
//if it is under, we return a success

// Sets default values
AFishingMinigame::AFishingMinigame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFishingMinigame::Initialize(float fishSize,AFisherman* fisherman) {
	_fishSize = fishSize;
	_fisherMan = fisherman;
}

void AFishingMinigame::StartGame() {
	_canStartTicking = true;
	_fishBall = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(FName("FishAnalog")));
	RandomizeFishVelocity();
}

// Called when the game starts or when spawned
void AFishingMinigame::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(delayTimerHandle, this, &AFishingMinigame::StartGame, MIN_WAIT_TIME_SECONDS, false);
}

// Called every frame
void AFishingMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_canStartTicking && _timeSinceLastMoveTick >= _currentMoveTickWait) {
		_timeSinceLastMoveTick = 0;
		RandomizeFishVelocity();
	}
	else {
		_timeSinceLastMoveTick += DeltaTime;
	}
	//increment our failure timer outside of the fail case, so that we every time we enter the fail zone we increment.
	if (_canStartTicking && _timeSinceLastFailTick < _failureTickWait) {
		_timeSinceLastFailTick += DeltaTime;
	}
	//if we have our fish active
	if (_fishBall && _canStartTicking) {
		_currentMinigameDuration += DeltaTime;
		if (_currentMinigameDuration >= _minigameDuration) {
			SuccessfulCatch();
		}
		_fishBall->AddRelativeLocation(_currentDirection * (_currentSpeed / 250));
		float distance = FVector::Distance(_fishBall->GetComponentLocation(), GetActorLocation());  //make sure you use GetComponentLocation(), worldspace math here
		//if we are outside the "good space"
		if (distance >= DISTANCE_FROM_CENTER) {
			//if the last time our fail meter ticked was longer than the alloted wait time
			if (_timeSinceLastFailTick >= _failureTickWait) {
				_timeSinceLastFailTick = 0;
				_errorValue += 0.1; //Increments every frame.
			}
			if (_errorValue >= _errorThreshold) {
				FailedCatch();
			}
		}
	}
}

void AFishingMinigame::MoveFish(FVector pullForce) {
	if (_fishBall) {
		_fishBall->AddRelativeLocation(pullForce);
	}
}

void AFishingMinigame::FailedCatch() {
	if (_fisherMan) {
		_fisherMan->HandleResult(false, 0, "");
		UE_LOG(LogTemp, Warning, TEXT("Fail"));
	}
	Destroy();
}

void AFishingMinigame::SuccessfulCatch() {
	if (_fisherMan) {
		_fisherMan->HandleResult(true, _fishSize, GenerateRandomFish(_fishSize));
		UE_LOG(LogTemp, Warning, TEXT("Win"));
	}
	Destroy();
}

//Randomize the speed, direction, and the wait time until next tick
void AFishingMinigame::RandomizeFishVelocity() {
	_currentSpeed = FMath::RandRange(MIN_MOVEMENT, MAX_MOVEMENT) * _fishSize;
	_currentMoveTickWait = FMath::RandRange(0.5f, 3.5f);
	//get a random angle between 0 and 359 rotated around the X axis
	_currentDirection = FVector::UpVector.RotateAngleAxis(FMath::RandRange(0.0f, 359.0f), FVector(1,0,0));
}

//create a fish based on catch size
FString AFishingMinigame::GenerateRandomFish(float size) {
	if (size > 0 && size <= 15) {
		return FString("Smelt");
	}
	if (size > 15 && size <= 30) {
		return FString("Carp");
	}
	if (size > 30 && size <= 45) {
		return FString("Octopus");
	}
	if (size > 45 && size <= 60) {
		return FString("Salmon");
	}
	if (size > 60 && size <= 75) {
		return FString("Shark");
	}
	return FString("");
}

