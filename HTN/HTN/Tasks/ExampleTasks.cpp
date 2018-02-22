// Author:	Jonathan Keslake

#include "AttackEnemy.h"

#include "Enemies/EnemyAI.h"
#include "Enemies/EnemyCharacter.h"

#include "Enemies/Squad/SquadHub.h"

// #include "Runtime/Engine/Classes/Engine/Engine.h" 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AttackEnemy Class

/**	Function called during the planning stage to determine what tasks
*	are required to complete this task
*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
TLinkedList<UPrimitiveTask*>* UAttackEnemy::Decompose(FEnemyBlackboard* PlanningBlackboard)
{
	FEnemyBlackboard* tempBB = new FEnemyBlackboard(*PlanningBlackboard);
	TLinkedList<UPrimitiveTask*>* LL1;
	TLinkedList<UPrimitiveTask*>* LL2;

	FString NameOfTask;

	if (tempBB->NPCAttackState == ENPCAttackState::MeleeAttacking)
	{

		// This is called if the character is already currently attacking
		if (!tempBB->bCanMelee)
		{
			// Create next plan as if the NPCAttackState is None
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The NPC can melee therefore perform melee attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = MeleeAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("MeleeAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	else if (tempBB->NPCAttackState == ENPCAttackState::ActivatingSpecial)
	{
		// This is called if the character is already performing a special attack
		if (!tempBB->bCanPerformSpecial)
		{
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The Character can now perform the special attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = SpecialAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("SpecialAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else
	{
		// The Character should move to the character and then wait for further instructions
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = DummyTask::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	return new TLinkedList<UPrimitiveTask*>(); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  AuxiliaBackground Class

/**	Function called during the planning stage to determine what tasks
*	are required to complete this task
*	@param PlanningBlackboard - Stack to place new Tasks on for processing */
TLinkedList<UPrimitiveTask*>* UAuxiliaBackground::Decompose(FEnemyBlackboard* PlanningBlackboard)
{
	FEnemyBlackboard* tempBB = new FEnemyBlackboard(*PlanningBlackboard);
	TLinkedList<UPrimitiveTask*>* LL1;
	TLinkedList<UPrimitiveTask*>* LL2;

	FString NameOfTask;

	// Auxilia will always attempt to join the active melee group if they can melee
	if (AEnemyCharacter::SquadHub->WSCurrentMeleeGroupAccepting)
	{
		// The Character should move to the character and then wait for further instructions
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	// If the melee group is not currently accepting new members
	// The auxilia should back away if in the close melee area
	if (tempBB->DistanceToTarget == EDistanceToTarget::AttackRange)
	{
		// Navigate to EDistanceToTarget::BackupRange through moving away from the player
		// then wait for further instructions (Apply dummy task)
		// The Character should move to the character and then wait for further instructions
		NameOfTask = FString::Printf(TEXT("NavigateAwayFromCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else
	{
		// Navigate to EDistanceToTarget::BackupRange through moving away from the player
		// then wait for further instructions (Apply dummy task)
		// The Character should move to the character and then wait for further instructions
		NameOfTask = FString::Printf(TEXT("NavigateToBackgroundDistance"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	return new TLinkedList<UPrimitiveTask*>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VelitesMeleeGroup class

/**	Function called during the planning stage to determine what tasks
*	are required to complete this task
*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
TLinkedList<UPrimitiveTask*>* UVelitesMeleeGroup::Decompose(FEnemyBlackboard* PlanningBlackboard)
{
	FEnemyBlackboard* tempBB = new FEnemyBlackboard(*PlanningBlackboard);
	TLinkedList<UPrimitiveTask*>* LL1;
	TLinkedList<UPrimitiveTask*>* LL2;

	FString NameOfTask;

	if (tempBB->NPCAttackState == ENPCAttackState::MeleeAttacking)
	{
		// This is called if the character is already currently attacking
		if (!tempBB->bCanMelee)
		{
			// Create next plan as if the NPCAttackState is None
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The NPC can melee therefore perform melee attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = MeleeAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("MeleeAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	else if (tempBB->NPCAttackState == ENPCAttackState::ActivatingSpecial)
	{
		// This is called if the character is already performing a special attack
		if (!tempBB->bCanPerformSpecial)
		{
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The Character can now perform the special attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = SpecialAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("SpecialAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else if (tempBB->NPCAttackState == ENPCAttackState::RangeAttacking)
	{
		// This is called if the character is already performing a special attack
		if (!tempBB->bCanRange)
		{
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The Character can now perform the special attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = SpecialAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("RangeAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else
	{
		// The Character should move to the character and then wait for further instructions
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = DummyTask::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	return new TLinkedList<UPrimitiveTask*>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VelitesBackgroundGroup Class

/**	Function called during the planning stage to determine what tasks
*	are required to complete this task
*	@param PlanningBlackboard - Stack to place new Tasks on for processing 	*/
TLinkedList<UPrimitiveTask*>* UVelitesBackgroundGroup::Decompose(FEnemyBlackboard* PlanningBlackboard)
{
	FEnemyBlackboard* tempBB = new FEnemyBlackboard(*PlanningBlackboard);
	TLinkedList<UPrimitiveTask*>* LL1;
	TLinkedList<UPrimitiveTask*>* LL2;

	FString NameOfTask;

	// If the velites can fire it should
	if (tempBB->NPCAttackState == ENPCAttackState::RangeAttacking)
	{
		// This is called if the character is already performing a special attack
		if (!tempBB->bCanRange)
		{
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The Character can now perform the special attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToBackgroundDistance"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = SpecialAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("RangeAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else if (tempBB->NPCAttackState == ENPCAttackState::ActivatingSpecial)
	{
		// This is called if the character is already performing a special attack
		if (!tempBB->bCanPerformSpecial)
		{
			tempBB->NPCAttackState = ENPCAttackState::None;
			LL1 = Decompose(tempBB);
			if (**LL1)
			{
				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
		}

		// The Character can now perform the special attack
		// LL1 = NavigateToCharacter::Instance()->Decompose(tempBB);
		NameOfTask = FString::Printf(TEXT("NavigateToBackgroundDistance"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			// LL2 = SpecialAttackCharacter::Instance()->Decompose(tempBB);
			NameOfTask = FString::Printf(TEXT("SpecialAttackCharacter"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);
			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}

	// If the melee group is not currently accepting new members
	// The auxilia should back away if in the close melee area
	if (tempBB->DistanceToTarget == EDistanceToTarget::AttackRange)
	{
		// Navigate to EDistanceToTarget::BackupRange through moving away from the player
		// then wait for further instructions (Apply dummy task)
		// The Character should move to the character and then wait for further instructions
		NameOfTask = FString::Printf(TEXT("NavigateAwayFromCharacter"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	else
	{
		// Navigate to EDistanceToTarget::BackupRange through moving away from the player
		// then wait for further instructions (Apply dummy task)
		// The Character should move to the character and then wait for further instructions
		NameOfTask = FString::Printf(TEXT("NavigateToBackgroundDistance"));
		LL1 = GetTaskByName(NameOfTask)->Decompose(tempBB);
		if (**LL1)
		{
			NameOfTask = FString::Printf(TEXT("DummyTask"));
			LL2 = GetTaskByName(NameOfTask)->Decompose(tempBB);

			if (**LL2)
			{
				LL1->LinkBefore(LL2);

				// Update the blackboard of the decomposition which called this
				*PlanningBlackboard = *tempBB;

				delete tempBB;
				return LL1;
			}
			delete LL2;
		}
		delete LL1;
	}
	// Otherwise, Navigate to EDistanceToTarget::BackupRange through moving towards the player
	// Then wait for further instructions (Apply dummy task)

	return new TLinkedList<UPrimitiveTask*>();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateAwayFromCharacter Class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*		immediately called */
bool UNavigateAwayFromCharacter::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
	EnemyController->bFinishTaskOnMoveCompleted = true;
	EnemyController->MoveAwayFromTarget();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateToCharacter class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*		immediately called */
bool UNavigateToCharacter::Execute(AHTNController* Controller)
{

    EnemyController = Cast<AEnemyAI>(Controller);
    EnemyController->bFinishTaskOnMoveCompleted = true;
    EnemyController->MoveToTarget();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  NavigateToBackgroundDistance class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called */
bool UNavigateToBackgroundDistance::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
	EnemyController->bFinishTaskOnMoveCompleted = true;
	EnemyController->MoveToBackgroundDistance();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  MeleeAttackCharacter class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called */
bool UMeleeAttackCharacter::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
	EnemyController->GetBlackboard()->bCanMelee = false;
	EnemyController->MeleeAttackTarget();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  SpecialAttackCharacter class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called */
bool USpecialAttackCharacter::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
    EnemyController->GetBlackboard()->bCanPerformSpecial = false;
	EnemyController->ActivateSpecial();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  RangeAttackCharacter class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called */
bool URangeAttackCharacter::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
	EnemyController->GetBlackboard()->bCanRange = false;
	EnemyController->GetBlackboard()->bCanMelee = false;
	EnemyController->RangeAttackTarget();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  KnockbackCharacter class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called */
bool UKnockbackCharacter::Execute(AHTNController* Controller)
{
    EnemyController = Cast<AEnemyAI>(Controller);
    EnemyController->ApplyKnockback();
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  DummyTask class

/**	Function called by AHTNController when the primitive task is activated
*	@param AHTNController - Controller to be manipulated by the task
*	@return - Boolean value indicating whether the NextTask should be
*				immediately called	*/
bool UDummyTask::Execute(AHTNController* Controller)
{
	return false;
}
