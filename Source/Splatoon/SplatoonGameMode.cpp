// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplatoonGameMode.h"
#include "SplatoonCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASplatoonGameMode::ASplatoonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
