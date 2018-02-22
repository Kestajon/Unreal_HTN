# Unreal_HTN
An HTN system designed for use in Unreal Engine 4

Last tested in version: 4.18.0

# Origin

This HTN system was developed based on a university project, and was then extended upon.

# Purpose

This system is intended to be used to control characters without using the built in behaviour tree system in UE4. It's main design goals were:
  - Have tasks which can be written in either C++ or Blueprints
  - Create an HTN system using a Linked List implementation instead of the traditional stacking of frames
  - Create an HTN system which can be multi-threaded
  
# Hierarchy

The system can be used by any character which selects the AHTNContoller class as its' Controller. The character can then call any of the HTN functions from within the character class. However this alone will not allow much to happen, as the blackboard will be empty. In order to create a blackboard for use within the system, the AHTNController class can be extended, and then a new subclass of FBlackboard can then be created in the Possess or BeginPlay function, and that blackboard will then be used by the character. In order to keep a single Controller subclass, it is recommended to call a function within the character class to setup the blackboard and return it to the controller so that the character can define what blackboard it needs, and combined with the ability to set the goal task from the character, the same controller can be used for any character.

The tasks are split into the base UTask type and the UPrimitiveTask type, both can be extended, to create compound or primitive tasks respectively in both blueprints and C++.

The final issue is getting the Controller to access the tasks. As the tasks can be defined in blueprints, this cannot be hardwired in, therefore this is handled in the TaskHub. All that is required is for the TaskHub to be initialized and then set as the static variable within AHTNController. So as not to burden the user with too many classes to subclass, this is not in the source code files, however this was achieved simply in the GameMode, which is shown in the ExampleGameMode class, then the Taskhub can be linked properly.

Blueprint classes need to be added to the array within the blueprint class derived from Taskhub which would then be associated with this gamemode

