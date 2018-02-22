// Author:	Jonathan Keslake

#include "TaskHub.h"
#include "Enemies/EnemyAI.h"
#include "AttackEnemy.h"

/** Function called after construction by SquadHub */
void UTaskHub::InitialSetup()
{
	// Register Default Auxilia goal tasks
	RegisterNewTask(NewObject<UEnemyTask>(this, UAttackEnemy::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, UAuxiliaBackground::StaticClass()));

	// Register Default Velites goal tasks
	RegisterNewTask(NewObject<UEnemyTask>(this, UVelitesMeleeGroup::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, UVelitesBackgroundGroup::StaticClass()));

	RegisterNewTask(NewObject<UEnemyTask>(this, UNavigateAwayFromCharacter::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, UNavigateToCharacter::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, UNavigateToBackgroundDistance::StaticClass()));

	RegisterNewTask(NewObject<UEnemyTask>(this, UMeleeAttackCharacter::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, URangeAttackCharacter::StaticClass()));
	RegisterNewTask(NewObject<UEnemyTask>(this, USpecialAttackCharacter::StaticClass()));

	RegisterNewTask(NewObject<UEnemyTask>(this, UKnockbackCharacter::StaticClass()));

	RegisterNewTask(NewObject<UEnemyTask>(this, UDummyTask::StaticClass()));

	for (TSubclassOf<UEnemyTask> Task : BlueprintTasks)
	{
		RegisterNewTask(NewObject<UEnemyTask>(this, Task.Get()));
	}

	BlueprintTasks.Empty();

	AEnemyAI::TaskHub = this;
}

/** @param newTask - New task to be added to the map */
void UTaskHub::RegisterNewTask(UEnemyTask* newTask)
{
	TaskMap.Add(newTask->ReturnName(), newTask);
}
