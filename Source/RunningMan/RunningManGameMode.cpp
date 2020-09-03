// Copyright Epic Games, Inc. All Rights Reserved.

#include "RunningManGameMode.h"
#include "RunningManCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARunningManGameMode::ARunningManGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
