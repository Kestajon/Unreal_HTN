///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AExampleGameMode class

/**
 *	Class representing the rules of the Game, contains the level and xp level of
 *	the characters and generates guns, swords and characters based on the current
 *	level of the character.
 */
UCLASS(minimalapi)
class AExampleGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
public:
    
    /** Default constructor */
    AExampleGameMode();

    /** Class to be set as the TaskHub for the HTN implementation */
    UPROPERTY(EditAnywhere)
    TSubclassOf<class UTaskHub> TaskHubClass;
    
    /** Function called after the constructor but before play begins */
    virtual void BeginPlay() override;

};
