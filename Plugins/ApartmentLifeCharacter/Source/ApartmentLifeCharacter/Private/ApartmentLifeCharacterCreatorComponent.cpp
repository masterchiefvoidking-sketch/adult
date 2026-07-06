// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeCharacterCreatorComponent.h"
#include "ApartmentLifeBodyCustomizationComponent.h"
#include "ApartmentLifeCharacterCreatorCatalogData.h"
#include "JsonObjectConverter.h"

void UApartmentLifeCharacterCreatorComponent::InitializeFromCatalog(UApartmentLifeMorphCatalogData* Catalog)
{
	MorphCatalog = Catalog;
	if (Catalog)
	{
		ApplyCreatorState(Catalog->DefaultCreatorState);
	}
}

void UApartmentLifeCharacterCreatorComponent::ApplyCreatorState(const FApartmentLifeCharacterCreatorState& NewState)
{
	CreatorState = NewState;
	ApplyFullAppearance();
	SyncBodyComponent();
	OnCreatorStateUpdated.Broadcast(CreatorState);
}

void UApartmentLifeCharacterCreatorComponent::SetFaceMorph(EApartmentLifeFaceRegion Region, FName MorphName, float Value)
{
	auto SetInArray = [MorphName, Value](TArray<FApartmentLifeMorphSlider>& Morphs)
	{
		for (FApartmentLifeMorphSlider& M : Morphs)
		{
			if (M.MorphTargetName == MorphName)
			{
				M.Value = Value;
				return;
			}
		}
		FApartmentLifeMorphSlider NewMorph;
		NewMorph.MorphTargetName = MorphName;
		NewMorph.Value = Value;
		Morphs.Add(NewMorph);
	};

	switch (Region)
	{
	case EApartmentLifeFaceRegion::FaceShape: SetInArray(CreatorState.Face.FaceShapeMorphs); break;
	case EApartmentLifeFaceRegion::Eyes: SetInArray(CreatorState.Face.EyeMorphs); break;
	case EApartmentLifeFaceRegion::Eyebrows: SetInArray(CreatorState.Face.EyebrowMorphs); break;
	case EApartmentLifeFaceRegion::Nose: SetInArray(CreatorState.Face.NoseMorphs); break;
	case EApartmentLifeFaceRegion::Lips: SetInArray(CreatorState.Face.LipMorphs); break;
	case EApartmentLifeFaceRegion::Jaw: SetInArray(CreatorState.Face.JawMorphs); break;
	case EApartmentLifeFaceRegion::Cheeks: SetInArray(CreatorState.Face.CheekMorphs); break;
	case EApartmentLifeFaceRegion::Ears: SetInArray(CreatorState.Face.EarMorphs); break;
	}

	ApplyFullAppearance();
}

void UApartmentLifeCharacterCreatorComponent::RandomizeFace()
{
	auto RandomizeArray = [](TArray<FApartmentLifeMorphSlider>& Morphs)
	{
		for (FApartmentLifeMorphSlider& M : Morphs)
		{
			M.Value = FMath::FRandRange(0.2f, 0.8f);
		}
	};

	RandomizeArray(CreatorState.Face.FaceShapeMorphs);
	RandomizeArray(CreatorState.Face.EyeMorphs);
	ApplyFullAppearance();
}

void UApartmentLifeCharacterCreatorComponent::SyncBodyComponent()
{
	if (AActor* Owner = GetOwner())
	{
		if (UApartmentLifeBodyCustomizationComponent* Body = Owner->FindComponentByClass<UApartmentLifeBodyCustomizationComponent>())
		{
			Body->ApplyBodyCustomization(CreatorState.Body);
		}
	}
}

void UApartmentLifeCharacterCreatorComponent::ApplyFullAppearance()
{
	ApplyAppearanceState(CreatorState.ToLegacyAppearanceState());
}

void UApartmentLifeCharacterCreatorComponent::CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const
{
	Super::CaptureSaveData_Implementation(OutData);
	FString Json;
	FJsonObjectConverter::UStructToJsonObjectString(CreatorState, Json);
	OutData.Add(TEXT("CreatorState"), Json);
}

void UApartmentLifeCharacterCreatorComponent::RestoreSaveData_Implementation(const TMap<FString, FString>& InData)
{
	Super::RestoreSaveData_Implementation(InData);
	if (const FString* Json = InData.Find(TEXT("CreatorState")))
	{
		FApartmentLifeCharacterCreatorState Restored;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(*Json, &Restored))
		{
			ApplyCreatorState(Restored);
		}
	}
}
