#pragma once

namespace color
{
	typedef struct
	{
		unsigned char R;
		unsigned char G;
		unsigned char B;
	} RGB;

	typedef unsigned int tCorrection;

	typedef enum : tCorrection
	{
		/// Typical values for SMD5050 LEDs
		ColorCorrectionTypicalSMD5050 = 0xFFB0F0 /* 255, 176, 240 */,
		/// @copydoc TypicalSMD5050
		ColorCorrectionTypicalLEDStrip = 0xFFB0F0 /* 255, 176, 240 */,

		/// Typical values for 8 mm "pixels on a string".
		/// Also for many through-hole 'T' package LEDs.
		ColorCorrectionTypical8mmPixel = 0xFFE08C /* 255, 224, 140 */,
		/// @copydoc Typical8mmPixel
		ColorCorrectionTypicalPixelString = 0xFFE08C /* 255, 224, 140 */,

		/// Uncorrected color (0xFFFFFF)
		ColorCorrectionUncorrectedColor = 0xFFFFFF /* 255, 255, 255 */

	} Correction;

	typedef unsigned int tTemperature;

	typedef enum : tTemperature
	{
		// Black Body Radiators
		// @{
		/// 1900 Kelvin
		ColorTemperatureCandle = 0xFF9329 /* 1900 K, 255, 147, 41 */,
		/// 2600 Kelvin
		ColorTemperatureTungsten40W = 0xFFC58F /* 2600 K, 255, 197, 143 */,
		/// 2850 Kelvin
		ColorTemperatureTungsten100W = 0xFFD6AA /* 2850 K, 255, 214, 170 */,
		/// 3200 Kelvin
		ColorTemperatureHalogen = 0xFFF1E0 /* 3200 K, 255, 241, 224 */,
		/// 5200 Kelvin
		ColorTemperatureCarbonArc = 0xFFFAF4 /* 5200 K, 255, 250, 244 */,
		/// 5400 Kelvin
		ColorTemperatureHighNoonSun = 0xFFFFFB /* 5400 K, 255, 255, 251 */,
		/// 6000 Kelvin
		ColorTemperatureDirectSunlight = 0xFFFFFF /* 6000 K, 255, 255, 255 */,
		/// 7000 Kelvin
		ColorTemperatureOvercastSky = 0xC9E2FF /* 7000 K, 201, 226, 255 */,
		/// 20000 Kelvin
		ColorTemperatureClearBlueSky = 0x409CFF /* 20000 K, 64, 156, 255 */,
		// @}

		// Gaseous Light Sources
		// @{
		/// Warm (yellower) flourescent light bulbs
		ColorTemperatureWarmFluorescent = 0xFFF4E5 /* 0 K, 255, 244, 229 */,
		/// Standard flourescent light bulbs
		ColorTemperatureStandardFluorescent = 0xF4FFFA /* 0 K, 244, 255, 250 */,
		/// Cool white (bluer) flourescent light bulbs
		ColorTemperatureCoolWhiteFluorescent = 0xD4EBFF /* 0 K, 212, 235, 255 */,
		/// Full spectrum flourescent light bulbs
		ColorTemperatureFullSpectrumFluorescent = 0xFFF4F2 /* 0 K, 255, 244, 242 */,
		/// Grow light flourescent light bulbs
		ColorTemperatureGrowLightFluorescent = 0xFFEFF7 /* 0 K, 255, 239, 247 */,
		/// Black light flourescent light bulbs
		ColorTemperatureBlackLightFluorescent = 0xA700FF /* 0 K, 167, 0, 255 */,
		/// Mercury vapor light bulbs
		ColorTemperatureMercuryVapor = 0xD8F7FF /* 0 K, 216, 247, 255 */,
		/// Sodium vapor light bulbs
		ColorTemperatureSodiumVapor = 0xFFD1B2 /* 0 K, 255, 209, 178 */,
		/// Metal-halide light bulbs
		ColorTemperatureMetalHalide = 0xF2FCFF /* 0 K, 242, 252, 255 */,
		/// High-pressure sodium light bulbs
		ColorTemperatureHighPressureSodium = 0xFFB74C /* 0 K, 255, 183, 76 */,
		// @}

		/// Uncorrected temperature (0xFFFFFF)
		UncorrectedTemperature = 0xFFFFFF /* 255, 255, 255 */
	} Temperature;
}
