// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FisherController.generated.h"

class AFisherman;

UCLASS()
class FISHINGGAME_API AFisherController : public APlayerController
{
	GENERATED_BODY()
private:
	

	AFisherman* GetControlledFisherman() const;

	AFisherman* ControlledFisherman = nullptr;
	
	
	


public:
	// Sets default values for this pawn's properties
	AFisherController();

	//If something is caught on the line, tell us
	UFUNCTION()
	void SetIsCaught(bool Caught);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
