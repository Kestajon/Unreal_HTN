// Author:	Jonathan Keslake

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Tasks/EnemyTask.h"
#include "AttackEnemy.generated.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AttackEnemy Class

/**  Default Auxilia behaviour while in the active melee squad */
UCLASS()
class TMO_API UAttackEnemy : public UEnemyTask
{
	GENERATED_BODY()
	
public:

	/**	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
	virtual TLinkedList<UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)  override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "AttackEnemy"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "AttackEnemy"; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AuxiliaBackground Class

/** Default Auxilia behaviour while in the background group */
UCLASS()
class TMO_API UAuxiliaBackground : public UEnemyTask
{
	GENERATED_BODY()

public:

	/**	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing */
	virtual TLinkedList<UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)  override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "AuxiliaBackground"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "AuxiliaBackground"; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VelitesMeleeGroup class

/**  Default Auxilia behaviour while in the active melee squad */
UCLASS()
class TMO_API UVelitesMeleeGroup : public UEnemyTask
{
	GENERATED_BODY()

public:

	/**	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
	virtual TLinkedList<UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)  override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "VelitesMeleeGroup"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "VelitesMeleeGroup"; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VelitesBackgroundGroup Class

/**  Default Auxilia behaviour while in the active melee squad */
UCLASS()
class TMO_API UVelitesBackgroundGroup : public UEnemyTask
{
	GENERATED_BODY()

public:

	/**	Function called during the planning stage to determine what tasks
	*	are required to complete this task
	*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
	virtual TLinkedList<UPrimitiveTask*>* Decompose(FEnemyBlackboard* PlanningBlackboard)  override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "VelitesBackgroundGroup"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "VelitesBackgroundGroup"; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateAwayFromCharacter Class

/** Navigates NPC away from the character in a straight line */
UCLASS()
class TMO_API UNavigateAwayFromCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*		immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "NavigateAwayFromCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "NavigateAwayFromCharacter"; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateToCharacter class

/** Navigates towards character in a straight line*/
UCLASS()
class TMO_API UNavigateToCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "NavigateToCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "NavigateToCharacter"; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateToBackgroundDistance class

/** Navigates towards character in a straight line*/
UCLASS()
class TMO_API UNavigateToBackgroundDistance : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "NavigateToBackgroundDistance"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "NavigateToBackgroundDistance"; };
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  MeleeAttackCharacter class

/**
* MeleeAttackPlayer class contains logic making the NPC perform a melee
* attack at the character targeted by the NPC
*/
UCLASS()
class TMO_API UMeleeAttackCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to determine if the task blocks */
	virtual bool IsTaskBlocking() override { return true; };

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "MeleeAttackCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "MeleeAttackCharacter"; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SpecialAttackCharacter class

/**	SpecialAttackPlayer class contains logic making the NPC perform a special
* attack at the character targeted by the NPC */
UCLASS()
class TMO_API USpecialAttackCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to determine if the task blocks */
	virtual bool IsTaskBlocking() override { return true; };

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "SpecialAttackCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "SpecialAttackCharacter"; };
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  RangeAttackCharacter class

/**	SpecialAttackPlayer class contains logic making the NPC perform a special
* attack at the character targeted by the NPC */
UCLASS()
class TMO_API URangeAttackCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to determine if the task blocks */
	virtual bool IsTaskBlocking() override { return true; };

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "RangeAttackCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "RangeAttackCharacter"; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  KnockbackCharacter class

UCLASS()
class TMO_API UKnockbackCharacter : public UPrimitiveTask
{
	GENERATED_BODY()

public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called */
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to determine if the task blocks */
	virtual bool IsTaskBlocking() override { return true; };

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "KnockbackCharacter"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "KnockbackCharacter"; };

};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  DummyTask class

UCLASS()
class TMO_API UDummyTask : public UPrimitiveTask
{
	GENERATED_BODY()
public:

	/**	Function called by AHTNController when the primitive task is activated
	*	@param AHTNController - Controller to be manipulated by the task
	*	@return - Boolean value indicating whether the NextTask should be
	*				immediately called	*/
	virtual bool Execute(AHTNController* Controller) override;

	/** Function called to return the name of the class to the caller */
	virtual FString ReturnName() override { return "DummyTask"; };

	/** static convenience function to get the name in a static form */
	static FString Sta_ReturnName() { return "DummyTask"; };
};
