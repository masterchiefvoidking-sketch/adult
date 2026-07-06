// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterAppearanceComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

UApartmentLifeCharacterAppearanceComponent::UApartmentLifeCharacterAppearanceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UApartmentLifeCharacterAppearanceComponent::InitializeFromData(UApartmentLifeCharacterCustomizationData* CustomizationData)
{
	CustomizationDataAsset = CustomizationData;
	if (CustomizationData)
	{
		ApplyAppearanceState(CustomizationData->DefaultAppearance);
	}
}

void UApartmentLifeCharacterAppearanceComponent::ApplyAppearanceState(const FApartmentLifeCharacterAppearanceState& NewState)
{
	AppearanceState = NewState;

	if (AActor* Owner = GetOwner())
	{
		if (USkeletalMeshComponent* BodyMesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (CustomizationDataAsset && CustomizationDataAsset->BaseBodyMesh)
			{
				BodyMesh->SetSkeletalMesh(CustomizationDataAsset->BaseBodyMesh);
			}

			BodyMesh->SetRelativeScale3D(FVector(AppearanceState.HeightCm / 165.f));
			ApplyMorphs(BodyMesh, AppearanceState.BodyProportions);
			ApplyMorphs(BodyMesh, AppearanceState.FaceMorphs);
			ApplyColorParameter(BodyMesh, AppearanceState.SkinTone);
			ApplyColorParameter(BodyMesh, AppearanceState.EyeColor);
			ApplyColorParameter(BodyMesh, AppearanceState.HairColor);
		}
	}

	OnAppearanceUpdated.Broadcast();
}

void UApartmentLifeCharacterAppearanceComponent::ApplyMorphs(USkeletalMeshComponent* Mesh, const TArray<FApartmentLifeMorphSlider>& Morphs) const
{
	if (!Mesh)
	{
		return;
	}

	for (const FApartmentLifeMorphSlider& Morph : Morphs)
	{
		if (!Morph.MorphTargetName.IsNone())
		{
			Mesh->SetMorphTarget(Morph.MorphTargetName, Morph.Value);
		}
	}
}

void UApartmentLifeCharacterAppearanceComponent::ApplyColorParameter(UMeshComponent* Mesh, const FApartmentLifeColorCustomization& ColorData) const
{
	if (!Mesh || ColorData.MaterialParameterName.IsNone())
	{
		return;
	}

	const int32 MaterialCount = Mesh->GetNumMaterials();
	for (int32 Index = 0; Index < MaterialCount; ++Index)
	{
		if (UMaterialInstanceDynamic* MID = Mesh->CreateAndSetMaterialInstanceDynamic(Index))
		{
			MID->SetVectorParameterValue(ColorData.MaterialParameterName, ColorData.Color);
		}
	}
}

FString UApartmentLifeCharacterAppearanceComponent::GetSaveId_Implementation() const
{
	if (const AActor* Owner = GetOwner())
	{
		return FString::Printf(TEXT("appearance_%s"), *Owner->GetName());
	}
	return TEXT("appearance_unknown");
}

void UApartmentLifeCharacterAppearanceComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	OutData.Add(TEXT("HeightCm"), FString::SanitizeFloat(AppearanceState.HeightCm));
	OutData.Add(TEXT("HairstyleId"), AppearanceState.HairstyleId.ToString());
	OutData.Add(TEXT("VoiceProfileId"), AppearanceState.VoiceProfileId.ToString());
}

void UApartmentLifeCharacterAppearanceComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	FApartmentLifeCharacterAppearanceState Restored = AppearanceState;

	if (const FString* Height = InData.Find(TEXT("HeightCm")))
	{
		Restored.HeightCm = FCString::Atof(**Height);
	}
	if (const FString* Hairstyle = InData.Find(TEXT("HairstyleId")))
	{
		Restored.HairstyleId = FName(**Hairstyle);
	}
	if (const FString* Voice = InData.Find(TEXT("VoiceProfileId")))
	{
		Restored.VoiceProfileId = FName(**Voice);
	}

	ApplyAppearanceState(Restored);
}
