// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fisherman.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "FishBobber.generated.h"
 
UCLASS()
class FISHINGGAME_API AFishBobber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishBobber();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void PullLine();
	void ResetLine();
	void SetCatchWeight(float Weight);

	float catchWeight = 0; //chance to catch a fish per tick

	const float MIN_WEIGHT = 15; //the absolute minimum we can roll
	const float MAX_WEIGHT = 100; //maximum and out of how much we roll the catchweight against

	const float MIN_SIZE_MODIFIER = 1.5;
	const float MAX_SIZE_MODIFIER = 2.5;

	void Launch(FVector velocity);

private: 
	float timeSinceLastCheck = 0;
	const float TIME_BETWEEN_CHECK_SECONDS = 1; //how long do we wait between each tick to check 
	bool _canStartTicking = false;
	const float MIN_WAIT_TIME_SECONDS = 5; //minimum time to wait before tickig the %

	UFUNCTION()
	void StartDelayedTick(); //start ticking after the delay
	bool _fishOnLine; //do we have a fish ready to catch?

	//store their size so we can use it
	float _rolledWeight = 0;
	float _rolledSize = 0;

	void CalculateFishChance();

	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;
	FTimerHandle delayTimerHandle;
	AFisherman* _fisherman = nullptr;
	
};
