// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomFishUI.generated.h"

/**
 * Used to bridge the fisherman to the UI

 REMEMBER TO MAKE SURE THE PARENT OF THE UI ISNT TOGGLING VISIBILITY 
 */
UCLASS()
class FISHINGGAME_API UCustomFishUI : public UUserWidget
{
	GENERATED_BODY()
public:
	//creates an event node with out params that can be called by another class so this BP can do stuff with it
	UFUNCTION(Category = "FishUI", BlueprintImplementableEvent)
	void SetResultText(const float &size, const FString &fishName);

	UFUNCTION(Category = "FishUI", BlueprintImplementableEvent)
	void ToggleChargeBar(bool isBarActive);

	UFUNCTION(Category = "FishUI", BlueprintImplementableEvent)
	void ToggleResultText(bool isTextActive);

	//caleld by the above event to create the UI string
	UFUNCTION(Category = "FishUI", BlueprintCallable)
	FString CreateResultString(float size, FString name);
};
