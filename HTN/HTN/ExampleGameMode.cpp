
#include "ExampleGameMode.h"
#include "HTNController.h"
#include "Tasks/TaskHub.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ATMOGameMode class

/** Default constructor */
AExampleGameMode::AExampleGameMode()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Update Functions

/** Function called after the constructor but before play begins */
void AExampleGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    if (TaskHubClass)
    {
        UTaskHub* TaskHub = NewObject<UTaskHub>(this, TaskHubClass.Get());
    }
}
