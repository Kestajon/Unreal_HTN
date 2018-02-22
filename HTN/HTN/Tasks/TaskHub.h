// Author:	Jonathan Keslake

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "EnemyTask.h"

#include "TaskHub.generated.h"



/** Encapsulates the collection of tasks which can be used by an AI within the system
	and facilitates the planning of these tasks*/
UCLASS(Blueprintable, BlueprintType)
class TMO_API UTaskHub : public UObject
{
	GENERATED_BODY()

protected:

	/** Tasks declared in blueprints */
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UEnemyTask>> BlueprintTasks;

	/** Map to allow tasks to be referenced by their name */
	UPROPERTY()
	TMap<FString, UEnemyTask*> TaskMap;

public:

	/** Function called after construction by SquadHub */
	void InitialSetup();

	/** @param newTask - New task to be added to the map */
	void RegisterNewTask(UEnemyTask* newTask);

	/** @param InputName - Name of desired task
		@return - Task corresponding to the name in InputName */
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UEnemyTask* GetTaskByName(FString& InputName)
	{
		if (TaskMap.Contains(InputName))
		{
			return TaskMap[InputName];
		}

		return nullptr;
	}
};