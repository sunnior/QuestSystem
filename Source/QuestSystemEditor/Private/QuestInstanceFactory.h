// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestInstanceFactory.generated.h"

/**
 * 
 */
UCLASS()
class UQuestInstanceFactory : public UFactory
{
	GENERATED_BODY()
public:
	UQuestInstanceFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;

};
