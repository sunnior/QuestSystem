// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraphNode_QuestTree.generated.h"

/**
 *
 */
UCLASS()
class UEdGraphNode_QuestTree : public UEdGraphNode
{
	GENERATED_BODY()
public:
	void AllocateDefaultPins() override;
};