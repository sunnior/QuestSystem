#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class SGraphNode_QuestInstance : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNode_QuestInstance) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UEdGraphNode_QuestInstance* InNode);

	virtual void UpdateGraphNode() override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;

private:
	void CreateNameArea();

	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;

private:
	TSharedPtr<SVerticalBox> NameAreaBox;


};