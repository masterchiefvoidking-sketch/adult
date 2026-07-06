// Copyright Adult Anime Apartment Life. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ApartmentLifeImmersionTypes.h"
#include "ApartmentLifeDeveloperTypes.generated.h"

UENUM(BlueprintType)
enum class EApartmentLifeDeveloperHubSection : uint8
{
	Root			UMETA(DisplayName = "Developer Hub"),
	Character		UMETA(DisplayName = "Character"),
	Wardrobe		UMETA(DisplayName = "Wardrobe"),
	Furniture		UMETA(DisplayName = "Furniture"),
	Apartment		UMETA(DisplayName = "Apartment"),
	Activities		UMETA(DisplayName = "Activities"),
	Dialogue		UMETA(DisplayName = "Dialogue"),
	Economy			UMETA(DisplayName = "Economy"),
	Camera			UMETA(DisplayName = "Camera"),
	Lighting		UMETA(DisplayName = "Lighting"),
	Animation		UMETA(DisplayName = "Animation"),
	SaveSystem		UMETA(DisplayName = "Save System"),
	Performance		UMETA(DisplayName = "Performance"),
	ContentBrowser	UMETA(DisplayName = "Content Browser"),
	Validation		UMETA(DisplayName = "Data Validation"),
	ScreenshotStudio UMETA(DisplayName = "Screenshot Studio"),
	QuickActions	UMETA(DisplayName = "Quick Actions"),
	Backup			UMETA(DisplayName = "Backup Tools"),
	Dashboard		UMETA(DisplayName = "Project Dashboard")
};

UENUM(BlueprintType)
enum class EApartmentLifeDeveloperContentCategory : uint8
{
	All			UMETA(DisplayName = "All"),
	Clothing	UMETA(DisplayName = "Clothing"),
	Hairstyle	UMETA(DisplayName = "Hairstyle"),
	Furniture	UMETA(DisplayName = "Furniture"),
	Activity	UMETA(DisplayName = "Activity"),
	Routine		UMETA(DisplayName = "Routine"),
	Animation	UMETA(DisplayName = "Animation"),
	Dialogue	UMETA(DisplayName = "Dialogue"),
	YogaPose	UMETA(DisplayName = "Yoga Pose"),
	WorkType	UMETA(DisplayName = "Work Type"),
	Music		UMETA(DisplayName = "Music")
};

UENUM(BlueprintType)
enum class EApartmentLifeDeveloperValidationSeverity : uint8
{
	Info		UMETA(DisplayName = "Info"),
	Warning		UMETA(DisplayName = "Warning"),
	Error		UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EApartmentLifeDeveloperLightingLabPreset : uint8
{
	Morning		UMETA(DisplayName = "Morning"),
	Noon		UMETA(DisplayName = "Noon"),
	Sunset		UMETA(DisplayName = "Sunset"),
	Night		UMETA(DisplayName = "Night"),
	Rain		UMETA(DisplayName = "Rain"),
	Cloudy		UMETA(DisplayName = "Cloudy"),
	WarmIndoor	UMETA(DisplayName = "Warm Indoor"),
	CoolIndoor	UMETA(DisplayName = "Cool Indoor"),
	Studio		UMETA(DisplayName = "Studio")
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperContentEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FName EntryId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FText Detail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	EApartmentLifeDeveloperContentCategory Category = EApartmentLifeDeveloperContentCategory::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	bool bFavorite = false;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperValidationIssue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	EApartmentLifeDeveloperValidationSeverity Severity = EApartmentLifeDeveloperValidationSeverity::Warning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString Category;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString Message;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FName RelatedId;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperValidationReport
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	TArray<FApartmentLifeDeveloperValidationIssue> Issues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 ErrorCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 WarningCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString Summary;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperPerformanceSnapshot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float Fps = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float FrameTimeMs = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float GameThreadMs = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float RenderThreadMs = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float MemoryUsedMb = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 DrawCalls = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 ActiveAnimations = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 ActiveInteractions = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 SaveSizeKb = 0;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperDashboardCategory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 CompletionPercent = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString Notes;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperAnimationPreviewState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FName MontageId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FName AnimationGroupId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	bool bPlaying = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	bool bLooping = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	float PlaybackSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	EApartmentLifeFacialExpression FacialExpression = EApartmentLifeFacialExpression::Neutral;
};

USTRUCT(BlueprintType)
struct APARTMENTLIFEDEVTOOLS_API FApartmentLifeDeveloperHubState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	EApartmentLifeDeveloperHubSection ActiveSection = EApartmentLifeDeveloperHubSection::Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	EApartmentLifeDeveloperContentCategory ContentCategory = EApartmentLifeDeveloperContentCategory::All;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	FString SearchQuery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	bool bFavoritesOnly = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 SelectedContentIndex = INDEX_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 SourceSaveSlot = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Developer")
	int32 TargetSaveSlot = 1;
};
