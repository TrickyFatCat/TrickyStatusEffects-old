# TrickyStatusEffects 

A plugin which helps to implement status effects in your prototype.

## Installation

The plugin can be used in both C++ and Blueprint projects.

### Blueprint projects

**At the moment, the package is compatible only with Unreal Engine 4.**

1. Download the latest [**package file**](https://github.com/TrickyFatCat/TrickyPickups/releases);
2. Unzip the package into Plugins folder of your project, e.g. `D:\UnrealProjects\MyProject\Plugins`;
3. Restart the project;

### C++ projects

1. Create the Plugins folder in the project directory;
2. Create the TrickyAnimationComponents folder in the Plugins folder;
3. Download the plugin source code into that folder;
4. Rebuild the project;

## Content

The plugin contains:

1. StatusEffect object;
2. StatusEffectsManagerComponent;
3. StatusEffectsLibrary;

### StatusEffect object

A simple object which encapsulates status effect logic.

Use this object to create your status effects.

#### Variables

By default it has one variable `StatusEffectData`, this structure contains:

1. `Instigator` - the actor which applied the status effect;
2. `TargetActor` - the target actor of the status effect;
3. `OwningManager` - status effects manager component which owns the status effect;
4. `EffectType` - determines status effect type;
   1. `Positive`
   2. `Negative`
   3. `Neutral`
5. `EffectUniqueness` - determines how many instances of the status effect can be created:
   1. `Normal` - no limits;
   2. `PerInstigator` - instigator can apply only one status effect of this class on a target;
   3. `PerTarget` - only one status effect of this class can be applied to the target;
6. `IsInfinite` - toggles if the status effect has infinite duration or not;
7. `Duration` - duration of the effect;
8. `DurationTimerHandle` - a duration timer handle of the status effect;
9. `ReActivationBehavior` - determines how the duration will be recalculated when the status effect was reapplied.
   1. `None` - no changes;
   2. `Custom` - by default do nothing, but can be overriden;
   3. `Reset` - reset the timer;
   4. `Add` - add duration to the remaining time;
10. `IsStackable` - toggles if the status effect can be stacked;
11. `MaxStacks` - maximum amount of stacks;
12. `InitialStacks` - initial amount of stacks;
13. `CurrentStacks` - current amount of stacks;

#### Functions

1. `GetInstigator` - returns instigator;
2. `GetTargetActor` - returns target actor;
3. `GetOwningManager` - returns owning manager;
4. `GetEffectType` - returns effect type;
5. `GetUniqueness` - returns status effect uniqueness;
6. `GetIsInfinie` - returns if the status effect is infinite or not;
7. `GetDuration` - returns duration if `IsInfinite` == false, else -1;
8. `GetRemainingTime` - returns remaining time if `IsInfinite` == false, else -1;
9. `GetElapsedTime` - returns elapsed time if `IsInfinite` = false, else -1;
10. `IsStackable` - returns if the status effect is stackable;
11. `GetMaxStacks` - returns maximum amount of stacks;
12. `GetCurrentStacks` - returns current amount of stacks;
13. `AddStacks` - increases the number of stacks;
14. `RemoveStacks` - decreases the number of stacks;
15. `HandleEffectActivation` - called when the status effect was activated. Override this function to implement logic;
16. `HandleEffectReactivation`  - called when the status effect was reactivated. Override this function to implement logic;
17. `HandleEffectDeactivation` - called when the status effect was deactivated. Override this function to implement logic;
18. `HandleStacksIncrease` - called when the number of stacks was increased. Override this function to implement logic;
19. `HandleStacksDecrease` - called when the number of stacks was decreased. Override this function to implement logic;

#### Delegates

1. `OnStatusEffectDeactivated` - called when the status effect was deactivated;
2. `OnStatusEffectReactivated` - called when the status effect was reactivated;
3. `OnStacksAdded` - called when current number of stacks was increased;
4. `OnStacksRemoved` - called when current number of stacks was decreased;

### StatusEffectsManagerComponent

An actor component which handles status effects applied to its owner.

#### Variables

1. `ActiveEffects` - an array of active effects;
2. `DebugEnabled` - toggles debug information on screen in editor;

#### Functions

1. `ApplyEffect` - applies a new status effect or reapplies already applied effect;
2. `RemoveAllEffects` - removes all status effects regardless of time and stacks;
3. `RemoveAllPositiveEffects` - removes all positive status effects regardless of time and stacks;
4. `RemoveAllNegativeEffects` - removes all negative status effects regardless of time and stacks;
5. `RemoveAllNeutralEffects` - removes all neutral status effects regardless of time and stacks;
6. `RemoveEffectOfClass` - removes the first found status effect of a given class;
7. `RemoveAllEffectsOfClass` - removes all status effects of a given class regardless of remaining time and stacks;
8. `RemoveEffectOfClassByInstigator` - removes the first found status effect of a given class of specific instigator;
9. `RemoveAllEffectsOfClassByInstigator` - removes all status effects of a given class of specific instigator regardless of remaining time and stacks;
10. `RemoveEffectByObject` - removes a specific instance of the status effect;
11. `GetAllActiveEffects` - returns all active status effects;
12. `GetAllPositiveEffects` - returns all active positive status effects;
13. `GetAllNegativeEffects` - returns all active negative status effects;
14. `GetAllNeutralEffects` - returns all active neutral status effects;
15. `HasEffectOfClass` - checks if the status effect of a given class is active;
16. `GetEffectOfClass` - returns the status effect instance of the given class;
17. `GetAllEffectsOfClass` - returns all status effect instances of the given class;
18. `HesEffectOfClassByInstigator` - checks if the status effect of a given class and instigator is active;
19. `GetEffectOfClassByInstigator` - returns the status effect instance of the given class and instigator;
20. `GetAllEffectsOfClassByInstigator` - returns all status effect instances of the given class and instigator;

#### Delegates

1. `OnStatusEffectApplied` - called when a new status effect applied or old effect reapplied;

### StatusEffectsLibrary

#### Functions
