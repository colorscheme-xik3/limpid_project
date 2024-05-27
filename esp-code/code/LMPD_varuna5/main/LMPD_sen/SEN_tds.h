/**
 * @file SEN_tds.h
 * @brief Header file for TDS sensor processing.
 * @details This file contains the definition of the sen0244_processing function,
 *          which is responsible for calculating the concentration of total dissolved solids
 *          in water based on raw sensor data and temperature readings.
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Your Name
 * 
 * @note Ensure proper calibration settings are applied before using this function.
 */


#ifndef SEN_TDS_H_
#define SEN_TDS_H_


/**
 * @brief Calculates the concentration of total dissolved solids (TDS) in water.
 *
 * This function processes raw voltage measurements and temperature readings from a
 * TDS sensor to calculate the concentration of TDS in water. It applies a temperature
 * compensation coefficient to adjust the voltage reading for variations in temperature,
 * then uses a polynomial equation to convert the compensated voltage into TDS concentration.
 *
 * The temperature compensation coefficient is calculated as 1.0 plus 0.02 times the difference
 * between the measured temperature and 25 degrees Celsius. This adjustment accounts for the
 * sensor's sensitivity to temperature changes.
 *
 * The polynomial equation used to calculate TDS concentration is derived from the sensor's
 * calibration curve, which relates voltage readings to TDS concentrations under standard conditions.
 *
 * @param voltage The raw voltage measurement from the TDS sensor in volts.
 * @param temperature The temperature of the water in degrees Celsius.
 * @return The calculated concentration of total dissolved solids in parts per million (ppm).
 */
float sen0244_processing(float voltage, float temperature);

#endif /* DS18B20_H_ */


