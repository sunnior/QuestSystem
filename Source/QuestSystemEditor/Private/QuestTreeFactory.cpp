// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestTreeFactory.h"
#include "QuestTree.h"

UQuestTreeFactory::UQuestTreeFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestTree::StaticClass();
}

UObject* UQuestTreeFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	//@todo let user choose parent class
	UObject* Obj = NewObject<UObject>(InParent, Class, Name, Flags);
	return Obj;
}
