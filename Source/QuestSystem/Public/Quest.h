// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintUpdateUnlock();

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintUpdateComplete();

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintUpdateCancelled();
};
