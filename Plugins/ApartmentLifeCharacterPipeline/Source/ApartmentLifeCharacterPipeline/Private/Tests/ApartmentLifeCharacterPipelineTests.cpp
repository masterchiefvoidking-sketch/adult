// Copyright Adult Anime Apartment Life. All Rights Reserved.

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "ApartmentLifeCharacterPipelineLibrary.h"
#include "ApartmentLifeCharacterCreatorTypes.h"
#include "ApartmentLifeCharacterCreatorCatalogLibrary.h"
#include "ApartmentLifeCharacterCreatorPresetLibrary.h"
#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeCharacterCreatorLibrary.h"
#include "ApartmentLifeAnimationLibrary.h"
#include "ApartmentLifeAnimationComponent.h"
#include "ApartmentLifeAnimationData.h"
#include "ApartmentLifeYogaMinigameComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeActivityAnimationMapTest,
	"ApartmentLife.CharacterPipeline.ActivityAnimationMap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeActivityAnimationMapTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Yoga maps correctly"),
		UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(FName(TEXT("activity.yoga.morning"))),
		EApartmentLifeAnimationGroup::Yoga);
	TestEqual(TEXT("Cooking maps correctly"),
		UApartmentLifeCharacterPipelineLibrary::GetAnimationGroupForActivity(FName(TEXT("activity.cook.dinner"))),
		EApartmentLifeAnimationGroup::Cooking);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeYogaPoseMatchTest,
	"ApartmentLife.CharacterPipeline.YogaPoseMatch",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeYogaPoseMatchTest::RunTest(const FString& Parameters)
{
	FApartmentLifeYogaSessionState State;
	State.bOnYogaMat = true;
	State.PoseMatchScore = 0.f;

	UApartmentLifeYogaPoseData* Pose = NewObject<UApartmentLifeYogaPoseData>();
	Pose->Difficulty = 0.6f;
	Pose->FlexibilityReward = 5.f;

	const FApartmentLifeYogaSessionState Updated =
		UApartmentLifeCharacterPipelineLibrary::UpdateYogaPoseMatch(State, Pose, 0.9f, 1.f);

	TestTrue(TEXT("Pose match improves with good input"), Updated.PoseMatchScore > State.PoseMatchScore);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeGroomingAdvanceTest,
	"ApartmentLife.CharacterPipeline.GroomingAdvance",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeGroomingAdvanceTest::RunTest(const FString& Parameters)
{
	FApartmentLifeGroomingRoutineState State;
	State.CurrentStep = EApartmentLifeGroomingStep::Shower;
	State.Hygiene = 50.f;

	UApartmentLifeGroomingRoutineData* Routine = NewObject<UApartmentLifeGroomingRoutineData>();
	Routine->Steps = { EApartmentLifeGroomingStep::Shower, EApartmentLifeGroomingStep::BrushTeeth };
	Routine->HygieneGainPerStep = 10.f;

	const FApartmentLifeGroomingRoutineState Updated =
		UApartmentLifeCharacterPipelineLibrary::AdvanceGroomingStep(State, Routine);

	TestEqual(TEXT("Advances to brush teeth"), Updated.CurrentStep, EApartmentLifeGroomingStep::BrushTeeth);
	TestEqual(TEXT("Hygiene increases"), Updated.Hygiene, 60.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeBodyFitProfileTest,
	"ApartmentLife.CharacterPipeline.BodyFitProfile",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeBodyFitProfileTest::RunTest(const FString& Parameters)
{
	FApartmentLifeBodyCustomization Body;
	Body.HeightCm = 180.f;
	Body.ShoulderWidth = 0.8f;

	const FVector Scale = Body.GetScaleVector();
	TestTrue(TEXT("Taller body scales up"), Scale.Z > 1.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeYogaSaveRoundTripTest,
	"ApartmentLife.CharacterPipeline.YogaSaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeYogaSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UApartmentLifeYogaMinigameComponent* Yoga = NewObject<UApartmentLifeYogaMinigameComponent>();
	Yoga->StartYogaSession(FName(TEXT("pose.builtin.stretch")), true);

	TMap<FString, FString> Saved;
	Yoga->CaptureSaveData(Saved);

	UApartmentLifeYogaMinigameComponent* Restored = NewObject<UApartmentLifeYogaMinigameComponent>();
	Restored->RestoreSaveData(Saved);

	TestEqual(TEXT("Pose restored"), Restored->GetSessionState().CurrentPoseId, FName(TEXT("pose.builtin.stretch")));
	TestTrue(TEXT("On mat restored"), Restored->GetSessionState().bOnYogaMat);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCreatorPresetApplyTest,
	"ApartmentLife.CharacterPipeline.CreatorPresetApply",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCreatorPresetApplyTest::RunTest(const FString& Parameters)
{
	const TArray<FApartmentLifeCharacterCreatorPreset> BuiltinPresets =
		UApartmentLifeCharacterCreatorCatalogLibrary::GetBuiltinPresets();
	TestTrue(TEXT("Builtin presets available"), BuiltinPresets.Num() >= 3);

	FApartmentLifeCharacterCreatorState State = UApartmentLifeCharacterCreatorCatalogLibrary::GetDefaultCreatorState();
	State.Body.HeightCm = 172.f;
	State.HairstyleId = FName(TEXT("hair.bob.soft"));

	FApartmentLifeCharacterCreatorPreset Preset;
	Preset.PresetId = FName(TEXT("test.preset.full"));
	Preset.PresetType = EApartmentLifeCreatorPresetType::FullCharacter;
	Preset.CreatorState = State;

	FApartmentLifeCharacterCreatorState Applied = UApartmentLifeCharacterCreatorCatalogLibrary::GetDefaultCreatorState();
	TestTrue(TEXT("Apply full preset"),
		UApartmentLifeCharacterCreatorPresetLibrary::ApplyPresetToState(Preset, Applied));
	TestEqual(TEXT("Height copied"), Applied.Body.HeightCm, 172.f);
	TestEqual(TEXT("Hairstyle copied"), Applied.HairstyleId, FName(TEXT("hair.bob.soft")));

	FApartmentLifeCharacterCreatorPreset BodyPreset;
	BodyPreset.PresetType = EApartmentLifeCreatorPresetType::Body;
	BodyPreset.CreatorState.Body.MuscleTone = 0.8f;
	FApartmentLifeCharacterCreatorState BodyOnly = State;
	TestTrue(TEXT("Apply body preset"),
		UApartmentLifeCharacterCreatorPresetLibrary::ApplyPresetToState(BodyPreset, BodyOnly, EApartmentLifeCreatorPresetType::Body));
	TestEqual(TEXT("Body morph applied"), BodyOnly.Body.MuscleTone, 0.8f);
	TestEqual(TEXT("Face preserved on body preset"), BodyOnly.HairstyleId, FName(TEXT("hair.bob.soft")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCreatorRandomizeBoundsTest,
	"ApartmentLife.CharacterPipeline.CreatorRandomizeBounds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCreatorRandomizeBoundsTest::RunTest(const FString& Parameters)
{
	UApartmentLifeCharacterCreatorComponent* Creator = NewObject<UApartmentLifeCharacterCreatorComponent>();
	Creator->InitializeFromBuiltinCatalog();
	Creator->RandomizeFace();

	const FApartmentLifeCharacterCreatorState State = Creator->GetCreatorState();
	for (const FApartmentLifeMorphSlider& Morph : State.Face.GetAllFaceMorphs())
	{
		TestTrue(TEXT("Face morph in range"), Morph.Value >= 0.f && Morph.Value <= 1.f);
	}

	Creator->RandomizeBody();
	TestTrue(TEXT("Height in range"),
		Creator->GetCreatorState().Body.HeightCm >= 140.f && Creator->GetCreatorState().Body.HeightCm <= 200.f);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCreatorSaveRoundTripTest,
	"ApartmentLife.CharacterPipeline.CreatorSaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCreatorSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UApartmentLifeCharacterCreatorComponent* Creator = NewObject<UApartmentLifeCharacterCreatorComponent>();
	Creator->InitializeFromBuiltinCatalog();
	Creator->SetBodySlider(FName(TEXT("HeightCm")), 171.f);
	Creator->SetHairstyle(FName(TEXT("hair.pixie.soft")));

	TMap<FString, FString> Saved;
	Creator->CaptureSaveData(Saved);

	UApartmentLifeCharacterCreatorComponent* Restored = NewObject<UApartmentLifeCharacterCreatorComponent>();
	Restored->RestoreSaveData(Saved);

	const FApartmentLifeCharacterCreatorState& State = Restored->GetCreatorState();
	TestEqual(TEXT("Height restored"), State.Body.HeightCm, 171.f);
	TestEqual(TEXT("Hairstyle restored"), State.HairstyleId, FName(TEXT("hair.pixie.soft")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeCreatorPreviewGroupsTest,
	"ApartmentLife.CharacterPipeline.CreatorPreviewGroups",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeCreatorPreviewGroupsTest::RunTest(const FString& Parameters)
{
	const TArray<FName> Groups = UApartmentLifeCharacterCreatorLibrary::GetCreatorPreviewAnimationGroups();
	TestTrue(TEXT("Preview groups populated"), Groups.Num() >= 6);
	TestTrue(TEXT("Idle preview available"), Groups.Contains(FName(TEXT("Idle"))));
	TestTrue(TEXT("Walk preview available"), Groups.Contains(FName(TEXT("Walking"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeAnimationTransitionTest,
	"ApartmentLife.CharacterPipeline.AnimationTransition",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeAnimationTransitionTest::RunTest(const FString& Parameters)
{
	const EApartmentLifeAnimationTransitionKind SitTransition = UApartmentLifeAnimationLibrary::GetTransitionBetweenGroups(
		EApartmentLifeAnimationGroup::Walking,
		EApartmentLifeAnimationGroup::ChairSitting);
	TestEqual(TEXT("Walk to chair uses montage bridge"), SitTransition, EApartmentLifeAnimationTransitionKind::MontageBridge);

	const FName MontageId = UApartmentLifeAnimationLibrary::GetMontageIdForActivity(FName(TEXT("activity.cook.prepare")));
	TestEqual(TEXT("Activity montage naming"), MontageId, FName(TEXT("montage.cook.prepare")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeAnimationSaveRoundTripTest,
	"ApartmentLife.CharacterPipeline.AnimationSaveRoundTrip",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeAnimationSaveRoundTripTest::RunTest(const FString& Parameters)
{
	UApartmentLifeAnimationComponent* Animation = NewObject<UApartmentLifeAnimationComponent>();
	Animation->PlayActivityAnimation(
		FName(TEXT("activity.work.computer")),
		EApartmentLifeAnimationGroup::ComputerUse,
		FApartmentLifeInteractionAlignmentSet(),
		70.f,
		60.f);

	TMap<FString, FString> Saved;
	Animation->CaptureSaveData(Saved);

	UApartmentLifeAnimationComponent* Restored = NewObject<UApartmentLifeAnimationComponent>();
	Restored->RestoreSaveData(Saved);
	Restored->RestoreAnimationAfterLoad();

	TestEqual(TEXT("Group restored"), Restored->GetAnimationState().CurrentGroup, EApartmentLifeAnimationGroup::ComputerUse);
	TestEqual(TEXT("Activity restored"), Restored->GetAnimationState().ActiveActivityId, FName(TEXT("activity.work.computer")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FApartmentLifeFacialExpressionTest,
	"ApartmentLife.CharacterPipeline.FacialExpression",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FApartmentLifeFacialExpressionTest::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("Sleep activity sleepy face"),
		UApartmentLifeAnimationLibrary::GetFacialExpressionForActivity(FName(TEXT("activity.sleep.bed")), 60.f, 60.f),
		EApartmentLifeFacialExpression::Sleepy);
	TestEqual(TEXT("Low energy tired face"),
		UApartmentLifeAnimationLibrary::GetFacialExpressionForActivity(FName(TEXT("activity.relax.sofa")), 60.f, 10.f),
		EApartmentLifeFacialExpression::Tired);
	return true;
}

#endif
