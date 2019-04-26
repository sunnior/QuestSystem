// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTree.generated.h"

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UQuestTree : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA

	/** Graph for Behavior Tree */
	UPROPERTY()
	class UEdGraph*	ObjGraph;
#endif
	
};
