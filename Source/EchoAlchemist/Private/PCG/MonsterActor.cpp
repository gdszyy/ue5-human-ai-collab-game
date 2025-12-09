// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/MonsterActor.h"
#include "PCG/AppearanceAssembler.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Engine.h"

AMonsterActor::AMonsterActor()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create root component
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = RootSceneComponent;

    // Create base flipbook component
    BaseFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BaseFlipbook"));
    BaseFlipbookComponent->SetupAttachment(RootComponent);
}

void AMonsterActor::BeginPlay()
{
    Super::BeginPlay();

    // If attributes are set in the editor, reconstruct appearance
    if (MonsterAttributes.EcologyAttributes.Habitat != EHabitatType::Forest || 
        MonsterAttributes.CombatAttributes.Traits.Num() > 0)
    {
        ReconstructAppearance();
    }
}

void AMonsterActor::SetMonsterAttributes(const FMonsterAttributes& Attributes)
{
    MonsterAttributes = Attributes;
    ReconstructAppearance();
}

void AMonsterActor::ReconstructAppearance()
{
    // Clear existing appearance
    ClearAppearance();

    // Construct new appearance
    ConstructBaseSkeleton();
    AttachParts();
    ApplyPaletteSwapping();
    ApplyScale();
}

void AMonsterActor::PlayAnimation(EMonsterAnimationType AnimationType)
{
    if (!BaseFlipbookComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: BaseFlipbookComponent is null, cannot play animation"));
        return;
    }

    if (!CurrentSkeletonData.IdleFlipbook)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: No skeleton data loaded, cannot play animation"));
        return;
    }

    UPaperFlipbook* FlipbookToPlay = nullptr;

    switch (AnimationType)
    {
    case EMonsterAnimationType::Idle:
        FlipbookToPlay = CurrentSkeletonData.IdleFlipbook;
        break;
    case EMonsterAnimationType::Walk:
        FlipbookToPlay = CurrentSkeletonData.WalkFlipbook;
        break;
    case EMonsterAnimationType::Attack:
        FlipbookToPlay = CurrentSkeletonData.AttackFlipbook;
        break;
    case EMonsterAnimationType::Death:
        FlipbookToPlay = CurrentSkeletonData.DeathFlipbook;
        break;
    }

    if (FlipbookToPlay)
    {
        BaseFlipbookComponent->SetFlipbook(FlipbookToPlay);
        BaseFlipbookComponent->Play();
    }
    else
    {
        // Fallback to idle animation if specific animation is not available
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: Animation type not available, falling back to Idle"));
        if (CurrentSkeletonData.IdleFlipbook)
        {
            BaseFlipbookComponent->SetFlipbook(CurrentSkeletonData.IdleFlipbook);
            BaseFlipbookComponent->Play();
        }
    }
}

void AMonsterActor::ClearAppearance()
{
    // Destroy all part components
    for (UPaperSpriteComponent* PartComponent : PartComponents)
    {
        if (PartComponent)
        {
            PartComponent->DestroyComponent();
        }
    }
    PartComponents.Empty();

    // Clear base flipbook
    if (BaseFlipbookComponent)
    {
        BaseFlipbookComponent->SetFlipbook(nullptr);
    }
}

void AMonsterActor::ConstructBaseSkeleton()
{
    if (!SkeletonDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: SkeletonDataTable is not set!"));
        return;
    }

    if (!BaseFlipbookComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: BaseFlipbookComponent is null!"));
        return;
    }

    // Select skeleton based on ecology attributes
    FSkeletonData SkeletonData;
    if (UAppearanceAssembler::SelectSkeleton(MonsterAttributes.EcologyAttributes, SkeletonDataTable, SkeletonData))
    {
        CurrentSkeletonData = SkeletonData;

        // Set idle flipbook as default
        if (SkeletonData.IdleFlipbook)
        {
            BaseFlipbookComponent->SetFlipbook(SkeletonData.IdleFlipbook);
            BaseFlipbookComponent->Play();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MonsterActor: Skeleton has no idle flipbook"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: Failed to select skeleton for habitat %d, size %d"),
               static_cast<int32>(MonsterAttributes.EcologyAttributes.Habitat),
               static_cast<int32>(MonsterAttributes.EcologyAttributes.SizeClass));
    }
}

void AMonsterActor::AttachParts()
{
    if (!PartDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: PartDataTable is not set, skipping part attachment"));
        return;
    }

    if (!BaseFlipbookComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: BaseFlipbookComponent is null!"));
        return;
    }

    // Select parts based on combat attributes
    TArray<FPartData> SelectedParts = UAppearanceAssembler::SelectParts(MonsterAttributes.CombatAttributes, PartDataTable);

    UE_LOG(LogTemp, Log, TEXT("MonsterActor: Attaching %d parts"), SelectedParts.Num());

    // Create and attach part components
    for (const FPartData& PartData : SelectedParts)
    {
        if (!PartData.PartSprite)
        {
            UE_LOG(LogTemp, Warning, TEXT("MonsterActor: Part has no sprite, skipping"));
            continue;
        }

        // Create part sprite component
        UPaperSpriteComponent* PartComponent = NewObject<UPaperSpriteComponent>(this);
        if (PartComponent)
        {
            PartComponent->RegisterComponent();
            PartComponent->AttachToComponent(BaseFlipbookComponent, FAttachmentTransformRules::KeepRelativeTransform);
            PartComponent->SetSprite(PartData.PartSprite);

            // Set relative transform
            PartComponent->SetRelativeLocation(PartData.RelativeLocation);
            PartComponent->SetRelativeRotation(PartData.RelativeRotation);
            PartComponent->SetRelativeScale3D(PartData.RelativeScale);

            // Set Z-order (translucent sort priority)
            PartComponent->SetTranslucentSortPriority(PartData.ZOrder);

            // Apply palette swapping to part if material is available
            if (PaletteSwapMaterial && CurrentPaletteData.Habitat != EHabitatType::Forest)
            {
                UMaterialInstanceDynamic* PartMaterial = UAppearanceAssembler::CreatePaletteSwapMaterial(PaletteSwapMaterial, CurrentPaletteData);
                if (PartMaterial)
                {
                    PartComponent->SetMaterial(0, PartMaterial);
                }
            }

            PartComponents.Add(PartComponent);
        }
    }
}

void AMonsterActor::ApplyPaletteSwapping()
{
    if (!PaletteDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: PaletteDataTable is not set, using default colors"));
        return;
    }

    if (!PaletteSwapMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("MonsterActor: PaletteSwapMaterial is not set, using default material"));
        return;
    }

    if (!BaseFlipbookComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: BaseFlipbookComponent is null!"));
        return;
    }

    // Get palette for habitat
    FPaletteData PaletteData;
    if (UAppearanceAssembler::GetPaletteForHabitat(MonsterAttributes.EcologyAttributes.Habitat, PaletteDataTable, PaletteData))
    {
        CurrentPaletteData = PaletteData;

        // Create palette swap material
        UMaterialInstanceDynamic* DynamicMaterial = UAppearanceAssembler::CreatePaletteSwapMaterial(PaletteSwapMaterial, PaletteData);
        if (DynamicMaterial)
        {
            BaseFlipbookComponent->SetMaterial(0, DynamicMaterial);
            UE_LOG(LogTemp, Log, TEXT("MonsterActor: Applied palette for habitat %d"),
                   static_cast<int32>(MonsterAttributes.EcologyAttributes.Habitat));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("MonsterActor: Failed to create palette swap material"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("MonsterActor: Failed to get palette for habitat %d"),
               static_cast<int32>(MonsterAttributes.EcologyAttributes.Habitat));
    }
}

void AMonsterActor::ApplyScale()
{
    if (!BaseFlipbookComponent)
    {
        return;
    }

    // Calculate final scale
    float FinalScale = UAppearanceAssembler::CalculateMonsterScale(
        MonsterAttributes.EcologyAttributes.SizeClass,
        CurrentSkeletonData.BaseScale
    );

    // Apply scale to base flipbook component
    BaseFlipbookComponent->SetRelativeScale3D(FVector(FinalScale, FinalScale, FinalScale));
}
