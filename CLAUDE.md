# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Single-player first-person horror game (SCP-1983 setting) built with **Unreal Engine 5.4**, C++ + Blueprint, Windows only. Only stage 1 is implemented: escape the house by unlocking a specific door on the 1st floor while a ghost AI patrols. The UE project lives in `UE5_HorrorGame/`; the git root is one level above it. There is one game module (`UE5_HorrorGame`) plus the third-party `BlockoutToolsPlugin` (marketplace plugin, don't modify).

## Build

No tests, lint, or CI exist. Build through UnrealBuildTool (engine install path varies per machine):

```
"<UE_5.4>/Engine/Build/BatchFiles/Build.bat" UE5_HorrorGameEditor Win64 Development -Project="<repo>/UE5_HorrorGame/UE5_HorrorGame.uproject" -WaitMutex
```

- Regenerate the VS solution (`.sln` is gitignored): right-click the `.uproject` → *Generate Visual Studio project files*.
- Header/UPROPERTY/UFUNCTION changes need a full editor-closed build; Live Coding only handles `.cpp` body changes.
- Startup/default map: `/Game/Map/LevelDesign/L_Main_Menu`. Default game mode is the Blueprint `/Game/Game/BP_GameMode` (subclass of `AHRGameMode`).

## Repository caveats

- Large content folders are **gitignored** (`Content/Asset`, `Content/Audio`, `Content/Animation`, `Content/Image`), so a fresh clone cannot open maps fully. Many `.uasset` files referenced by C++ may not exist locally.
- `.uasset`/`.umap` are binary — Blueprint logic (behavior tree contents, widget layout, door/drawer behavior, BP_Interact implementations) cannot be read or edited from here. Much gameplay lives in BP; C++ provides the base classes and hooks.
- **Mixed source encodings**: most files are ASCII/UTF-8, but several (e.g. `HRCharacterNonPlayer.cpp`, `HRInteractableActorInterface.h`, `HRToggleItemInterface.h`) contain Korean comments in **CP949/EUC-KR**. Preserve the existing encoding when editing those files, or the comments become mojibake.
- C++ hard-codes asset paths via `ConstructorHelpers` (e.g. `/Game/GameData/HRCharacterStatTable`, `/Game/Blueprint/Ghost/BB_/BT_GhostCharacter_Perception`, `/Game/Blueprint/Player/BP_CharacterPlayer`, `/Game/UI/WBP_Interaction`). Renaming/moving those assets in the editor breaks the C++ silently.

## Architecture

All classes use the `HR` prefix. `PublicIncludePaths` includes the module root, so includes are written as `"Folder/File.h"`.

**Game flow** — `AHRMainMenuGameMode` + `AHRMainMenuPlayerController` + `HRMainMenuHUD` for the title screen; `AHRGameMode` sets `BP_CharacterPlayer` as pawn and `AHRPlayerController`. The controller owns pause menu (`IMC_System` mapping context, `TogglePauseGameAction`) and game over: it binds to the player's `OnPlayerDied` delegate on possess, shows the game-over widget, and restarts the level after `RestartDelay`. `UHRUIManagerComponent` centralizes switching between UI and game input modes.

**Stats** — `UHRGameSingleton` (registered via `GameSingletonClassName` in `Config/DefaultEngine.ini`) loads the `FHRCharacterStat` DataTable (source CSV/XLSX in `UE5_HorrorGame/GameData/`) and serves rows by level. `UHRCharacterStatComponent` pulls from it. `FHRCharacterStat::operator+` reinterprets the struct as a float array — **every field must stay `float`**.

**Player (`AHRCharacterPlayer`)** — Enhanced Input. Three camera rigs (First / Shoulder / Quarter) each with its own `UHRCharacterControlData` data asset (IMC + movement settings) in `CharacterControlManager`; `ChangeCharacterControl` swaps them. Also handles timeline-driven crouch, sprint with timer-based stamina drain/regen, HP effect and stamina widgets, inventory (`UInventoryComponent`), key chain (`EKeyType` set), flashlight attachment, floor maps (`EMapType`), and an `UAIPerceptionStimuliSourceComponent` so the ghost can perceive it.

**Interaction system** (spans player + actor):
1. `AHRInteractableActorBase`'s `InteractionBox` overlap calls `Player->RegisterInteractableActor/UnregisterInteractableActor`.
2. While any actor is registered, the player runs a timer-driven `TraceInteractable()` line trace from the view point on `ECC_GameTraceChannel1` (`ECC_Interactable`) against each actor's `TraceHitBox`, and calls `OnFocusGained/OnFocusLost` to show the screen-space prompt widget (`WBP_Interaction`, text block named `InteractionText_WBP`).
3. `Interact()` invokes `IHRInteractableActorInterface::BP_Interact` on the focused actor — the actual behavior is implemented in Blueprint subclasses.
- `AHRItemBase` extends the interactable base with `IHRItemInterface` (`OnPickedUp` → inventory, `BP_OnPickedUp` hook). `IHRToggleItemInterface::BP_ToggleItem` is for toggleable items like the flashlight.
- Collision profiles used here (`InteractableActorBase_Mesh/_Box/_TraceHitBox`) and custom channels are defined in `DefaultEngine.ini`.

**Ghost AI** — `AHRCharacterNonPlayer` implements `IHRCharacterAIinterface` (patrol radius, detect/attack range, turn speed, attack delegate) and `IHRAnimationInterface` (`DoAttackHitCheck`, triggered from an anim notify; sweep on `CCHANNEL_HRACTION` = `ECC_GameTraceChannel2`). `AHRAIController` owns an `UAIPerceptionComponent` (sight + hearing), and on possess runs the Blackboard/BehaviorTree, seeding `HomePos`. BT nodes in `AI/` (`BTTask_FindPatrolPos`, `BTTask_Attack`, `BTTask_TurnToTarget`, `BTService_Detect`, `BTDecorator_AttackInRange`) talk to the pawn only through `IHRCharacterAIinterface`. Blackboard key names are macros in `AI/HRAI.h` and must match the BB asset.
