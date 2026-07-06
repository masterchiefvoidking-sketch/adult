// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ApartmentLifeDeveloperTypes.h"
#include "ApartmentLifeDeveloperUiController.generated.h"

class AApartmentLifeApartmentUnit;
class APlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnApartmentLifeDeveloperUiStateChanged);

UCLASS(ClassGroup = Developer, meta = (BlueprintSpawnableComponent))
class APARTMENTLIFEDEVTOOLS_API UApartmentLifeDeveloperUiController : public UActorComponent
{
	GENERATED_BODY()

public:
	void InitializeContext(
		APlayerController* InController,
		AApartmentLifeApartmentUnit* InApartment,
		AActor* InGirlCharacter);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void OpenDeveloperHub();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void CloseDeveloperHub();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void ToggleDeveloperHub();

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer|UI")
	bool IsDeveloperHubOpen() const { return bIsOpen; }

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void NavigateToSection(EApartmentLifeDeveloperHubSection Section);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void SetContentCategory(EApartmentLifeDeveloperContentCategory Category);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void SetSearchQuery(const FString& Query);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void ToggleFavoritesOnly();

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void SelectContentIndex(int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Apartment Life|Developer|UI")
	void ActivateEntryIndex(int32 Index);

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer|UI")
	const TArray<FApartmentLifeDeveloperContentEntry>& GetVisibleEntries() const { return VisibleEntries; }

	UFUNCTION(BlueprintPure, Category = "Apartment Life|Developer|UI")
	EApartmentLifeDeveloperHubSection GetActiveSection() const;

	UPROPERTY(BlueprintAssignable, Category = "Apartment Life|Developer|UI")
	FOnApartmentLifeDeveloperUiStateChanged OnDeveloperUiStateChanged;

protected:
	void RefreshContentList();
	void BroadcastStateChanged();

	UPROPERTY()
	TWeakObjectPtr<APlayerController> OwnerController;

	UPROPERTY()
	TWeakObjectPtr<AApartmentLifeApartmentUnit> ApartmentUnit;

	UPROPERTY()
	TWeakObjectPtr<AActor> GirlCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Developer|UI")
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Developer|UI")
	TArray<FApartmentLifeDeveloperContentEntry> VisibleEntries;
};
