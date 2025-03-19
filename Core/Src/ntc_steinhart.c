/*==============================================================================
 * Hardware parameters and thermistor constants:
 *    - NTC_UP_R:      The reference resistor (in ohms) placed in 
 *                     series with the NTC.
 *    - STEINHART_A,
 *      STEINHART_B,
 *      STEINHART_C:   Coefficients for the Steinhart-Hart equation.
 *    - ADC_MAX:       The maximum ADC value for 12-bit resolution (4095).
 *
 * Formulas:
 *      R_therm = (NTC_UP_R) / ( (ADC_MAX / adcVal) - 1 )
 *      lnR = ln(R_therm)
 *      T (K) = 1 / (A + B * lnR + C * (lnR)^3)
 *      T (°C) = T(K) - 273.15
 *
 *============================================================================*/
#include "ntc_steinhart.h"
#include <math.h>

#define NTC_UP_R         3300.0f   // Reference resistor in ohms
#define STEINHART_A      0.0008736528f
#define STEINHART_B      0.000253893f
#define STEINHART_C      0.0000001816f
#define ADC_MAX          4095.0f

/**
  * @brief  Computes the temperature in Celsius from a 12-bit ADC raw value
  *         using the Steinhart-Hart equation for an NTC thermistor.
  * @param  rawAdcValue: 12-bit ADC result (range 0..4095)
  * @retval Temperature in degrees Celsius
  */
float NTC_GetTemperature(uint16_t rawAdcValue)
{
    // Check for boundary cases that can cause division by zero
    if (rawAdcValue == 0) {
        // ADC=0 might indicate extremely low thermistor resistance
        // or an open circuit. Return NaN as an error.
        return NAN;
    }
    if (rawAdcValue >= (uint16_t)ADC_MAX) {
        // ADC=4095 might indicate extremely high thermistor resistance
        // or an open circuit. Return NaN as an error.
        return NAN;
    }

    //Calculate the ratio: ADC_MAX / rawAdcValue
    float ratio = (float)ADC_MAX / (float)rawAdcValue;

    //Compute the thermistor resistance using the voltage divider formula
    float R_therm = NTC_UP_R / (ratio - 1.0f);

    //Compute the natural logarithm of the thermistor resistance
    float lnR = logf(R_therm);

    //Use the Steinhart-Hart equation to get the temperature in Kelvin
    float tempKelvin = 1.0f / (STEINHART_A + (STEINHART_B * lnR) + (STEINHART_C * lnR * lnR * lnR));

    //Convert Kelvin to Celsius
    float tempCelsius = tempKelvin - 273.15f;

    return tempCelsius;
}
