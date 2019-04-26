// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestTreeFactory.generated.h"

/**
 * 
 */
UCLASS()
class UQuestTreeFactory : public UFactory
{
	GENERATED_BODY()
public:
	UQuestTreeFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;

};
