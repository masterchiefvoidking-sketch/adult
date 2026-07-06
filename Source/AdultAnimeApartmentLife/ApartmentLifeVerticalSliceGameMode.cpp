// Copyright Adult Anime Apartment Life. All Rights Reserved.

#include "ApartmentLifeVerticalSliceGameMode.h"
#include "ApartmentLifeSlicePlayerController.h"
#include "ApartmentLifeSimCharacter.h"
#include "ApartmentLifeApartmentUnit.h"
#include "ApartmentLifeFurniturePlacementComponent.h"
#include "ApartmentLifeFurnitureActor.h"
#include "ApartmentLifeNPCSimulationComponent.h"
#include "ApartmentLifeSaveSubsystem.h"
#include "ApartmentLifeSocialSubsystem.h"
#include "Engine/World.h"

AApartmentLifeVerticalSliceGameMode::AApartmentLifeVerticalSliceGameMode()
{
	PlayerControllerClass = AApartmentLifeSlicePlayerController::StaticClass();
}

void AApartmentLifeVerticalSliceGameMode::BeginPlay()
{
	Super::BeginPlay();
	BootstrapSlice();

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

void AApartmentLifeVerticalSliceGameMode::BootstrapSlice()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ApartmentUnit = World->SpawnActor<AApartmentLifeApartmentUnit>(AApartmentLifeApartmentUnit::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);
	PlayerCharacter = World->SpawnActor<AApartmentLifeSimCharacter>(AApartmentLifeSimCharacter::StaticClass(), FVector(120.f, 0.f, 0.f), FRotator(0.f, 180.f, 0.f), Params);
	PartnerCharacter = World->SpawnActor<AApartmentLifeSimCharacter>(AApartmentLifeSimCharacter::StaticClass(), FVector(220.f, 120.f, 0.f), FRotator(0.f, -90.f, 0.f), Params);

	bool bSeedFurniture = true;
	if (bAutoLoadOnStart)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UApartmentLifeSaveSubsystem* SaveSubsystem = GI->GetSubsystem<UApartmentLifeSaveSubsystem>())
			{
				bSeedFurniture = !SaveSubsystem->DoesSaveExist(AutoLoadSlot);
			}
		}
	}

	if (ApartmentUnit && bSeedFurniture)
	{
		SeedStarterFurniture(ApartmentUnit);
	}

	ConfigureCharacter(PlayerCharacter, FName(TEXT("player.main")), FVector(120.f, 0.f, 0.f));
	ConfigureCharacter(PartnerCharacter, FName(TEXT("npc.partner")), FVector(220.f, 120.f, 0.f));

	if (PlayerCharacter && PartnerCharacter)
	{
		if (UApartmentLifeNPCSimulationComponent* PlayerSim = PlayerCharacter->GetSimulationComponent())
		{
			PlayerSim->AdjustRelationship(FName(TEXT("npc.partner")), 40.f, 35.f, 30.f);
		}
		if (UApartmentLifeNPCSimulationComponent* PartnerSim = PartnerCharacter->GetSimulationComponent())
		{
			PartnerSim->AdjustRelationship(FName(TEXT("player.main")), 40.f, 35.f, 30.f);
			PartnerSim->Personality.Extroversion = 0.7f;
			PartnerSim->Personality.CookingInterest = 0.8f;
		}
	}

	LinkPlayerController();
}

void AApartmentLifeVerticalSliceGameMode::SeedStarterFurniture(AApartmentLifeApartmentUnit* Apartment)
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
		{ FName(TEXT("furniture.sofa.default")), EApartmentLifeRoomType::LivingRoom, FVector(80.f, 60.f, 0.f), EApartmentLifeFurnitureCategory::Sofa },
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

void AApartmentLifeVerticalSliceGameMode::ConfigureCharacter(AApartmentLifeSimCharacter* Character, FName CharacterId, const FVector& Location)
{
	if (!Character)
	{
		return;
	}

	Character->SetActorLocation(Location);

	if (UApartmentLifeNPCSimulationComponent* Sim = Character->GetSimulationComponent())
	{
		Sim->CharacterId = CharacterId;
	}

	if (UGameInstance* GI = GetGameInstance())
	{
		if (UApartmentLifeSocialSubsystem* Social = GI->GetSubsystem<UApartmentLifeSocialSubsystem>())
		{
			Social->RegisterSimCharacter(Character);
		}
	}
}

void AApartmentLifeVerticalSliceGameMode::LinkPlayerController()
{
	if (AApartmentLifeSlicePlayerController* SlicePC = Cast<AApartmentLifeSlicePlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		SlicePC->SetSliceContext(ApartmentUnit, PlayerCharacter, PartnerCharacter);
	}
}
