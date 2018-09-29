// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomFishUI.h"

FString UCustomFishUI::CreateResultString(float size, FString name)
{
	if (size > 0) {
		FString returnstring = "You caught a ";
		FString s = FString::SanitizeFloat(size);
		s.RemoveAt(4, s.Len() - 4, true);
		returnstring += s;
		returnstring += "cm ";
		returnstring += name;
		returnstring += "!";
		returnstring += "\nCast again when you're ready!";
		return returnstring;
	}
	else if (size == -1) {
		return FString("Catch a fish!");
	}
	else if (size == -2) { //technically not a result, but this is the handiest way to set the string
		return FString("Somethings on the line!");
	}
	else if (size == -3) {
		return ("Try again.");
	}
	else {
		return FString("It got away...\nCast again when you're ready!");
	}
}


