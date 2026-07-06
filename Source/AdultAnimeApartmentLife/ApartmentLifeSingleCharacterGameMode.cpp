// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeSingleCharacterGameMode.h"
#include "ApartmentLifeSingleCharacterPlayerController.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeFurniturePlacementComponent.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeGirlLifeLibrary.h"
#include "ApartmentLifeWardrobeComponent.h"
#include "ApartmentLifeWardrobeBootstrapLibrary.h"
#include "ApartmentLifeProgressionComponent.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "Engine/World.h"

AApartmentLifeSingleCharacterGameMode::AApartmentLifeSingleCharacterGameMode()
{
	PlayerControllerClass = AApartmentLifeSingleCharacterPlayerController::StaticClass();
}

void AApartmentLifeSingleCharacterGameMode::BeginPlay()
{
	Super::BeginPlay();
	BootstrapApartment();

	if (bAutoLoadOnStart)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
			{
				if (SaveSubsystem->DoesSaveExist(AutoLoadSlot))
				{
					SaveSubsystem->LoadFromSlot(AutoLoadSlot);
				}
			}
		}
	}
}

void AApartmentLifeSingleCharacterGameMode::BootstrapApartment()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ApartmentUnit = World->SpawnActor<AApartmentLifeApartmentUnit>(AApartmentLifeApartmentUnit::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	GirlCharacter = World->SpawnActor<AApartmentLifeSimCharacter>(AApartmentLifeSimCharacter::StaticClass(), FVector(0.f, 0.f, 0.f), FRotator(0.f, 180.f, 0.f), Params);

	bool bSeedFurniture = true;
	bool bSeedWardrobe = true;
	if (bAutoLoadOnStart)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
			{
				const bool bHasSave = SaveSubsystem->DoesSaveExist(AutoLoadSlot);
				bSeedFurniture = !bHasSave;
				bSeedWardrobe = !bHasSave;
			}
		}
	}

	if (ApartmentUnit && bSeedFurniture)
	{
		SeedStarterFurniture(ApartmentUnit);
	}

	ConfigureGirlCharacter(GirlCharacter, bSeedWardrobe);
	LinkPlayerController();
}

void AApartmentLifeSingleCharacterGameMode::SeedStarterFurniture(AApartmentLifeApartmentUnit* Apartment)
{
	if (!Apartment)
	{
		return;
	}

	struct FSliceFurnitureSeed
	{
		FName ItemId;
		EApartmentLifeRoomType Room;
		FVector Location;
		EApartmentLifeFurnitureCategory Category;
	};

	const TArray<FSliceFurnitureSeed> Seeds = {
		{ FName(TEXT("furniture.bed.default")), EApartmentLifeRoomType::Bedroom, FVector(-150.f, -80.f, 0.f), EApartmentLifeFurnitureCategory::Bed },
		{ FName(TEXT("furniture.closet.default")), EApartmentLifeRoomType::Bedroom, FVector(-220.f, 40.f, 0.f), EApartmentLifeFurnitureCategory::Closet },
		{ FName(TEXT("furniture.desk.default")), EApartmentLifeRoomType::Office, FVector(-40.f, -120.f, 0.f), EApartmentLifeFurnitureCategory::Desk },
		{ FName(TEXT("furniture.sofa.default")), EApartmentLifeRoomType::LivingRoom, FVector(80.f, 60.f, 0.f), EApartmentLifeFurnitureCategory::Sofa },
		{ FName(TEXT("furniture.tv.default")), EApartmentLifeRoomType::LivingRoom, FVector(120.f, 20.f, 80.f), EApartmentLifeFurnitureCategory::TV },
		{ FName(TEXT("furniture.kitchen.default")), EApartmentLifeRoomType::Kitchen, FVector(140.f, 160.f, 0.f), EApartmentLifeFurnitureCategory::KitchenAppliance },
		{ FName(TEXT("furniture.fridge.default")), EApartmentLifeRoomType::Kitchen, FVector(200.f, 160.f, 0.f), EApartmentLifeFurnitureCategory::KitchenAppliance },
		{ FName(TEXT("furniture.shower.default")), EApartmentLifeRoomType::Bathroom, FVector(-80.f, 180.f, 0.f), EApartmentLifeFurnitureCategory::BathroomFixture },
		{ FName(TEXT("furniture.mirror.default")), EApartmentLifeRoomType::Bathroom, FVector(20.f, 180.f, 0.f), EApartmentLifeFurnitureCategory::Mirror },
		{ FName(TEXT("furniture.yoga_mat.default")), EApartmentLifeRoomType::LivingRoom, FVector(180.f, -40.f, 0.f), EApartmentLifeFurnitureCategory::WorkoutEquipment },
	};

	UApartmentLifeFurniturePlacementComponent* Placement = Apartment->FindComponentByClass<UApartmentLifeFurniturePlacementComponent>();

	for (const FSliceFurnitureSeed& Seed : Seeds)
	{
		FApartmentLifePlacedFurnitureInstance Instance;
		Instance.InstanceId = FGuid::NewGuid();
		Instance.FurnitureItemId = Seed.ItemId;
		Instance.Room = Seed.Room;
		Instance.Transform = FTransform(FRotator::ZeroRotator, Seed.Location);

		if (Apartment->PlaceFurnitureInstance(Instance) && Placement)
		{
			if (AApartmentLifeFurnitureActor* Actor = Placement->GetSpawnedFurnitureActor(Instance.InstanceId))
			{
				Actor->SetInteractionsForCategory(Seed.Category);
			}
		}
	}
}

void AApartmentLifeSingleCharacterGameMode::ConfigureGirlCharacter(AApartmentLifeSimCharacter* Character, bool bSeedWardrobe)
{
	if (!Character)
	{
		return;
	}

	if (UApartmentLifeNPCSimulationComponent* Sim = Character->GetSimulationComponent())
	{
		Sim->CharacterId = FName(TEXT("character.main"));
		UApartmentLifeGirlLifeLibrary::ConfigureRemoteWorkCareer(Sim, RemoteWorkTypeId);
		Sim->Personality.Kindness = 0.75f;
		Sim->Personality.Empathy = 0.8f;
		Sim->Personality.FitnessInterest = 0.65f;
		Sim->AffectionTowardPlayer = 55.f;
		Sim->TrustTowardPlayer = 50.f;
	}

	if (bSeedWardrobe)
	{
		if (UApartmentLifeWardrobeComponent* Wardrobe = Character->GetWardrobeComponent())
		{
			UApartmentLifeWardrobeBootstrapLibrary::SeedVerticalSliceWardrobe(Wardrobe);
		}

		if (UApartmentLifeProgressionComponent* Progression = Character->GetProgressionComponent())
		{
			Progression->SeedStarterUnlocks();
		}
	}
}

void AApartmentLifeSingleCharacterGameMode::LinkPlayerController()
{
	if (AApartmentLifeSingleCharacterPlayerController* PC = Cast<AApartmentLifeSingleCharacterPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PC->SetSingleCharacterContext(ApartmentUnit, GirlCharacter);
	}
}
