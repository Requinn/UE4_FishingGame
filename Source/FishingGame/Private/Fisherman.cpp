// Fill out your copyright notice in the Description page of Project Settings.

#include "Fisherman.h"
#include "FishBobber.h"
#include "CustomFishUI.h"
#include "FishingMinigame.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"


AFisherman::AFisherman() {
	//Load in audio file (soundcue) from project path
	static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader1(TEXT("/Game/LureWaterHit.LureWaterHit"));
	lureOnWater = soundCueLoader1.Object;
	static ConstructorHelpers::FObjectFinder<USoundCue> soundCueLoader2(TEXT("/Game/rodCastSwing.rodCastSwing"));
	rodCastSwing = soundCueLoader2.Object;

	//create the sound component
	_fishAudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("FishingAudio"));
	//don't go immediately
	//_fishAudioComponent->bAutoActivate = false;
	//attach to player
	_fishAudioComponent->AttachTo(RootComponent);
	_fishAudioComponent->bStopWhenOwnerDestroyed = false;
}

void AFisherman::BeginPlay() {
	_controlMode = EInputModeType::Character;
	Super::BeginPlay();
	if (CustomFishUI) {
		CustomFishUI->ToggleResultText(true);
		CustomFishUI->SetResultText(-1, "");
	}
}

void AFisherman::PostInitializeComponents() {
	Super::PostInitializeComponents();
}
// Called to bind functionality to input
void AFisherman::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAxis("MouseX", this, &AFisherman::AdjustFishX);
	InputComponent->BindAxis("MouseY", this, &AFisherman::AdjustFishY);
}

void AFisherman::AdjustFishX(float amount) {
	if (_minigame && _controlMode == EInputModeType::Minigame) {
		_minigame->MoveFish(FVector(0, amount, 0));
	}
}

void AFisherman::AdjustFishY(float amount) {
	if (_minigame && _controlMode == EInputModeType::Minigame) {
		_minigame->MoveFish(FVector(0, 0, amount));
	}
}

//Minigame finished, allow player to do actions
void AFisherman::ResetPlayer() {
	_controlMode = EInputModeType::Character;
	_isReadyToCast = true;
}

//Ticked while charging to increment our power
void AFisherman::ChargeThrow()
{
	_castStrength += GetWorld()->DeltaTimeSeconds * _strengthModifier;
	_castStrength = FMath::Clamp<float>(_castStrength, 0, _maximumStrength);
	//if our strength is over the max charge, automatically cast it out?
	if (_castStrength >= _maximumStrength) {
		CastLine();
	}
}

//Throw the cast a distance based on the charge
void AFisherman::CastLine()
{
	if (_isReadyToCast && _controlMode == EInputModeType::Character) {
		FVector OUT_Velocity = FVector::ZeroVector;
		//FVector launchPoint = 
		_isReadyToCast = false;
		//spawn actor with a velocity based on the charge time
		//angle is 60 up?
		CustomFishUI->ToggleChargeBar(false);

		//Issue here: Can't Calculate a proper return value on the OutValue
		//Resolved: Values too small will cause it to fail

		float calculatedSpeed = CalculateScaledSpeed(_castStrength);
		//TODO: Figure this out, it's being sent out WAY too far, and smaller values won't work it seems?
		//Suggestproctile velocity can't handle too small values, so we inflate them, then divide by that value after
		if (UGameplayStatics::SuggestProjectileVelocity(this, OUT_Velocity, GetActorLocation(), GetActorLocation() + FVector(CalculateScaledDistance(_castStrength), 0, 0) * 500, calculatedSpeed * 500, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace)) {
			UE_LOG(LogTemp, Warning, TEXT("Casted to %s from %s"), *(OUT_Velocity / 100).ToString(), *GetActorLocation().ToString());
			AFishBobber* bobber = GetWorld()->SpawnActor<AFishBobber>(FishBobberBP, GetActorLocation(), GetActorRotation()); //this is only here because we need to check if our cast strength was enough to throw out a line
			if (bobber) {
				bobber->SetCatchWeight(_castStrength);
				bobber->Launch((OUT_Velocity / 500).GetSafeNormal() * calculatedSpeed);
				if (rodCastSwing) {
					_fishAudioComponent->SetSound(rodCastSwing);
				}
				_fishAudioComponent->Play();
				GetWorld()->GetTimerManager().SetTimer(delayTimerHandle, this, &AFisherman::PlayDelayedSound, 1.0, false);
				_isCharging = false;
			}
		}
		//we failed to throw out a line
		else {
			CustomFishUI->ToggleResultText(true);
			CustomFishUI->SetResultText(-3, FString("Try Again.")); //borrow this method to set the text to display
			_isCharging = false;
			_isReadyToCast = true;
		}
	}
}

//play a sound delayed
void AFisherman::PlayDelayedSound() {
	if (lureOnWater) {
		_fishAudioComponent->SetSound(lureOnWater);
	}
	_fishAudioComponent->Play();
}

//return Log2n for throw distance
float AFisherman::CalculateScaledDistance(float charge) {
	float scaledCharge = charge * _scaleValue;
	UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), scaledCharge * FMath::Log2(scaledCharge));
	return scaledCharge * FMath::Log2(scaledCharge);
}

//return nLogn of the charge for a throw speed
float AFisherman::CalculateScaledSpeed(float charge) {
	UE_LOG(LogTemp, Warning, TEXT("Speed: %f"), charge * FMath::Loge(charge));
	return charge * FMath::Loge(charge);
}

void AFisherman::StartCharge()
{
	if (_isReadyToCast) {
		_castStrength = 0;
		_isCharging = true;
		if (CustomFishUI) {
			CustomFishUI->ToggleResultText(false);
			CustomFishUI->ToggleChargeBar(true);
		}
	}
}

//pull in the line and start the minigame
void AFisherman::ReelLine(float fishSize)
{
	_controlMode = EInputModeType::Minigame;
	_minigame = GetWorld()->SpawnActor<AFishingMinigame>(FishMinigameBP, GetActorLocation() + FVector(150, 0, 64), GetActorRotation());
	if (_minigame){
		_minigame->Initialize(fishSize, this);
	}
	//start the minigame
}

void AFisherman::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (_isCharging) {
		ChargeThrow();
	}

	if (_isCaught) {
		_isCaughtTime += DeltaTime;
		if (_isCaughtTime >= _isCaughtTimeThreshold) {
			if (_bobber) {
				_bobber->ResetLine();
			}
		}
	}
}

//get the % of the max charge we can do
float AFisherman::GetCastStrengthPercent() {
	return _castStrength / _maximumStrength;
}

//are we charging?
bool AFisherman::GetCastState()
{
	return _isCharging;
}

//Handle the result of the fishing game
void AFisherman::HandleResult(bool isCaught, float fishSize, FString fishName) {
	if (!isCaught) {
		fishSize = 0;
		fishName = "";
	}
	if (CustomFishUI) {
		CustomFishUI->ToggleResultText(true);
		CustomFishUI->SetResultText(fishSize, fishName);
	}
	ResetPlayer();
}

//called from the bobber to let player know we have a fish to catch
void AFisherman::SetReadyToReel(bool isReady, AFishBobber* bobber = nullptr) {
	_isCaughtTime = 0;
	_isCaught = isReady;
	if (isReady) {
		_bobber = bobber;

		if (CustomFishUI) {
			CustomFishUI->ToggleResultText(true);
			CustomFishUI->SetResultText(-2, FString("Somethings on the line!")); //borrow this method to set the text to display
		}
	}
}

//tell the bobber to pull in, starting the minigame
void AFisherman::StartReel() {
	if (_isCaught) {
		_isCaught = false;
		_bobber->PullLine();
		_bobber = nullptr;
	}
}

/* Deprecated
//build a result string to be fetched by the game ui
///Better way is to tell the CustomFishUI class to do this. Problem is the SetRestultText doesn't seem to work properly?
FString AFisherman::GetResultText()
{
	if (_recentCatchSize > 0) {
		FString returnstring = "You caught a ";
		FString size = FString::SanitizeFloat(_recentCatchSize);
		size.RemoveAt(4, size.Len() - 4, true);
		returnstring += size;
		returnstring += "cm ";
		returnstring += _fishName;
		returnstring += "!";
		return returnstring;
	}
	else if (_recentCatchSize == -1) {
		return FString("Catch a fish!");
	}
	else {
		return FString("It got away...");
	}
}
*/
//Sets the UI to say something about what we just got
//FString AFisherman::SetUIText(float fishSize, FString fishName) {
//}
