// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HRAIController.h"
#include "HRAI.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AHRAIController::AHRAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Blueprint/Ghost/BB_GhostCharacter.BB_GhostCharacter'"));
	if (nullptr != BBAssetRef.Object)
	{
		BBasset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Blueprint/Ghost/BT_GhostCharacter.BT_GhostCharacter'"));
	if (nullptr != BTAssetRef.Object)
	{
		BTasset = BTAssetRef.Object;
	}

}

void AHRAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBasset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTasset);
		ensure(RunResult);		// assertion
	}

}

void AHRAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTComponent)
	{
		BTComponent->StopTree();
	}
}

void AHRAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();



}
