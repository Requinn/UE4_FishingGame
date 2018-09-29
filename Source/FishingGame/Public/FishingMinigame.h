// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FishingMinigame.generated.h"

class AFisherman;

UCLASS()
class FISHINGGAME_API AFishingMinigame : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingMinigame();
	void Initialize(float fishSize,AFisherman* fisherman);
	//Input from the player to move the fish
	void MoveFish(FVector pullForce);
private:
	UPROPERTY(EditAnywhere, Category = Fishing)
	UStaticMeshComponent* _fishBall;

	AFisherman* _fisherMan;

	const float DISTANCE_FROM_CENTER = 50;
	const float MIN_MOVEMENT = 2.5;
	const float MAX_MOVEMENT = 10;
	float _errorValue = 0;
	const float _errorThreshold = 3.5f;
	float _currentSpeed;
	float _minigameDuration = 10.0f;
	float _currentMinigameDuration = 0;
	FVector _currentDirection;
	float _fishSize;
	const float MIN_WAIT_TIME_SECONDS = 3;
	FTimerHandle delayTimerHandle;
	
	//Handle our movement ticks on the bobber
	bool _canStartTicking = false;
	float _currentMoveTickWait = 0.5;
	float _timeSinceLastMoveTick = 0;

	//handle our ticks for when we are in the fail zone
	const float _failureTickWait = 0.1;
	float _timeSinceLastFailTick = 0.1; //start this off greater than so the meter goes up right as they enter the zone

	void RandomizeFishVelocity();
	void FailedCatch();
	void SuccessfulCatch();
	void StartGame();

	FString GenerateRandomFish(float size);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
