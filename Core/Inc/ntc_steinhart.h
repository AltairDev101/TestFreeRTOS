#ifndef __NTC_STEINHART_H
#define __NTC_STEINHART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
  * @brief  Computes temperature (in °C) from a 12-bit ADC raw value using
  *         Steinhart-Hart equation for an NTC thermistor.
  * @param  rawAdcValue: 12-bit ADC result (0..4095)
  * @retval Temperature in degrees Celsius
  */
float NTC_GetTemperature(uint16_t rawAdcValue);

#ifdef __cplusplus
}
#endif

#endif /* __NTC_STEINHART_H */
