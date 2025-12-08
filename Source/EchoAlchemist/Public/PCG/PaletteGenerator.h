// Copyright Echo Alchemist. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCG/MonsterAttributes.h"
#include "PaletteGenerator.generated.h"

/**
 * @struct FPalette
 * @brief Represents a color palette containing an array of colors.
 * 
 * A palette consists of primary, secondary, and highlight colors that define
 * the visual appearance of a procedurally generated monster.
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FPalette
{
    GENERATED_BODY()

    /** Primary color - the dominant color of the monster */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor PrimaryColor;

    /** Secondary color - complementary color for details */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor SecondaryColor;

    /** Highlight color - accent color for special features */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor HighlightColor;

    /** Shadow color - darker shade for depth */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    FLinearColor ShadowColor;

    /** All colors in the palette (for compatibility with existing code) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Palette")
    TArray<FLinearColor> Colors;

    /** Default constructor */
    FPalette()
        : PrimaryColor(FLinearColor::White)
        , SecondaryColor(FLinearColor::Gray)
        , HighlightColor(FLinearColor::White)
        , ShadowColor(FLinearColor::Black)
    {
    }
};

/**
 * @struct FHabitatColorSpectrum
 * @brief Defines the base color spectrum for a specific habitat type.
 * 
 * Each habitat has a characteristic color range that reflects its environment.
 */
USTRUCT(BlueprintType)
struct ECHOALCHEMIST_API FHabitatColorSpectrum
{
    GENERATED_BODY()

    /** Base hue range (0-360 degrees in HSV color space) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habitat")
    FVector2D HueRange;

    /** Base saturation range (0-1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habitat")
    FVector2D SaturationRange;

    /** Base value/brightness range (0-1) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Habitat")
    FVector2D ValueRange;

    /** Default constructor */
    FHabitatColorSpectrum()
        : HueRange(FVector2D(0.0f, 360.0f))
        , SaturationRange(FVector2D(0.3f, 0.8f))
        , ValueRange(FVector2D(0.4f, 0.9f))
    {
    }

    /** Constructor with parameters */
    FHabitatColorSpectrum(FVector2D InHueRange, FVector2D InSaturationRange, FVector2D InValueRange)
        : HueRange(InHueRange)
        , SaturationRange(InSaturationRange)
        , ValueRange(InValueRange)
    {
    }
};

/**
 * @class UPaletteGenerator
 * @brief A Blueprint Function Library for generating color palettes procedurally.
 * 
 * This class provides algorithm-driven palette generation based on monster attributes,
 * including ecology attributes (habitat, size) and combat attributes (rarity, traits).
 * 
 * **Design Principles:**
 * - Habitat determines the base color spectrum
 * - Rarity affects saturation and brightness
 * - Combat traits add accent colors
 * - All generation is deterministic (seed-based)
 * 
 * **Blueprint Usage Example:**
 * ```
 * // Generate palette from monster attributes
 * Palette = Generate Palette From Monster Attributes(Monster Attributes, Seed: 12345)
 * 
 * // Generate palette from ecology attributes only
 * Palette = Generate Palette From Ecology(Ecology Attributes, Seed: 67890)
 * 
 * // Get habitat color spectrum
 * Spectrum = Get Habitat Color Spectrum(Habitat: Forest)
 * ```
 */
UCLASS()
class ECHOALCHEMIST_API UPaletteGenerator : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    // ========================================
    // Algorithm-Driven Palette Generation
    // ========================================

    /**
     * Generates a complete color palette from monster attributes.
     * 
     * This is the main entry point for palette generation. It combines ecology attributes
     * (habitat, size, tolerance) with combat attributes (rarity, traits) to create a
     * harmonious and meaningful color scheme.
     * 
     * @param MonsterAttributes Complete monster attributes (ecology + combat).
     * @param Seed Random seed for deterministic generation.
     * @return Generated color palette with primary, secondary, highlight, and shadow colors.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate palette for a forest monster with fire affinity
     * Monster Attributes = ... // (Habitat: Forest, Rarity: Rare, Trait: FireAffinity)
     * Palette = Generate Palette From Monster Attributes(Monster Attributes, Seed: 12345)
     * // Result: Green primary (forest), red-orange highlight (fire), darker shadow
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Palette From Monster Attributes", Keywords = "monster palette generator attributes"))
    static FPalette GeneratePaletteFromMonsterAttributes(const FMonsterAttributes& MonsterAttributes, int32 Seed);

    /**
     * Generates a color palette from ecology attributes only.
     * 
     * This function generates a palette based solely on the monster's ecological characteristics,
     * without considering combat traits. Useful for generating base palettes that can be
     * modified later based on combat attributes.
     * 
     * @param EcologyAttributes Ecology attributes (habitat, size, tolerance, etc.).
     * @param Seed Random seed for deterministic generation.
     * @return Generated color palette based on ecology.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate palette for a large desert creature
     * Ecology Attributes = ... // (Habitat: Desert, Size: Large, Tolerance: 75)
     * Palette = Generate Palette From Ecology(Ecology Attributes, Seed: 67890)
     * // Result: Sandy yellow-orange tones with high brightness
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Palette From Ecology", Keywords = "ecology palette generator"))
    static FPalette GeneratePaletteFromEcology(const FEcologyAttributes& EcologyAttributes, int32 Seed);

    /**
     * Modifies a base palette based on combat attributes.
     * 
     * This function takes an existing palette and modifies it based on rarity and combat traits.
     * Rarity affects saturation and brightness, while traits add accent colors.
     * 
     * @param BasePalette The base palette to modify.
     * @param CombatAttributes Combat attributes (rarity, traits).
     * @param Seed Random seed for deterministic generation.
     * @return Modified palette with combat-based adjustments.
     * 
     * **Blueprint Usage:**
     * ```
     * // Add fire affinity to a forest monster's palette
     * Base Palette = ... // (Green forest colors)
     * Combat Attributes = ... // (Rarity: Epic, Trait: FireAffinity)
     * Modified Palette = Modify Palette With Combat Attributes(Base Palette, Combat Attributes, Seed: 11111)
     * // Result: Vibrant green with fiery red-orange highlights
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Modify Palette With Combat Attributes", Keywords = "combat palette modifier"))
    static FPalette ModifyPaletteWithCombatAttributes(const FPalette& BasePalette, const FCombatAttributes& CombatAttributes, int32 Seed);

    // ========================================
    // Habitat Color Spectrum
    // ========================================

    /**
     * Gets the base color spectrum for a specific habitat type.
     * 
     * Each habitat has a predefined color spectrum that reflects its environment:
     * - Forest: Green and brown tones
     * - Desert: Sandy yellow and orange tones
     * - Tundra: Cool blue and white tones
     * - Cave: Dark gray and purple tones
     * - Volcano: Red and orange tones with high saturation
     * - Swamp: Murky green and brown tones with low brightness
     * 
     * @param Habitat The habitat type.
     * @return The color spectrum for the habitat.
     * 
     * **Blueprint Usage:**
     * ```
     * // Get the color spectrum for a forest habitat
     * Spectrum = Get Habitat Color Spectrum(Habitat: Forest)
     * // Result: Hue Range (90-150), Saturation (0.4-0.7), Value (0.3-0.8)
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Get Habitat Color Spectrum", Keywords = "habitat spectrum color"))
    static FHabitatColorSpectrum GetHabitatColorSpectrum(EHabitatType Habitat);

    /**
     * Generates a random color within a habitat's color spectrum.
     * 
     * @param Habitat The habitat type.
     * @param Seed Random seed for deterministic generation.
     * @return A random color within the habitat's spectrum.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate a random forest color
     * Color = Generate Color From Habitat(Habitat: Forest, Seed: 12345)
     * // Result: A green or brown color typical of forests
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Color From Habitat", Keywords = "habitat color generator"))
    static FLinearColor GenerateColorFromHabitat(EHabitatType Habitat, int32 Seed);

    // ========================================
    // Rarity and Combat Trait Color Mapping
    // ========================================

    /**
     * Adjusts a color based on rarity level.
     * 
     * Rarity affects the visual impact of a color:
     * - Common: Normal saturation and brightness
     * - Rare: Increased saturation (+20%)
     * - Epic: High saturation (+40%) and brightness (+20%)
     * 
     * @param BaseColor The base color to adjust.
     * @param Rarity The rarity level.
     * @return Adjusted color based on rarity.
     * 
     * **Blueprint Usage:**
     * ```
     * // Make a color more vibrant for an epic monster
     * Base Color = ... // (Normal green)
     * Adjusted Color = Adjust Color By Rarity(Base Color, Rarity: Epic)
     * // Result: Vibrant, saturated green
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Adjust Color By Rarity", Keywords = "rarity color adjust"))
    static FLinearColor AdjustColorByRarity(const FLinearColor& BaseColor, ERarity Rarity);

    /**
     * Gets an accent color for a specific combat trait.
     * 
     * Each combat trait has an associated accent color that reflects its nature:
     * - Fire traits: Red-orange
     * - Ice traits: Cyan-blue
     * - Poison traits: Toxic green
     * - Stone traits: Gray-brown
     * - Lightning traits: Bright yellow
     * - etc.
     * 
     * @param TraitID The trait identifier.
     * @return The accent color for the trait, or white if not found.
     * 
     * **Blueprint Usage:**
     * ```
     * // Get the accent color for fire affinity
     * Accent Color = Get Trait Accent Color(Trait ID: "FireAffinity")
     * // Result: Red-orange color
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Get Trait Accent Color", Keywords = "trait accent color"))
    static FLinearColor GetTraitAccentColor(FName TraitID);

    // ========================================
    // LUT Texture Generation (Advanced)
    // ========================================

    /**
     * Generates a 1D LUT (Look-Up Table) texture from a palette.
     * 
     * A LUT texture can be used in materials to remap colors. This function generates
     * a 1D texture where each pixel represents a color in the palette, with smooth
     * gradients between them.
     * 
     * @param Palette The palette to convert to a LUT.
     * @param TextureSize The size of the LUT texture (e.g., 256).
     * @return A 2D texture object that can be used in materials.
     * 
     * **Note:** This function creates a UTexture2D at runtime. For performance reasons,
     * consider pre-generating LUTs during development and loading them as assets.
     * 
     * **Blueprint Usage:**
     * ```
     * // Generate a LUT texture from a palette
     * Palette = ... // (Generated palette)
     * LUT Texture = Generate LUT Texture From Palette(Palette, Texture Size: 256)
     * // Result: A 256x1 texture with smooth color gradients
     * ```
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate LUT Texture From Palette", Keywords = "lut texture palette"))
    static UTexture2D* GenerateLUTTextureFromPalette(const FPalette& Palette, int32 TextureSize = 256);

    // ========================================
    // Legacy Functions (Compatibility)
    // ========================================

    /**
     * Generates a color palette from a given spectrum of colors.
     * 
     * **Legacy function** - kept for backward compatibility with existing code.
     * Consider using GeneratePaletteFromMonsterAttributes for new implementations.
     * 
     * @param Spectrum The base colors to generate the palette from.
     * @param NumColors The number of colors to generate in the final palette.
     * @return A new FPalette struct containing the generated colors.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Palette From Spectrum", Keywords = "color palette generator spectrum"))
    static FPalette GeneratePaletteFromSpectrum(const TArray<FLinearColor>& Spectrum, int32 NumColors);

    /**
     * Generates a monochromatic palette from a base color.
     * 
     * **Legacy function** - kept for backward compatibility with existing code.
     * Consider using GeneratePaletteFromEcology for new implementations.
     * 
     * @param BaseColor The base color for the palette.
     * @param NumColors The number of colors to generate.
     * @param SaturationRange The range of saturation values to use.
     * @param ValueRange The range of value (brightness) values to use.
     * @return A new FPalette struct containing the generated monochromatic colors.
     */
    UFUNCTION(BlueprintCallable, Category = "PCG | Palette", meta = (DisplayName = "Generate Monochromatic Palette", Keywords = "color palette generator mono"))
    static FPalette GenerateMonochromaticPalette(FLinearColor BaseColor, int32 NumColors, FVector2D SaturationRange = FVector2D(0.2f, 1.0f), FVector2D ValueRange = FVector2D(0.3f, 1.0f));

private:

    /** Helper function to get a seeded random stream */
    static FRandomStream GetSeededRandomStream(int32 Seed);

    /** Helper function to convert HSV to Linear RGB (UE4 uses 0-360 for H, 0-1 for S and V) */
    static FLinearColor HSVToLinearRGB(float H, float S, float V);

    /** Helper function to convert Linear RGB to HSV */
    static void LinearRGBToHSV(const FLinearColor& RGB, float& OutH, float& OutS, float& OutV);

    /** Helper function to blend two colors in HSV space */
    static FLinearColor BlendColorsHSV(const FLinearColor& ColorA, const FLinearColor& ColorB, float Alpha);

    /** Helper function to generate a complementary color */
    static FLinearColor GenerateComplementaryColor(const FLinearColor& BaseColor);

    /** Helper function to generate a shadow color (darker variant) */
    static FLinearColor GenerateShadowColor(const FLinearColor& BaseColor);
};
