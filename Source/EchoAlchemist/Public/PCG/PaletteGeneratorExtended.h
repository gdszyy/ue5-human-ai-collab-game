// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCG/MonsterAttributes.h"
#include "PCG/PaletteGenerator.h"
#include "Engine/Texture2D.h"
#include "PaletteGeneratorExtended.generated.h"

/**
 * @struct FHabitatColorSpectrum
 * @brief Defines the color spectrum for a specific habitat type.
 * 
 * Each habitat has a base color spectrum that defines the range of colors
 * that monsters from that habitat can have.
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FHabitatColorSpectrum
{
    GENERATED_BODY()

    /** Habitat type this spectrum belongs to */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    EHabitatType Habitat = EHabitatType::Forest;

    /** Primary color spectrum (body colors) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> PrimarySpectrum;

    /** Secondary color spectrum (detail colors) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> SecondarySpectrum;

    /** Accent color spectrum (highlight colors) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> AccentSpectrum;

    /** Default constructor */
    FHabitatColorSpectrum()
        : Habitat(EHabitatType::Forest)
    {
    }
};

/**
 * @struct FMonsterColorPalette
 * @brief A complete color palette for a monster, including primary, secondary, and accent colors.
 * 
 * This palette can be applied to monster materials using palette swap techniques.
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FMonsterColorPalette
{
    GENERATED_BODY()

    /** Primary color (main body color) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor PrimaryColor;

    /** Secondary color (detail color) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor SecondaryColor;

    /** Accent color (highlight color) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor AccentColor;

    /** Default constructor */
    FMonsterColorPalette()
        : PrimaryColor(FLinearColor::White)
        , SecondaryColor(FLinearColor::Gray)
        , AccentColor(FLinearColor::Black)
    {
    }
};

/**
 * @class UPaletteGeneratorExtended
 * @brief Extended palette generator with ecology-driven color generation.
 * 
 * This class extends the base PaletteGenerator with intelligent color generation
 * based on monster ecology attributes (habitat, rarity) and combat attributes (traits).
 */
UCLASS()
class ECHOALCHEMIST_API UPaletteGeneratorExtended : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Generates a color palette for a monster based on its ecology and combat attributes.
     * 
     * @param EcologyAttributes The ecology attributes of the monster (habitat, size, etc.)
     * @param CombatAttributes The combat attributes of the monster (traits)
     * @param Seed Random seed for deterministic generation
     * @return A complete color palette for the monster
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", 
        meta = (DisplayName = "Generate Monster Palette", 
                Keywords = "color palette monster ecology"))
    static FMonsterColorPalette GenerateMonsterPalette(
        const FEcologyAttributes& EcologyAttributes,
        const FCombatAttributes& CombatAttributes,
        int32 Seed = 0);

    /**
     * Gets the default color spectrum for a specific habitat type.
     * 
     * @param Habitat The habitat type
     * @return The color spectrum for that habitat
     */
    UFUNCTION(BlueprintPure, Category = "PCG | Palette",
        meta = (DisplayName = "Get Habitat Color Spectrum",
                Keywords = "color spectrum habitat"))
    static FHabitatColorSpectrum GetHabitatColorSpectrum(EHabitatType Habitat);

    /**
     * Adjusts a color palette based on rarity level.
     * Rare and epic monsters get more vibrant and saturated colors.
     * 
     * @param BasePalette The base color palette
     * @param Rarity The rarity level
     * @return The adjusted color palette
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette",
        meta = (DisplayName = "Adjust Palette By Rarity",
                Keywords = "color palette rarity adjust"))
    static FMonsterColorPalette AdjustPaletteByRarity(
        const FMonsterColorPalette& BasePalette,
        ERarity Rarity);

    /**
     * Adjusts a color palette based on combat traits.
     * Traits like "Fire Affinity" will shift colors towards red/orange.
     * 
     * @param BasePalette The base color palette
     * @param Traits The combat traits
     * @return The adjusted color palette
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette",
        meta = (DisplayName = "Adjust Palette By Traits",
                Keywords = "color palette traits adjust"))
    static FMonsterColorPalette AdjustPaletteByTraits(
        const FMonsterColorPalette& BasePalette,
        const TArray<FCombatTrait>& Traits);

    /**
     * Creates a LUT (Look-Up Table) texture from a color palette.
     * This texture can be used in materials for palette swapping.
     * 
     * @param Palette The color palette
     * @param TextureName The name for the generated texture
     * @return The generated LUT texture (nullptr if failed)
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette",
        meta = (DisplayName = "Create LUT Texture From Palette",
                Keywords = "lut texture palette material"))
    static UTexture2D* CreateLUTTextureFromPalette(
        const FMonsterColorPalette& Palette,
        const FString& TextureName = TEXT("LUT_MonsterPalette"));

    /**
     * Generates a full palette (multiple colors) from a monster color palette.
     * Useful for creating color variations and gradients.
     * 
     * @param MonsterPalette The monster color palette
     * @param NumColors The number of colors to generate
     * @return A full palette with interpolated colors
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette",
        meta = (DisplayName = "Expand Monster Palette",
                Keywords = "color palette expand gradient"))
    static FPalette ExpandMonsterPalette(
        const FMonsterColorPalette& MonsterPalette,
        int32 NumColors = 8);

private:
    /** Helper function to get trait color influence */
    static FLinearColor GetTraitColorInfluence(const FName& TraitID);

    /** Helper function to blend two colors in HSV space */
    static FLinearColor BlendColorsHSV(const FLinearColor& ColorA, const FLinearColor& ColorB, float Alpha);

    /** Helper function to adjust color saturation and value */
    static FLinearColor AdjustColorSaturationValue(const FLinearColor& Color, float SaturationMultiplier, float ValueMultiplier);
};
