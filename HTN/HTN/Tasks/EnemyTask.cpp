// Author:	Jonathan Keslake

#include "EnemyTask.h"

#include "Runtime/Engine/Classes/Engine/Engine.h" 

#include "HTN/HTNController.h"

#include "HTN/Tasks/TaskHub.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UEnemyTask

/** Convenience function to obtain a task by its' name */
UEnemyTask* UEnemyTask::GetTaskByName(FString& NameOfTask)
{
	return AEnemyAI::TaskHub->GetTaskByName(NameOfTask);
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
