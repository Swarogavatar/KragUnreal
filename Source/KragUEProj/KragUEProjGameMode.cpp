// Copyright Epic Games, Inc. All Rights Reserved.

#include "KragUEProjGameMode.h"
#include "KragUEProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKragUEProjGameMode::AKragUEProjGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
