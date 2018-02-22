// Author:	Jonathan Keslake
// Version: 28/08/2017

#include "HTNController.h"

// #include "Runtime/Engine/Classes/Engine/Engine.h" 
#include "Runtime/Engine/Public/TimerManager.h"

#include "Navigation/CrowdFollowingComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "Tasks/TaskHub.h"
#include "Tasks/EnemyTask.h"

UTaskHub* AHTNController::TaskHub;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AHTNController class

/** PCIP Constructor - Allow crowd path adjustment */
AHTNController::AHTNController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	// Setup other AI variables

	AITimer = {};
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup Functions

/**
*	Function called when the AHTNController class takes control of a pawn
*	@param InPawn - The pawn to be taken control of, is then cast to a
*					AEnemyCharacter object and set to ControlledCharacter
*/
void AHTNController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);
    if (!CharacterBlackboard)
    {
        Character = new FEnemyBlackboard();
	}
}

/** Function called once the AHTNController instance is set to be destroyed */
void AHTNController::Destroyed()
{
	Super::Destroyed();

	// RemoveFromSquad();

	EmptyTaskList();

	// Manual clean-up of CharacterBlackboard variable
	if (CharacterBlackboard)
	{
		delete CharacterBlackboard;
	}

	if (AITimer.IsValid())
	{
		AITimer.Invalidate();
	}

	// delete TasksToProcess

	// delete TaskQueue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//* Tasks and Replanning */

/** Function called by character destruction to clear up NPC */
void AHTNController::PrepareAIForDestruction()
{
	bIsBeingDestroyed = true;

	// NextTask may be called at an unfortunate time while the character is being destroyed
	// As the character has to go into Knockback in order to be destroyed, which will then have
	// it's animation state set to knockback, so NextTask cannot be called.
}

/**
*	Function which actually determines what the next plan is.
*	Note: This function is used exclusively by the planning thread, and
*	should not be called directly, use RegisterReplan instead
*	@return - The new plan for the AHTNController class
*/
TLinkedList<UPrimitiveTask*>* AHTNController::DetermineNewPlan()
{
	if (!GoalTask || !CharacterBlackboard || !GetControlledCharacter())
	{
		return new TLinkedList<UPrimitiveTask*>();
	}

	FEnemyBlackboard* PlanningBlackboard = new FEnemyBlackboard(*CharacterBlackboard);

	return GoalTask->Decompose(PlanningBlackboard);

	// UE_LOG(CustomGameModeLog, Log, TEXT("Successfully exiting replan"));

}

TLinkedList<UPrimitiveTask*>* AHTNController::DetermineNewPlan(TArray<FVector>& NewLocations)
{
	if (!GoalTask || !CharacterBlackboard || !GetControlledCharacter())
	{
		return new TLinkedList<UPrimitiveTask*>();
	}

	FEnemyBlackboard* PlanningBlackboard = new FEnemyBlackboard(*CharacterBlackboard);

	FVector CharacterLocation = GetControlledCharacter()->GetActorLocation();

	return new TLinkedList<UPrimitiveTask*>();
}

void AHTNController::ApplyNewPlan(TLinkedList<UPrimitiveTask*>* NewPlan, TArray<FVector>& NewLocations)
{
	if (this)
	{
		// If this character is being destroyed ignore the new task list
		if (bIsBeingDestroyed)
		{
			return;
		}

		EmptyTaskList();
		TaskList = NewPlan;
//		PointsToMoveTo = MoveTemp(NewLocations);
		if (CurrentTask)
		{
			if (!CurrentTask->IsTaskBlocking())
			{
				bFinishTaskOnMoveCompleted = false;
				StopMovement();
				NextTask();
			}
		}
		else
		{
			NextTask();
		}
	}

	// After this operation the completed plan cannot be used
	delete NewPlan;
}

/**
*	Function called to apply a new plan to the AHTNController class
*	@return - The new plan for the AHTNController class
*/
void AHTNController::ApplyNewPlan(TLinkedList<UPrimitiveTask*>* NewPlan)
{
	// If this character is being destroyed ignore the new task list
	if (bIsBeingDestroyed)
	{
		return;
	}

	EmptyTaskList();
	TaskList = NewPlan;
	if (CurrentTask)
	{
		if (!CurrentTask->IsTaskBlocking())
		{
			bFinishTaskOnMoveCompleted = false;
			StopMovement();
			NextTask();
		}
	}
	else
	{
		NextTask();
	}

	bIsCurrentlyReplanning = false;
	if (bPendingReplan)
	{
		bPendingReplan = false;
		RegisterReplan();
	}
}

/** Function called to empty the task list */
void AHTNController::EmptyTaskList()
{
	while (TaskList)
	{
		TLinkedList<UPrimitiveTask*>* NextTaskNode = TaskList->GetNextLink();
		delete TaskList;
		TaskList = NextTaskNode;
	}
}

/** Function called to execute the next action in the action stack */
void AHTNController::NextTask()
{
	AEnemyCharacter* controlledChar = GetControlledCharacter();
	if (!controlledChar)
	{
		return;
	}

	if (!GetControlledCharacter()->isAnimStateIdle() || bIsBeingDestroyed)
	{
		return;
	}

	if (TaskList)
	{
		CurrentTask = **TaskList;
		TLinkedList<UPrimitiveTask*>* NextTaskNode = TaskList->GetNextLink();
		delete TaskList;
		TaskList = NextTaskNode;
		if (CurrentTask->Execute(this))
		{
			// NextTask();
			RegisterReplan();
		}
	}
	else 
	{
		FString NameOfTask = FString::Printf(TEXT("DummyTask"));
		CurrentTask = Cast<UPrimitiveTask>(TaskHub->GetTaskByName(NameOfTask));
		RegisterReplan();
	}


// CurrentTask->PrintName();
}

/** Function called to indicate that a Replan should be called*/
void AHTNController::RegisterReplan()
{
	TLinkedList<UPrimitiveTask*>* newPlan = DetermineNewPlan();
	ApplyNewPlan(newPlan);
}

/**
*	Function called to force replan, this is the function that should be bound
*	to terminate a task which blocks replanning
*/
void AHTNController::ForceReplan()
{
	FString NameOfTask = FString::Printf(TEXT("DummyTask"));
	CurrentTask = Cast<UPrimitiveTask>(TaskHub->GetTaskByName(NameOfTask));
	RegisterReplan();
}

/** Function called to set the current task, without causing a replan
@param TaskName - Name of task to be set as the current task
@param bShouldExecute - Whether the task should be executed*/
void AHTNController::SetCurrentTask(FString& TaskName, bool bShouldExecute)
{
	CurrentTask = Cast<UPrimitiveTask>(TaskHub->GetTaskByName(TaskName));

	if (CurrentTask && bShouldExecute)
	{
		CurrentTask->Execute(this);
	}
	else
	{
		FString NameOfTask = FString::Printf(TEXT("DummyTask"));
		CurrentTask = Cast<UPrimitiveTask>(TaskHub->GetTaskByName(NameOfTask));
	}
}

/** Function called to set the current goal task for the AHTNController instance */
void AHTNController::SetGoalTask(UEnemyTask* NewGoalTask)
{
	GoalTask = NewGoalTask;

	// GoalTask->PrintName();

	RegisterReplan();
}

/** @param NewGoalTaskName - String name of new goal task to be set on the character */
void AHTNController::SetGoalTask(FString& NewGoalTaskName)
{
	GoalTask = TaskHub->GetTaskByName(NewGoalTaskName);

	RegisterReplan();
}

