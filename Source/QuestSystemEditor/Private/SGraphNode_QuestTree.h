#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class SGraphNode_QuestTree : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestTree) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, class UEdGraphNode_QuestTree* InNode);

	virtual void UpdateGraphNode() override;

private:
	void CreateNameArea();

	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

private:
	TSharedPtr<SVerticalBox> NameAreaBox;


};
