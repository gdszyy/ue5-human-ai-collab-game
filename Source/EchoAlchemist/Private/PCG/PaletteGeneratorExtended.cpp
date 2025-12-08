// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/PaletteGeneratorExtended.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Texture2D.h"

// ============================================================================
// Habitat Color Spectrum Definitions
// ============================================================================

FHabitatColorSpectrum UPaletteGeneratorExtended::GetHabitatColorSpectrum(EHabitatType Habitat)
{
    FHabitatColorSpectrum Spectrum;
    Spectrum.Habitat = Habitat;

    switch (Habitat)
    {
    case EHabitatType::Forest:
        // Forest: Green, brown, and earthy tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.2f, 0.5f, 0.2f, 1.0f),  // Dark green
            FLinearColor(0.3f, 0.7f, 0.3f, 1.0f),  // Medium green
            FLinearColor(0.4f, 0.8f, 0.4f, 1.0f)   // Light green
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.4f, 0.3f, 0.2f, 1.0f),  // Brown
            FLinearColor(0.5f, 0.4f, 0.3f, 1.0f),  // Light brown
            FLinearColor(0.3f, 0.5f, 0.3f, 1.0f)   // Olive
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(0.8f, 0.9f, 0.3f, 1.0f),  // Yellow-green
            FLinearColor(0.9f, 0.7f, 0.3f, 1.0f),  // Golden
            FLinearColor(0.7f, 0.9f, 0.5f, 1.0f)   // Lime
        };
        break;

    case EHabitatType::Desert:
        // Desert: Sandy, orange, and warm tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.9f, 0.7f, 0.3f, 1.0f),  // Sand
            FLinearColor(0.8f, 0.6f, 0.2f, 1.0f),  // Dark sand
            FLinearColor(0.95f, 0.8f, 0.4f, 1.0f)  // Light sand
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.7f, 0.4f, 0.2f, 1.0f),  // Terracotta
            FLinearColor(0.8f, 0.5f, 0.3f, 1.0f),  // Clay
            FLinearColor(0.6f, 0.5f, 0.3f, 1.0f)   // Dust
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(1.0f, 0.6f, 0.2f, 1.0f),  // Orange
            FLinearColor(1.0f, 0.8f, 0.4f, 1.0f),  // Gold
            FLinearColor(0.9f, 0.5f, 0.3f, 1.0f)   // Amber
        };
        break;

    case EHabitatType::Tundra:
        // Tundra: Ice blue, white, and cool tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.7f, 0.85f, 0.95f, 1.0f), // Ice blue
            FLinearColor(0.8f, 0.9f, 1.0f, 1.0f),   // Pale blue
            FLinearColor(0.6f, 0.8f, 0.9f, 1.0f)    // Sky blue
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.85f, 0.9f, 0.95f, 1.0f), // Snow white
            FLinearColor(0.7f, 0.8f, 0.85f, 1.0f),  // Frost
            FLinearColor(0.6f, 0.7f, 0.8f, 1.0f)    // Ice gray
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(0.9f, 0.95f, 1.0f, 1.0f),  // Bright white
            FLinearColor(0.7f, 0.9f, 1.0f, 1.0f),   // Cyan
            FLinearColor(0.8f, 0.85f, 1.0f, 1.0f)   // Pale cyan
        };
        break;

    case EHabitatType::Cave:
        // Cave: Dark gray, purple, and mineral tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.3f, 0.3f, 0.35f, 1.0f),  // Dark gray
            FLinearColor(0.4f, 0.35f, 0.4f, 1.0f),  // Stone
            FLinearColor(0.35f, 0.3f, 0.4f, 1.0f)   // Dark purple-gray
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.5f, 0.4f, 0.5f, 1.0f),   // Light gray
            FLinearColor(0.4f, 0.4f, 0.45f, 1.0f),  // Slate
            FLinearColor(0.45f, 0.35f, 0.45f, 1.0f) // Dull purple
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(0.6f, 0.5f, 0.7f, 1.0f),   // Crystal purple
            FLinearColor(0.5f, 0.6f, 0.7f, 1.0f),   // Crystal blue
            FLinearColor(0.7f, 0.6f, 0.5f, 1.0f)    // Mineral orange
        };
        break;

    case EHabitatType::Volcano:
        // Volcano: Red, orange, and fiery tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.8f, 0.2f, 0.1f, 1.0f),   // Lava red
            FLinearColor(0.9f, 0.3f, 0.1f, 1.0f),   // Bright red
            FLinearColor(0.7f, 0.15f, 0.1f, 1.0f)   // Dark red
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.3f, 0.2f, 0.15f, 1.0f),  // Charcoal
            FLinearColor(0.4f, 0.25f, 0.2f, 1.0f),  // Ash
            FLinearColor(0.5f, 0.3f, 0.2f, 1.0f)    // Burnt
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(1.0f, 0.5f, 0.1f, 1.0f),   // Bright orange
            FLinearColor(1.0f, 0.7f, 0.2f, 1.0f),   // Yellow-orange
            FLinearColor(1.0f, 0.3f, 0.1f, 1.0f)    // Fire orange
        };
        break;

    case EHabitatType::Swamp:
        // Swamp: Murky green, brown, and toxic tones
        Spectrum.PrimarySpectrum = {
            FLinearColor(0.3f, 0.4f, 0.2f, 1.0f),   // Murky green
            FLinearColor(0.4f, 0.5f, 0.3f, 1.0f),   // Moss green
            FLinearColor(0.35f, 0.45f, 0.25f, 1.0f) // Swamp green
        };
        Spectrum.SecondarySpectrum = {
            FLinearColor(0.4f, 0.35f, 0.25f, 1.0f), // Mud brown
            FLinearColor(0.5f, 0.4f, 0.3f, 1.0f),   // Wet earth
            FLinearColor(0.35f, 0.4f, 0.3f, 1.0f)   // Dark moss
        };
        Spectrum.AccentSpectrum = {
            FLinearColor(0.6f, 0.8f, 0.3f, 1.0f),   // Toxic green
            FLinearColor(0.7f, 0.7f, 0.2f, 1.0f),   // Sickly yellow
            FLinearColor(0.5f, 0.7f, 0.4f, 1.0f)    // Bright moss
        };
        break;

    default:
        // Fallback to forest
        return GetHabitatColorSpectrum(EHabitatType::Forest);
    }

    return Spectrum;
}

// ============================================================================
// Monster Palette Generation
// ============================================================================

FMonsterColorPalette UPaletteGeneratorExtended::GenerateMonsterPalette(
    const FEcologyAttributes& EcologyAttributes,
    const FCombatAttributes& CombatAttributes,
    int32 Seed)
{
    // Initialize random stream with seed
    FRandomStream RandomStream(Seed);

    // Get habitat color spectrum
    FHabitatColorSpectrum Spectrum = GetHabitatColorSpectrum(EcologyAttributes.Habitat);

    // Select colors from spectrum based on random values
    FMonsterColorPalette Palette;

    // Select primary color
    if (Spectrum.PrimarySpectrum.Num() > 0)
    {
        int32 Index = RandomStream.RandRange(0, Spectrum.PrimarySpectrum.Num() - 1);
        float LerpAlpha = RandomStream.FRand();
        int32 NextIndex = (Index + 1) % Spectrum.PrimarySpectrum.Num();
        Palette.PrimaryColor = FLinearColor::LerpUsingHSV(
            Spectrum.PrimarySpectrum[Index],
            Spectrum.PrimarySpectrum[NextIndex],
            LerpAlpha);
    }

    // Select secondary color
    if (Spectrum.SecondarySpectrum.Num() > 0)
    {
        int32 Index = RandomStream.RandRange(0, Spectrum.SecondarySpectrum.Num() - 1);
        float LerpAlpha = RandomStream.FRand();
        int32 NextIndex = (Index + 1) % Spectrum.SecondarySpectrum.Num();
        Palette.SecondaryColor = FLinearColor::LerpUsingHSV(
            Spectrum.SecondarySpectrum[Index],
            Spectrum.SecondarySpectrum[NextIndex],
            LerpAlpha);
    }

    // Select accent color
    if (Spectrum.AccentSpectrum.Num() > 0)
    {
        int32 Index = RandomStream.RandRange(0, Spectrum.AccentSpectrum.Num() - 1);
        float LerpAlpha = RandomStream.FRand();
        int32 NextIndex = (Index + 1) % Spectrum.AccentSpectrum.Num();
        Palette.AccentColor = FLinearColor::LerpUsingHSV(
            Spectrum.AccentSpectrum[Index],
            Spectrum.AccentSpectrum[NextIndex],
            LerpAlpha);
    }

    // Adjust by rarity (get highest rarity from traits)
    ERarity HighestRarity = ERarity::Common;
    for (const FCombatTrait& Trait : CombatAttributes.Traits)
    {
        if (static_cast<uint8>(Trait.Rarity) > static_cast<uint8>(HighestRarity))
        {
            HighestRarity = Trait.Rarity;
        }
    }
    Palette = AdjustPaletteByRarity(Palette, HighestRarity);

    // Adjust by combat traits
    Palette = AdjustPaletteByTraits(Palette, CombatAttributes.Traits);

    return Palette;
}

// ============================================================================
// Palette Adjustment Functions
// ============================================================================

FMonsterColorPalette UPaletteGeneratorExtended::AdjustPaletteByRarity(
    const FMonsterColorPalette& BasePalette,
    ERarity Rarity)
{
    FMonsterColorPalette AdjustedPalette = BasePalette;

    float SaturationMultiplier = 1.0f;
    float ValueMultiplier = 1.0f;

    switch (Rarity)
    {
    case ERarity::Common:
        // No adjustment for common
        SaturationMultiplier = 1.0f;
        ValueMultiplier = 1.0f;
        break;

    case ERarity::Rare:
        // Rare: More saturated and brighter
        SaturationMultiplier = 1.2f;
        ValueMultiplier = 1.1f;
        break;

    case ERarity::Epic:
        // Epic: Highly saturated and bright
        SaturationMultiplier = 1.4f;
        ValueMultiplier = 1.2f;
        break;
    }

    // Apply adjustments
    AdjustedPalette.PrimaryColor = AdjustColorSaturationValue(
        BasePalette.PrimaryColor, SaturationMultiplier, ValueMultiplier);
    AdjustedPalette.SecondaryColor = AdjustColorSaturationValue(
        BasePalette.SecondaryColor, SaturationMultiplier, ValueMultiplier);
    AdjustedPalette.AccentColor = AdjustColorSaturationValue(
        BasePalette.AccentColor, SaturationMultiplier, ValueMultiplier);

    return AdjustedPalette;
}

FMonsterColorPalette UPaletteGeneratorExtended::AdjustPaletteByTraits(
    const FMonsterColorPalette& BasePalette,
    const TArray<FCombatTrait>& Traits)
{
    FMonsterColorPalette AdjustedPalette = BasePalette;

    // Blend trait color influences
    for (const FCombatTrait& Trait : Traits)
    {
        FLinearColor TraitInfluence = GetTraitColorInfluence(Trait.TraitID);
        
        // Blend trait influence into accent color (most visible)
        AdjustedPalette.AccentColor = BlendColorsHSV(
            AdjustedPalette.AccentColor,
            TraitInfluence,
            0.3f); // 30% influence

        // Slightly blend into primary color
        AdjustedPalette.PrimaryColor = BlendColorsHSV(
            AdjustedPalette.PrimaryColor,
            TraitInfluence,
            0.15f); // 15% influence
    }

    return AdjustedPalette;
}

// ============================================================================
// LUT Texture Generation
// ============================================================================

UTexture2D* UPaletteGeneratorExtended::CreateLUTTextureFromPalette(
    const FMonsterColorPalette& Palette,
    const FString& TextureName)
{
    // Create a simple 3x1 LUT texture with primary, secondary, and accent colors
    // In a real implementation, this would create a proper LUT texture
    // For now, we return nullptr as texture creation requires more complex UE4 API usage
    
    // Note: This function is a placeholder. In a production environment,
    // you would use UTexture2D::CreateTransient() and fill pixel data.
    // However, this requires platform-specific code and is better handled
    // in the editor or through blueprints.
    
    return nullptr;
}

// ============================================================================
// Palette Expansion
// ============================================================================

FPalette UPaletteGeneratorExtended::ExpandMonsterPalette(
    const FMonsterColorPalette& MonsterPalette,
    int32 NumColors)
{
    FPalette ExpandedPalette;

    if (NumColors <= 0)
    {
        return ExpandedPalette;
    }

    // Create a spectrum from the three base colors
    TArray<FLinearColor> Spectrum;
    Spectrum.Add(MonsterPalette.PrimaryColor);
    Spectrum.Add(MonsterPalette.SecondaryColor);
    Spectrum.Add(MonsterPalette.AccentColor);

    // Use the base palette generator to expand
    ExpandedPalette = UPaletteGenerator::GeneratePaletteFromSpectrum(Spectrum, NumColors);

    return ExpandedPalette;
}

// ============================================================================
// Helper Functions
// ============================================================================

FLinearColor UPaletteGeneratorExtended::GetTraitColorInfluence(const FName& TraitID)
{
    // Define color influences for common trait types
    FString TraitString = TraitID.ToString();

    if (TraitString.Contains(TEXT("Fire")) || TraitString.Contains(TEXT("Flame")))
    {
        return FLinearColor(1.0f, 0.3f, 0.1f, 1.0f); // Red-orange
    }
    else if (TraitString.Contains(TEXT("Ice")) || TraitString.Contains(TEXT("Frost")))
    {
        return FLinearColor(0.5f, 0.8f, 1.0f, 1.0f); // Ice blue
    }
    else if (TraitString.Contains(TEXT("Poison")) || TraitString.Contains(TEXT("Toxic")))
    {
        return FLinearColor(0.5f, 0.8f, 0.3f, 1.0f); // Toxic green
    }
    else if (TraitString.Contains(TEXT("Stone")) || TraitString.Contains(TEXT("Rock")))
    {
        return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f); // Gray
    }
    else if (TraitString.Contains(TEXT("Lightning")) || TraitString.Contains(TEXT("Thunder")))
    {
        return FLinearColor(0.8f, 0.8f, 1.0f, 1.0f); // Electric blue
    }
    else if (TraitString.Contains(TEXT("Shadow")) || TraitString.Contains(TEXT("Dark")))
    {
        return FLinearColor(0.2f, 0.1f, 0.3f, 1.0f); // Dark purple
    }
    else if (TraitString.Contains(TEXT("Light")) || TraitString.Contains(TEXT("Holy")))
    {
        return FLinearColor(1.0f, 1.0f, 0.8f, 1.0f); // Bright yellow
    }

    // Default: no influence (return base color)
    return FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

FLinearColor UPaletteGeneratorExtended::BlendColorsHSV(
    const FLinearColor& ColorA,
    const FLinearColor& ColorB,
    float Alpha)
{
    return FLinearColor::LerpUsingHSV(ColorA, ColorB, Alpha);
}

FLinearColor UPaletteGeneratorExtended::AdjustColorSaturationValue(
    const FLinearColor& Color,
    float SaturationMultiplier,
    float ValueMultiplier)
{
    // Convert to HSV
    FLinearColor HSV = Color.LinearRGBToHSV();

    // Adjust saturation and value
    HSV.G = FMath::Clamp(HSV.G * SaturationMultiplier, 0.0f, 1.0f); // Saturation
    HSV.B = FMath::Clamp(HSV.B * ValueMultiplier, 0.0f, 1.0f);      // Value

    // Convert back to RGB
    return HSV.HSVToLinearRGB();
}
