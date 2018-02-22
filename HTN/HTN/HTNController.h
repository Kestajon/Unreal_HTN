// Author:	Jonathan Keslake
// Version: 28/08/2017


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/Core/Public/Containers/Queue.h"
#include "EnemyAI.generated.h"

struct FEnemyBlackboard;
class AEnemyCharacter;
class UEnemyTask;
class UPrimitiveTask;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  FEnemyBlackboard

/** Blackboard representing the world state */
USTRUCT()
struct FEnemyBlackboard
{
    
    GENERATED_USTRUCT_BODY()
    
    /** Default Constructor */
    FEnemyBlackboard() {};
    
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AEnemyAI class


/**
*	=====================================================================
*		List of Non UPROPERTY values, must be deleted
*	=====================================================================
*		- FEnemyBlackboard* CharacterBlackboard;
*		- PrimitiveTask* CurrentTask;	(NO DELETE - Static class, not owned by this class)
*		- NPCTask* GoalTask;			(NO DELETE - Static class, not owned by this class)
* 		- TLinkedList<PrimitiveTask*>* TaskList;
*/

/**
 * Base class for an enemy controller which uses the HTN implementation
 */
UCLASS()
class TMO_API AHTNController : public AAIController
{
	GENERATED_BODY()

	/** Grant Access to private variables to the FPlanningThread class */
	friend class FPlanningThread;
	
public:

	/** PCIP Constructor */
	AHTNController(const FObjectInitializer& ObjectInitializer);

	static class UTaskHub* TaskHub;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Blackboard

protected:

	/**
	*	Pointer to the blackboard class containing the data to be used for planning the NPC's moves,
	*	not a UPROPERTY as an exposed struct pointer is not allowed to be a UPROPERTY, therefore the lifetime
	*	of this variable must be manually controlled
	*/
	FEnemyBlackboard* CharacterBlackboard;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Setup Functions

public:

	/** Function called when the AEnemyAI class takes control of a pawn
	*	@param InPawn - The pawn to be taken control of, is then cast to a
	*					AEnemyCharacter object and set to ControlledCharacter */
	virtual void Possess(APawn* InPawn) override;
	

protected:

	/** Function called once the AEnemyAI instance is set to be destroyed */
	virtual void Destroyed() override;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Tasks and Replanning Variables

protected:

	/** Action currently being executed by the AEnemyAI class */
	UPROPERTY()
	UPrimitiveTask* CurrentTask;

	/** Currently set goal for the AEnemyAI class */
	UPROPERTY()
	UEnemyTask* GoalTask;

	/** TLinkedList containing the current plan to be executed */
	//UPROPERTY()
	TLinkedList<UPrimitiveTask*>* TaskList;


public:

	/**
	*	Boolean value indicating whether NextTask should be called when the
	*	current movement is finished
	*/
	UPROPERTY()
	bool bFinishTaskOnMoveCompleted = false;

	/**
	*	Boolena value indicating whether future replans should be blocked, used
	*	if there is a protected operation such as a melee attack which requires that
	*	no further melee operation be set as the task until 
	*/
	UPROPERTY()
	bool bPendingReplan = false;
	
	/** Bool indicating whether the character is currently in the replanning queue */
	UPROPERTY()
	bool bIsCurrentlyReplanning = false;

	/** Bool value indicating whether the character is currently being destroyed */
	UPROPERTY()
	bool bIsBeingDestroyed = false;

	/** Function called by character destruction to clear up NPC */
	void PrepareAIForDestruction();

	/** Timer used by the CurrentTask, must be accessible by an NPC task*/
	UPROPERTY()
	FTimerHandle AITimer;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Tasks and Replanning Functions

private:

	/**
	*	Function which actually determines what the next plan is.
	*	Note: This function is used exclusively by the planning thread, and
	*	should not be called directly, use RegisterReplan instead
	*	@return - The new plan for the AEnemyAI class
	*/
	TLinkedList<UPrimitiveTask*>* DetermineNewPlan();

    /**
    *   Experimental version of determine new plan, which also creates a stack of locations
    *   that the currently running plan can access
    */
	TLinkedList<UPrimitiveTask*>* DetermineNewPlan(TArray<FVector>& NewLocations);

    /**
     *   Experimental version of apply new plan, which also creates a stack of locations
     *   that the currently running plan can access
     */
	void ApplyNewPlan(TLinkedList<UPrimitiveTask*>* NewPlan, TArray<FVector>& NewLocations);

	/**
	*	Function called to apply a new plan to the AEnemyAI class
	*	@return - The new plan for the AEnemyAI class
	*/
	void ApplyNewPlan(TLinkedList<UPrimitiveTask*>* NewPlan);

	/** Function called to empty the task list */
	void EmptyTaskList();
	
public:

	/** Function called to execute the next action in the action list */
	void NextTask();

	/** Function called to indicate that a Replan should be called*/
	void RegisterReplan();

	/**
	*	Function called to force replan, this terminates all tasks including
	*	tasks which would block replanning
	*/
	void ForceReplan();

	/** Function called to set the current task, without causing a replan 
		@param TaskName - Name of task to be set as the current task
		@param bShouldExecute - Whether the task should be executed*/
	void SetCurrentTask(FString& TaskName, bool bShouldExecute);

	/** Function called to set the current goal task for the AEnemyAI instance */
	void SetGoalTask(UEnemyTask* NewGoalTask);

	/** @param NewGoalTaskName - String name of new goal task to be set on the character */
	void SetGoalTask(FString& NewGoalTaskName);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Getters

public:

	/** @return - Pointer to the character being controlled by this AI */
	FORCEINLINE AEnemyCharacter* GetControlledCharacter() const { return Cast<AEnemyCharacter>(GetPawn()); };

	/** @return - The blackboard owned by this AI */
	FORCEINLINE FEnemyBlackboard* GetBlackboard() const { return CharacterBlackboard; };

	/** @return - The planning blackboard owned by this AI */
	// FORCEINLINE UEnemyBlackboard* GetPlanningBlackboard() const { return PlanningBlackboard; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
