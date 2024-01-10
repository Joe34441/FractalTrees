// Copyright Epic Games, Inc. All Rights Reserved.

#include "FractalTreeGameMode.h"
#include "FractalTreeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFractalTreeGameMode::AFractalTreeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
