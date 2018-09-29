// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include "CoreMinimal.h"
#include "EInputModeType.h" //enum classes can't be forward declared
#include "SoundDefinitions.h"
#include "GameFramework/Pawn.h"
#include "Fisherman.generated.h"

class AFishBobber;
class AFishingMinigame;
class UCustomFishUI;
class UAudioComponent;
class USoundCue;
/**
 * 
 */
UCLASS()
class FISHINGGAME_API AFisherman : public APawn
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	//While the left mous button is held, charge up the throw
	UFUNCTION(BluePrintCallable, Category = FishInput)
	void ChargeThrow();
	
	//Release the mouse button to cast out the line
	UFUNCTION(BluePrintCallable, Category = FishInput)
	void CastLine();

	UFUNCTION(BluePrintCallable, Category = FishInput)
	void StartCharge();

	UFUNCTION(BlueprintCallable, Category = FishInput)
	void StartReel();

	//When we have something on the line, pull it in
	UFUNCTION(BluePrintCallable, Category = FishInput)
	void ReelLine(float fishSize);

	UFUNCTION(BlueprintPure, Category = FishUtility)
	float GetCastStrengthPercent();
	UFUNCTION(BlueprintPure, Category = FishUtility)
	bool GetCastState();
	//Reset the player and be ready to cast again
	UFUNCTION()
	void ResetPlayer();
	void HandleResult(bool result, float fishSize, FString fishName);
	float currentCastStrengthPercent = 0;

	void SetReadyToReel(bool isReady, AFishBobber* bobber);

	//UFUNCTION(BlueprintPure, Category = FishUtility)
	//FString GetResultText();
	//https://answers.unrealengine.com/questions/173591/c-blueprint-event-with-parameter.html
	//UFUNCTION(Category = FishUtility, BlueprintPure)
	//void SetUIText(const float &size, const FString &fishName);
	//float GetCastStrengthPercent_Implementation();

	//create a reference for the UI that can be set from where we create our UI. in the BP
	UPROPERTY(BlueprintReadWrite, Category = Fishing)
	UCustomFishUI* CustomFishUI;

	//http://bendemott.blogspot.com/2016/10/unreal-4-playing-sound-from-c-with.html
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	USoundCue* lureOnWater;
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	USoundCue* rodCastSwing;
	UPROPERTY(BlueprintReadOnly, Category = FishAudio)
	UAudioComponent* _fishAudioComponent;

	FTimerHandle delayTimerHandle;
private:
	UPROPERTY()
	float _castStrength = 0;
	UPROPERTY()
	bool _isCharging = false;
	//modifies how fast we charge our cast meter
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	float _strengthModifier = 6;
	//what is the maximum strength we can charge to
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	float _maximumStrength = 40;
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	float _scaleValue = 1.75f;


	float _isCaughtTime = 0;
	float _isCaughtTimeThreshold = 5; //how long can we wait before the fish is caught/run away

	EInputModeType _controlMode;
	float CalculateScaledSpeed(float strength);
	float CalculateScaledDistance(float strength);
	UPROPERTY()
	bool _isReadyToCast = true; //are we ready to throw out again
	UPROPERTY()
	bool _isCaught = false; //a fish is on the line? probably not used
	AFisherman();
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void Tick(float DeltaTime) override;
	
	void PlayDelayedSound();

	void AdjustFishX(float amount);
	void AdjustFishY(float amount);
	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	TSubclassOf<AFishBobber> FishBobberBP;
	AFishBobber* _bobber = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Fishing)
	TSubclassOf<AFishingMinigame> FishMinigameBP;
	AFishingMinigame* _minigame = nullptr;
};
