// Copyright Epic Games, Inc. All Rights Reserved.

#include "LostSoulGameMode.h"
#include "LostSoulCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALostSoulGameMode::ALostSoulGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
