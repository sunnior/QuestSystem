// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestInstanceFactory.h"
#include "QuestInstance.h"
#include "Quest.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

UQuestInstanceFactory::UQuestInstanceFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UQuestInstance::StaticClass();
}

UObject* UQuestInstanceFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	//@todo let user choose parent class
	UQuestInstance* NewBP = CastChecked<UQuestInstance>(FKismetEditorUtilities::CreateBlueprint(UQuest::StaticClass(), InParent, Name, BPTYPE_Normal, UQuestInstance::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext));
	return NewBP;
}
