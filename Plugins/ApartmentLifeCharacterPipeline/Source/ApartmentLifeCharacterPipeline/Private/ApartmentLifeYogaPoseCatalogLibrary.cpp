// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeYogaPoseCatalogLibrary.h"

namespace
{
	FApartmentLifeBuiltinYogaPose MakePose(FName Id, const TCHAR* Name, float Difficulty, float FlexReward, float StressReduction)
	{
		FApartmentLifeBuiltinYogaPose Pose;
		Pose.PoseId = Id;
		Pose.DisplayName = FText::FromString(Name);
		Pose.PoseMontageId = FName(*FString::Printf(TEXT("montage.yoga.%s"), *Id.ToString()));
		Pose.Difficulty = Difficulty;
		Pose.FlexibilityReward = FlexReward;
		Pose.StressReduction = StressReduction;
		return Pose;
	}
}

TArray<FApartmentLifeBuiltinYogaPose> UApartmentLifeYogaPoseCatalogLibrary::GetBuiltinPoses()
{
	return {
		MakePose(FName(TEXT("pose.builtin.stretch")), TEXT("Gentle Stretch"), 0.3f, 4.f, 6.f),
		MakePose(FName(TEXT("pose.warrior.two")), TEXT("Warrior II"), 0.55f, 6.f, 8.f),
		MakePose(FName(TEXT("pose.tree.balance")), TEXT("Tree Pose"), 0.5f, 5.f, 9.f),
		MakePose(FName(TEXT("pose.cobra.backbend")), TEXT("Cobra Pose"), 0.45f, 5.f, 7.f),
		MakePose(FName(TEXT("pose.child.rest")), TEXT("Child's Pose"), 0.2f, 3.f, 12.f),
		MakePose(FName(TEXT("pose.downward.dog")), TEXT("Downward Dog"), 0.5f, 6.f, 8.f),
		MakePose(FName(TEXT("pose.cat.cow")), TEXT("Cat-Cow Flow"), 0.35f, 4.f, 7.f),
		MakePose(FName(TEXT("pose.pigeon.hip")), TEXT("Pigeon Pose"), 0.65f, 7.f, 9.f)
	};
}

bool UApartmentLifeYogaPoseCatalogLibrary::TryGetBuiltinPose(FName PoseId, FApartmentLifeBuiltinYogaPose& OutPose)
{
	for (const FApartmentLifeBuiltinYogaPose& Pose : GetBuiltinPoses())
	{
		if (Pose.PoseId == PoseId)
		{
			OutPose = Pose;
			return true;
		}
	}
	return false;
}
