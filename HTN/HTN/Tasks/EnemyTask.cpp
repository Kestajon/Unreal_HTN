// Author:	Jonathan Keslake

#include "EnemyTask.h"

#include "Runtime/Engine/Classes/Engine/Engine.h" 

#include "Enemies/EnemyCharacter.h"
#include "Enemies/EnemyAI.h"

#include "Enemies/Tasks/TaskHub.h"

#include "Enemies/Squad/SquadHub.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UEnemyTask

/** Convenience function to obtain a task by its' name */
UEnemyTask* UEnemyTask::GetTaskByName(FString& NameOfTask)
{
	return AEnemyAI::TaskHub->GetTaskByName(NameOfTask);
}

/** Function to obtain the USquadHub which contains the squad blackboard */
USquadHub* UEnemyTask::GetSquadHub()
{
	return AEnemyCharacter::SquadHub;
}

/** Function called to print the name of the task, used for debugging purposes */
void UEnemyTask::PrintName()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, ReturnName());
}

/** @return - String representation of the name of this task */
FString UEnemyTask::ReturnName()
{
	return "EnemyTask_Base";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UPrimitiveTask

/** @return - String representation of the name of this task */
FString UPrimitiveTask::ReturnName()
{
	return "PrimitiveTask_Base";
}