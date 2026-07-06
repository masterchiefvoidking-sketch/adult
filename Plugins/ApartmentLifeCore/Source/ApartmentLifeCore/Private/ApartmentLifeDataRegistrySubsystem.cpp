// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeDataRegistrySubsystem.h"

void UApartmentLifeDataRegistrySubsystem::RegisterAsset(UApartmentLifePrimaryDataAsset* Asset)
{
	if (!Asset || Asset->AssetId.IsNone())
	{
		return;
	}

	AssetMap.Add(Asset->AssetId, Asset);

	const FName TypeName = Asset->GetPrimaryAssetType();
	AssetsByType.FindOrAdd(TypeName).AddUnique(Asset);
}

void UApartmentLifeDataRegistrySubsystem::UnregisterAsset(FName AssetId)
{
	if (UApartmentLifePrimaryDataAsset* const* Found = AssetMap.Find(AssetId))
	{
		if (UApartmentLifePrimaryDataAsset* Asset = *Found)
		{
			if (TArray<UApartmentLifePrimaryDataAsset*>* TypeList = AssetsByType.Find(Asset->GetPrimaryAssetType()))
			{
				TypeList->Remove(Asset);
			}
		}
		AssetMap.Remove(AssetId);
	}
}

UApartmentLifePrimaryDataAsset* UApartmentLifeDataRegistrySubsystem::FindAsset(FName AssetId) const
{
	if (UApartmentLifePrimaryDataAsset* const* Found = AssetMap.Find(AssetId))
	{
		return *Found;
	}
	return nullptr;
}

TArray<UApartmentLifePrimaryDataAsset*> UApartmentLifeDataRegistrySubsystem::GetAssetsByType(FName AssetType) const
{
	if (const TArray<UApartmentLifePrimaryDataAsset*>* Found = AssetsByType.Find(AssetType))
	{
		return *Found;
	}
	return {};
}
