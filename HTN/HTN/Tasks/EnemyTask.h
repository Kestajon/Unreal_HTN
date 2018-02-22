// Author:	Jonathan Keslake

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyTask.generated.h"

struct FEnemyBlackboard;
class USquadHub;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UEnemyTask

/** Baseclass for all tasks within the system */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class TMO_API UEnemyTask : public UObject
{
	GENERATED_BODY()

public:

	/** Convenience function to obtain a task by its' name */
	UFUNCTION(BlueprintCallable)
	static UEnemyTask* GetTaskByName(FString& NameOfTask);

	/** Function to obtain the USquadHub which contains the squad blackboard */
	UFUNCTION(BlueprintCallable)
	static USquadHub* GetSquadHub();

	/**
	*	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing
	*/
	virtual TLinkedList<class UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)
	{ return new TLinkedList<class UPrimitiveTask*>(); };

	/** Function called to print the name of the task, used fordebugging  purposes */
	void PrintName();

	/** @return - String representation of the name of this task */
	virtual FString ReturnName();
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UPrimitiveTask

/** Baseclass for all tasks within the system */
UCLASS()
class TMO_API UPrimitiveTask : public UEnemyTask
{
	GENERATED_BODY()

public:

	/** Function called to execute the primitive task  @return - if character should replan*/
	virtual bool Execute(class AHTNController* Controller) { return false; };

	/** @return - If task should prevent next plan from being called until this task calls next task */
	virtual bool IsTaskBlocking() { return false; };

	/**
	*	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing
	*/
	virtual TLinkedList<UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)
	{
		return new TLinkedList<UPrimitiveTask*>(this);
	};

	/** @return - String representation of the name of this task */
	virtual FString ReturnName() override;
};
