// Copyright Epic Games, Inc. All Rights Reserved.

#include "ParticleZoomGameMode.h"
#include "ParticleZoomCharacter.h"
#include "UObject/ConstructorHelpers.h"

AParticleZoomGameMode::AParticleZoomGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
