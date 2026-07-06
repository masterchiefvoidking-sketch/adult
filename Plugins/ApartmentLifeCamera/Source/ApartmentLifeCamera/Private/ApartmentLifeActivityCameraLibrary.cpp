// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeActivityCameraLibrary.h"

FApartmentLifeActivityCameraFrame UApartmentLifeActivityCameraLibrary::GetFrameForActivity(FName ActivityId)
{
	FApartmentLifeActivityCameraFrame Frame;
	const FString Id = ActivityId.ToString().ToLower();

	if (Id.Contains(TEXT("yoga")) || Id.Contains(TEXT("stretch")) || Id.Contains(TEXT("fitness")))
	{
		Frame.ArmLength = 420.f;
		Frame.Pitch = -18.f;
		Frame.FocusOffset = FVector(0.f, 0.f, 20.f);
		Frame.bAllowManualOrbit = true;
	}
	else if (Id.Contains(TEXT("work.computer")) || Id.Contains(TEXT("study")) || Id.Contains(TEXT("freelance")) || Id.Contains(TEXT("programming")))
	{
		Frame.ArmLength = 340.f;
		Frame.Pitch = -12.f;
		Frame.FocusOffset = FVector(0.f, 40.f, 50.f);
	}
	else if (Id.Contains(TEXT("groom")) || Id.Contains(TEXT("mirror")))
	{
		Frame.ArmLength = 260.f;
		Frame.Pitch = -8.f;
		Frame.FocusOffset = FVector(30.f, 0.f, 60.f);
	}
	else if (Id.Contains(TEXT("shower")) || Id.Contains(TEXT("hygiene")))
	{
		Frame.ArmLength = 300.f;
		Frame.Pitch = -10.f;
		Frame.FocusOffset = FVector(0.f, 60.f, 70.f);
		Frame.bPrivacyFraming = true;
	}
	else if (Id.Contains(TEXT("sleep")) || Id.Contains(TEXT("relax")) || Id.Contains(TEXT("read")))
	{
		Frame.ArmLength = 400.f;
		Frame.Pitch = -22.f;
		Frame.FocusOffset = FVector(-20.f, 0.f, 30.f);
	}
	else if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("wardrobe")))
	{
		Frame.ArmLength = 220.f;
		Frame.Pitch = -10.f;
		Frame.FocusOffset = FVector(0.f, 0.f, 30.f);
	}
	else if (Id.Contains(TEXT("cook")) || Id.Contains(TEXT("eat")))
	{
		Frame.ArmLength = 320.f;
		Frame.Pitch = -15.f;
		Frame.FocusOffset = FVector(0.f, 20.f, 40.f);
	}
	else if (Id.Contains(TEXT("breathe")))
	{
		Frame.ArmLength = 400.f;
		Frame.Pitch = -16.f;
		Frame.FocusOffset = FVector(0.f, 0.f, 15.f);
		Frame.bAllowManualOrbit = true;
	}

	return Frame;
}

EApartmentLifePrimaryCameraMode UApartmentLifeActivityCameraLibrary::GetPrimaryModeForActivity(FName ActivityId)
{
	const FString Id = ActivityId.ToString().ToLower();
	if (Id.Contains(TEXT("dress")) || Id.Contains(TEXT("wardrobe")))
	{
		return EApartmentLifePrimaryCameraMode::Wardrobe;
	}
	return EApartmentLifePrimaryCameraMode::Activity;
}
