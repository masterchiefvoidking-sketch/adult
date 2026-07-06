// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeSaveable.h"
#include "ApartmentLifeBedroomRoutineComponent.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeBedroomRoutineType : uint8
{
	None	UMETA(DisplayName = "None"),
	Sleep	UMETA(DisplayName = "Sleep"),
	Nap		UMETA(DisplayName = "Nap"),
	Relax	UMETA(DisplayName = "Relax"),
	Read	UMETA(DisplayName = "Read")
};

UCLASS(ClassGroup = Character, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFECHARACTERPIPELINE_API UApartmentLifeBedroomRoutineComponent : public UActorComponent, public IApartmentLifeSaveable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Bedroom")
	void StartRoutine(EApartmentLifeBedroomRoutineType RoutineType);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Bedroom")
	void EndRoutine();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Bedroom")
	EApartmentLifeBedroomRoutineType GetActiveRoutine() const { return ActiveRoutine; }

	virtual FString GetSaveId_Implementation() const override;
	virtual void CaptureSaveData_Implementation(TMap<FString, FString>& OutData) const override;
	virtual void RestoreSaveData_Implementation(const TMap<FString, FString>& InData) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bedroom")
	EApartmentLifeBedroomRoutineType ActiveRoutine = EApartmentLifeBedroomRoutineType::None;
};
