// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeBodyCustomizationComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "JsonObjectConverter.h"

void UApartmentLifeBodyCustomizationComponent::ApplyBodyCustomization(const FApartmentLifeBodyCustomization& Body)
{
	BodyState = Body;
	RecalculateFitProfile();
	UpdateMeshAndCollision();
	OnBodyCustomizationUpdated.Broadcast(FitProfile);
}

void UApartmentLifeBodyCustomizationComponent::UpdateMeshAndCollision()
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	if (USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
	{
		Mesh->SetRelativeScale3D(BodyState.GetScaleVector());
		for (const FApartmentLifeMorphSlider& Morph : BodyState.ToMorphSliders())
		{
			if (!Morph.MorphTargetName.IsNone())
			{
				Mesh->SetMorphTarget(Morph.MorphTargetName, Morph.Value);
			}
		}
	}

	if (ACharacter* Character = Cast<ACharacter>(Owner))
	{
		if (UCapsuleComponent* Capsule = Character->GetCapsuleComponent())
		{
			Capsule->SetCapsuleRadius(FitProfile.CollisionRadius);
			Capsule->SetCapsuleHalfHeight(BodyState.HeightCm * 0.5f * (BodyState.GetScaleVector().Z / (BodyState.HeightCm / 165.f)));
		}
	}
}

void UApartmentLifeBodyCustomizationComponent::RecalculateFitProfile()
{
	FitProfile.ScaleAdjustment = BodyState.GetScaleVector();
	FitProfile.SleeveLengthOffset = (BodyState.ArmLength - 0.5f) * 4.f;
	FitProfile.PantLengthOffset = (BodyState.LegLength - 0.5f) * 4.f;
	FitProfile.CollisionRadius = 30.f + BodyState.ShoulderWidth * 8.f;
}

FName UApartmentLifeBodyCustomizationComponent::GetWalkStyleId() const
{
	return FName(*FString::Printf(TEXT("anim.walk.%s"), *UEnum::GetValueAsString(BodyState.WalkStyle)));
}

FName UApartmentLifeBodyCustomizationComponent::GetIdleStyleId() const
{
	return FName(*FString::Printf(TEXT("anim.idle.%s"), *UEnum::GetValueAsString(BodyState.IdleStyle)));
}

FString UApartmentLifeBodyCustomizationComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("body_%s"), *Owner->GetName());
	}
	return TEXT("body_unknown");
}

void UApartmentLifeBodyCustomizationComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(BodyState, Json);
	OutData.Add(TEXT("BodyState"), Json);
}

void UApartmentLifeBodyCustomizationComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	if (const FString* Json = InData.Find(TEXT("BodyState")))
	{
		FApartmentLifeBodyCustomization Restored;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &Restored))
		{
			ApplyBodyCustomization(Restored);
		}
	}
}
