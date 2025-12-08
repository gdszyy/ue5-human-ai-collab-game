// Copyright Echo Alchemist. All Rights Reserved.

#include "PCG/PaletteGenerator.h"
#include "Engine/Texture2D.h"

// ========================================
// Algorithm-Driven Palette Generation
// ========================================

FPalette UPaletteGenerator::GeneratePaletteFromMonsterAttributes(const FMonsterAttributes& MonsterAttributes, int32 Seed)
{
    // Step 1: Generate base palette from ecology attributes
    FPalette BasePalette = GeneratePaletteFromEcology(MonsterAttributes.EcologyAttributes, Seed);

    // Step 2: Modify palette based on combat attributes
    FPalette FinalPalette = ModifyPaletteWithCombatAttributes(BasePalette, MonsterAttributes.CombatAttributes, Seed + 1000);

    return FinalPalette;
}

FPalette UPaletteGenerator::GeneratePaletteFromEcology(const FEcologyAttributes& EcologyAttributes, int32 Seed)
{
    FPalette Palette;
    FRandomStream RandomStream = GetSeededRandomStream(Seed);

    // Get habitat color spectrum
    FHabitatColorSpectrum Spectrum = GetHabitatColorSpectrum(EcologyAttributes.Habitat);

    // Generate primary color from habitat spectrum
    float PrimaryHue = FMath::Lerp(Spectrum.HueRange.X, Spectrum.HueRange.Y, RandomStream.FRand());
    float PrimarySaturation = FMath::Lerp(Spectrum.SaturationRange.X, Spectrum.SaturationRange.Y, RandomStream.FRand());
    float PrimaryValue = FMath::Lerp(Spectrum.ValueRange.X, Spectrum.ValueRange.Y, RandomStream.FRand());

    // Adjust based on tolerance (higher tolerance = more saturated colors)
    PrimarySaturation = FMath::Clamp(PrimarySaturation + (EcologyAttributes.Tolerance - 50.0f) * 0.002f, 0.0f, 1.0f);

    // Adjust based on size class (larger creatures = darker colors)
    float SizeModifier = 0.0f;
    switch (EcologyAttributes.SizeClass)
    {
        case ESizeClass::Tiny:   SizeModifier = 0.2f; break;
        case ESizeClass::Small:  SizeModifier = 0.1f; break;
        case ESizeClass::Medium: SizeModifier = 0.0f; break;
        case ESizeClass::Large:  SizeModifier = -0.1f; break;
        case ESizeClass::Giant:  SizeModifier = -0.2f; break;
    }
    PrimaryValue = FMath::Clamp(PrimaryValue + SizeModifier, 0.0f, 1.0f);

    Palette.PrimaryColor = HSVToLinearRGB(PrimaryHue, PrimarySaturation, PrimaryValue);

    // Generate secondary color (complementary or analogous)
    float SecondaryHue = PrimaryHue + (RandomStream.FRand() > 0.5f ? 30.0f : -30.0f); // Analogous
    if (SecondaryHue < 0.0f) SecondaryHue += 360.0f;
    if (SecondaryHue >= 360.0f) SecondaryHue -= 360.0f;
    
    float SecondarySaturation = PrimarySaturation * 0.7f;
    float SecondaryValue = PrimaryValue * 0.9f;
    Palette.SecondaryColor = HSVToLinearRGB(SecondaryHue, SecondarySaturation, SecondaryValue);

    // Generate highlight color (lighter variant of primary)
    Palette.HighlightColor = HSVToLinearRGB(PrimaryHue, PrimarySaturation * 0.6f, FMath::Min(PrimaryValue + 0.3f, 1.0f));

    // Generate shadow color (darker variant of primary)
    Palette.ShadowColor = GenerateShadowColor(Palette.PrimaryColor);

    // Populate Colors array for compatibility
    Palette.Colors.Empty();
    Palette.Colors.Add(Palette.PrimaryColor);
    Palette.Colors.Add(Palette.SecondaryColor);
    Palette.Colors.Add(Palette.HighlightColor);
    Palette.Colors.Add(Palette.ShadowColor);

    return Palette;
}

FPalette UPaletteGenerator::ModifyPaletteWithCombatAttributes(const FPalette& BasePalette, const FCombatAttributes& CombatAttributes, int32 Seed)
{
    FPalette ModifiedPalette = BasePalette;
    FRandomStream RandomStream = GetSeededRandomStream(Seed);

    // If no traits, just adjust by rarity and return
    if (CombatAttributes.Traits.Num() == 0)
    {
        ModifiedPalette.PrimaryColor = AdjustColorByRarity(ModifiedPalette.PrimaryColor, ERarity::Common);
        ModifiedPalette.SecondaryColor = AdjustColorByRarity(ModifiedPalette.SecondaryColor, ERarity::Common);
        ModifiedPalette.HighlightColor = AdjustColorByRarity(ModifiedPalette.HighlightColor, ERarity::Common);
        return ModifiedPalette;
    }

    // Get the highest rarity among all traits
    ERarity HighestRarity = ERarity::Common;
    for (const FCombatTrait& Trait : CombatAttributes.Traits)
    {
        if (static_cast<uint8>(Trait.Rarity) > static_cast<uint8>(HighestRarity))
        {
            HighestRarity = Trait.Rarity;
        }
    }

    // Adjust colors by rarity
    ModifiedPalette.PrimaryColor = AdjustColorByRarity(ModifiedPalette.PrimaryColor, HighestRarity);
    ModifiedPalette.SecondaryColor = AdjustColorByRarity(ModifiedPalette.SecondaryColor, HighestRarity);

    // Blend trait accent colors into highlight color
    FLinearColor AccentBlend = FLinearColor::Black;
    float AccentWeight = 0.0f;

    for (const FCombatTrait& Trait : CombatAttributes.Traits)
    {
        FLinearColor TraitAccent = GetTraitAccentColor(Trait.TraitID);
        float Weight = 1.0f / CombatAttributes.Traits.Num();
        AccentBlend += TraitAccent * Weight;
        AccentWeight += Weight;
    }

    if (AccentWeight > 0.0f)
    {
        AccentBlend = AccentBlend / AccentWeight;
        // Blend 60% base highlight + 40% trait accent
        ModifiedPalette.HighlightColor = BlendColorsHSV(ModifiedPalette.HighlightColor, AccentBlend, 0.4f);
        ModifiedPalette.HighlightColor = AdjustColorByRarity(ModifiedPalette.HighlightColor, HighestRarity);
    }

    // Update Colors array
    ModifiedPalette.Colors.Empty();
    ModifiedPalette.Colors.Add(ModifiedPalette.PrimaryColor);
    ModifiedPalette.Colors.Add(ModifiedPalette.SecondaryColor);
    ModifiedPalette.Colors.Add(ModifiedPalette.HighlightColor);
    ModifiedPalette.Colors.Add(ModifiedPalette.ShadowColor);

    return ModifiedPalette;
}

// ========================================
// Habitat Color Spectrum
// ========================================

FHabitatColorSpectrum UPaletteGenerator::GetHabitatColorSpectrum(EHabitatType Habitat)
{
    switch (Habitat)
    {
        case EHabitatType::Forest:
            // Green and brown tones
            return FHabitatColorSpectrum(
                FVector2D(90.0f, 150.0f),   // Hue: Yellow-green to cyan-green
                FVector2D(0.4f, 0.7f),      // Saturation: Moderate
                FVector2D(0.3f, 0.8f)       // Value: Medium to bright
            );

        case EHabitatType::Desert:
            // Sandy yellow and orange tones
            return FHabitatColorSpectrum(
                FVector2D(30.0f, 60.0f),    // Hue: Orange to yellow
                FVector2D(0.3f, 0.6f),      // Saturation: Low to moderate
                FVector2D(0.6f, 0.95f)      // Value: Bright
            );

        case EHabitatType::Tundra:
            // Cool blue and white tones
            return FHabitatColorSpectrum(
                FVector2D(180.0f, 240.0f),  // Hue: Cyan to blue
                FVector2D(0.2f, 0.5f),      // Saturation: Low
                FVector2D(0.7f, 1.0f)       // Value: Very bright
            );

        case EHabitatType::Cave:
            // Dark gray and purple tones
            return FHabitatColorSpectrum(
                FVector2D(260.0f, 300.0f),  // Hue: Blue-purple to purple
                FVector2D(0.2f, 0.5f),      // Saturation: Low to moderate
                FVector2D(0.2f, 0.5f)       // Value: Dark
            );

        case EHabitatType::Volcano:
            // Red and orange tones with high saturation
            return FHabitatColorSpectrum(
                FVector2D(0.0f, 30.0f),     // Hue: Red to orange
                FVector2D(0.7f, 1.0f),      // Saturation: High
                FVector2D(0.5f, 0.9f)       // Value: Medium to bright
            );

        case EHabitatType::Swamp:
            // Murky green and brown tones with low brightness
            return FHabitatColorSpectrum(
                FVector2D(60.0f, 120.0f),   // Hue: Yellow-green to green
                FVector2D(0.3f, 0.6f),      // Saturation: Low to moderate
                FVector2D(0.2f, 0.5f)       // Value: Dark
            );

        default:
            // Default to forest
            return GetHabitatColorSpectrum(EHabitatType::Forest);
    }
}

FLinearColor UPaletteGenerator::GenerateColorFromHabitat(EHabitatType Habitat, int32 Seed)
{
    FRandomStream RandomStream = GetSeededRandomStream(Seed);
    FHabitatColorSpectrum Spectrum = GetHabitatColorSpectrum(Habitat);

    float Hue = FMath::Lerp(Spectrum.HueRange.X, Spectrum.HueRange.Y, RandomStream.FRand());
    float Saturation = FMath::Lerp(Spectrum.SaturationRange.X, Spectrum.SaturationRange.Y, RandomStream.FRand());
    float Value = FMath::Lerp(Spectrum.ValueRange.X, Spectrum.ValueRange.Y, RandomStream.FRand());

    return HSVToLinearRGB(Hue, Saturation, Value);
}

// ========================================
// Rarity and Combat Trait Color Mapping
// ========================================

FLinearColor UPaletteGenerator::AdjustColorByRarity(const FLinearColor& BaseColor, ERarity Rarity)
{
    float H, S, V;
    LinearRGBToHSV(BaseColor, H, S, V);

    switch (Rarity)
    {
        case ERarity::Common:
            // No adjustment
            break;

        case ERarity::Rare:
            // Increase saturation by 20%
            S = FMath::Clamp(S + 0.2f, 0.0f, 1.0f);
            break;

        case ERarity::Epic:
            // Increase saturation by 40% and brightness by 20%
            S = FMath::Clamp(S + 0.4f, 0.0f, 1.0f);
            V = FMath::Clamp(V + 0.2f, 0.0f, 1.0f);
            break;
    }

    return HSVToLinearRGB(H, S, V);
}

FLinearColor UPaletteGenerator::GetTraitAccentColor(FName TraitID)
{
    // Define accent colors for common combat traits
    // These colors are in RGB space for simplicity
    
    FString TraitIDString = TraitID.ToString().ToLower();

    // Fire-related traits
    if (TraitIDString.Contains(TEXT("fire")) || TraitIDString.Contains(TEXT("flame")) || TraitIDString.Contains(TEXT("burn")))
    {
        return FLinearColor(1.0f, 0.3f, 0.0f); // Red-orange
    }

    // Ice-related traits
    if (TraitIDString.Contains(TEXT("ice")) || TraitIDString.Contains(TEXT("frost")) || TraitIDString.Contains(TEXT("cold")))
    {
        return FLinearColor(0.0f, 0.8f, 1.0f); // Cyan-blue
    }

    // Poison-related traits
    if (TraitIDString.Contains(TEXT("poison")) || TraitIDString.Contains(TEXT("toxic")) || TraitIDString.Contains(TEXT("venom")))
    {
        return FLinearColor(0.3f, 1.0f, 0.2f); // Toxic green
    }

    // Stone/Earth-related traits
    if (TraitIDString.Contains(TEXT("stone")) || TraitIDString.Contains(TEXT("rock")) || TraitIDString.Contains(TEXT("earth")))
    {
        return FLinearColor(0.5f, 0.4f, 0.3f); // Gray-brown
    }

    // Lightning-related traits
    if (TraitIDString.Contains(TEXT("lightning")) || TraitIDString.Contains(TEXT("electric")) || TraitIDString.Contains(TEXT("thunder")))
    {
        return FLinearColor(1.0f, 1.0f, 0.2f); // Bright yellow
    }

    // Shadow/Dark-related traits
    if (TraitIDString.Contains(TEXT("shadow")) || TraitIDString.Contains(TEXT("dark")) || TraitIDString.Contains(TEXT("void")))
    {
        return FLinearColor(0.2f, 0.0f, 0.3f); // Dark purple
    }

    // Light/Holy-related traits
    if (TraitIDString.Contains(TEXT("light")) || TraitIDString.Contains(TEXT("holy")) || TraitIDString.Contains(TEXT("divine")))
    {
        return FLinearColor(1.0f, 1.0f, 0.8f); // Bright white-yellow
    }

    // Blood/Life-related traits
    if (TraitIDString.Contains(TEXT("blood")) || TraitIDString.Contains(TEXT("life")) || TraitIDString.Contains(TEXT("regenerate")))
    {
        return FLinearColor(0.8f, 0.0f, 0.2f); // Deep red
    }

    // Wind/Air-related traits
    if (TraitIDString.Contains(TEXT("wind")) || TraitIDString.Contains(TEXT("air")) || TraitIDString.Contains(TEXT("storm")))
    {
        return FLinearColor(0.7f, 0.9f, 1.0f); // Light blue
    }

    // Default: White (no specific accent)
    return FLinearColor::White;
}

// ========================================
// LUT Texture Generation
// ========================================

UTexture2D* UPaletteGenerator::GenerateLUTTextureFromPalette(const FPalette& Palette, int32 TextureSize)
{
    if (TextureSize <= 0)
    {
        return nullptr;
    }

    // Create a new texture
    UTexture2D* LUTTexture = UTexture2D::CreateTransient(TextureSize, 1, PF_B8G8R8A8);
    if (!LUTTexture)
    {
        return nullptr;
    }

    // Lock the texture for editing
    FTexture2DMipMap& Mip = LUTTexture->PlatformData->Mips[0];
    void* Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
    uint8* PixelData = static_cast<uint8*>(Data);

    // Generate gradient from palette colors
    TArray<FLinearColor> PaletteColors;
    PaletteColors.Add(Palette.ShadowColor);
    PaletteColors.Add(Palette.PrimaryColor);
    PaletteColors.Add(Palette.SecondaryColor);
    PaletteColors.Add(Palette.HighlightColor);

    for (int32 X = 0; X < TextureSize; ++X)
    {
        // Calculate position in palette (0-1)
        float Alpha = static_cast<float>(X) / static_cast<float>(TextureSize - 1);
        
        // Map to palette color segments
        float SegmentFloat = Alpha * (PaletteColors.Num() - 1);
        int32 SegmentIndex = FMath::FloorToInt(SegmentFloat);
        float SegmentAlpha = FMath::Frac(SegmentFloat);

        // Clamp segment index
        SegmentIndex = FMath::Clamp(SegmentIndex, 0, PaletteColors.Num() - 2);

        // Interpolate between two palette colors
        FLinearColor Color = FLinearColor::LerpUsingHSV(
            PaletteColors[SegmentIndex],
            PaletteColors[SegmentIndex + 1],
            SegmentAlpha
        );

        // Convert to 8-bit RGBA
        FColor Color8Bit = Color.ToFColor(false);

        // Write pixel (BGRA format)
        int32 PixelIndex = X * 4;
        PixelData[PixelIndex + 0] = Color8Bit.B;
        PixelData[PixelIndex + 1] = Color8Bit.G;
        PixelData[PixelIndex + 2] = Color8Bit.R;
        PixelData[PixelIndex + 3] = Color8Bit.A;
    }

    // Unlock and update texture
    Mip.BulkData.Unlock();
    LUTTexture->UpdateResource();

    return LUTTexture;
}

// ========================================
// Legacy Functions
// ========================================

FPalette UPaletteGenerator::GeneratePaletteFromSpectrum(const TArray<FLinearColor>& Spectrum, int32 NumColors)
{
    FPalette Palette;
    if (Spectrum.Num() == 0 || NumColors <= 0)
    {
        return Palette;
    }

    for (int32 i = 0; i < NumColors; ++i)
    {
        float Alpha = (NumColors == 1) ? 0.0f : static_cast<float>(i) / static_cast<float>(NumColors - 1);
        int32 Index = FMath::FloorToInt(Alpha * (Spectrum.Num() - 1));
        float LerpAlpha = FMath::Frac(Alpha * (Spectrum.Num() - 1));

        FLinearColor Color = FLinearColor::LerpUsingHSV(Spectrum[Index], Spectrum[FMath::Min(Index + 1, Spectrum.Num() - 1)], LerpAlpha);
        Palette.Colors.Add(Color);
    }

    // Set primary color as the first color in the array
    if (Palette.Colors.Num() > 0)
    {
        Palette.PrimaryColor = Palette.Colors[0];
        Palette.SecondaryColor = Palette.Colors.Num() > 1 ? Palette.Colors[1] : Palette.PrimaryColor;
        Palette.HighlightColor = Palette.Colors.Num() > 2 ? Palette.Colors[2] : Palette.PrimaryColor;
        Palette.ShadowColor = GenerateShadowColor(Palette.PrimaryColor);
    }

    return Palette;
}

FPalette UPaletteGenerator::GenerateMonochromaticPalette(FLinearColor BaseColor, int32 NumColors, FVector2D SaturationRange, FVector2D ValueRange)
{
    FPalette Palette;
    if (NumColors <= 0)
    {
        return Palette;
    }

    float H, S, V;
    LinearRGBToHSV(BaseColor, H, S, V);

    for (int32 i = 0; i < NumColors; ++i)
    {
        float Alpha = (NumColors == 1) ? 0.5f : static_cast<float>(i) / static_cast<float>(NumColors - 1);
        float NewSaturation = FMath::Lerp(SaturationRange.X, SaturationRange.Y, Alpha);
        float NewValue = FMath::Lerp(ValueRange.X, ValueRange.Y, Alpha);

        FLinearColor NewColor = HSVToLinearRGB(H, NewSaturation, NewValue);
        Palette.Colors.Add(NewColor);
    }

    // Set primary color as the middle color
    if (Palette.Colors.Num() > 0)
    {
        int32 MidIndex = Palette.Colors.Num() / 2;
        Palette.PrimaryColor = Palette.Colors[MidIndex];
        Palette.SecondaryColor = Palette.Colors.Num() > 1 ? Palette.Colors[FMath::Max(0, MidIndex - 1)] : Palette.PrimaryColor;
        Palette.HighlightColor = Palette.Colors.Num() > 2 ? Palette.Colors[FMath::Min(Palette.Colors.Num() - 1, MidIndex + 1)] : Palette.PrimaryColor;
        Palette.ShadowColor = Palette.Colors[0];
    }

    return Palette;
}

// ========================================
// Private Helper Functions
// ========================================

FRandomStream UPaletteGenerator::GetSeededRandomStream(int32 Seed)
{
    return FRandomStream(Seed);
}

FLinearColor UPaletteGenerator::HSVToLinearRGB(float H, float S, float V)
{
    // UE4's HSVToLinearRGB expects H in range [0, 360], S and V in range [0, 1]
    FLinearColor HSV(H, S, V);
    return HSV.HSVToLinearRGB();
}

void UPaletteGenerator::LinearRGBToHSV(const FLinearColor& RGB, float& OutH, float& OutS, float& OutV)
{
    FLinearColor HSV = RGB.LinearRGBToHSV();
    OutH = HSV.R; // Hue (0-360)
    OutS = HSV.G; // Saturation (0-1)
    OutV = HSV.B; // Value (0-1)
}

FLinearColor UPaletteGenerator::BlendColorsHSV(const FLinearColor& ColorA, const FLinearColor& ColorB, float Alpha)
{
    return FLinearColor::LerpUsingHSV(ColorA, ColorB, Alpha);
}

FLinearColor UPaletteGenerator::GenerateComplementaryColor(const FLinearColor& BaseColor)
{
    float H, S, V;
    LinearRGBToHSV(BaseColor, H, S, V);

    // Complementary color is 180 degrees opposite on the color wheel
    float ComplementaryHue = H + 180.0f;
    if (ComplementaryHue >= 360.0f)
    {
        ComplementaryHue -= 360.0f;
    }

    return HSVToLinearRGB(ComplementaryHue, S, V);
}

FLinearColor UPaletteGenerator::GenerateShadowColor(const FLinearColor& BaseColor)
{
    float H, S, V;
    LinearRGBToHSV(BaseColor, H, S, V);

    // Shadow color: reduce value by 50%, slightly increase saturation
    float ShadowValue = V * 0.5f;
    float ShadowSaturation = FMath::Clamp(S + 0.1f, 0.0f, 1.0f);

    return HSVToLinearRGB(H, ShadowSaturation, ShadowValue);
}
