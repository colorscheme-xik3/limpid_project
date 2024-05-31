/**
 * @file SEN_do.h
 * @brief Header file for Dissolved Oxygen (DO) sensor processing.
 * @details This file contains the definition of the sen0237_processing function,
 *          which is responsible for calculating the concentration of dissolved oxygen
 *          in water based on raw sensor data and temperature readings.
 *          The function supports both single-point and two-point calibration methods.
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Francisco Duarte
 * 
 * @note Make sure to define TWO_POINT_CALIBRATION before including this header.
 *       Set TWO_POINT_CALIBRATION to 0 for single-point calibration or 1 for two-point calibration.
 */

#ifndef SEN_DO_H
#define SEN_DO_H

#include <stdint.h>
#include "SEN_ds.h"


#define TWO_POINT_CALIBRATION 1
#define READ_TEMP 25
#define CAL1_V 1200.5f
#define CAL1_T 40
#define CAL05_V 900.5f
#define CAL05_T 23
#define CAL2_V 650.5f
#define CAL2_T 13
#define TEMPERATURE_COEFFICIENT 35

/**
 * @brief Processes raw sensor data to calculate dissolved oxygen concentration.
 *
 * This function takes raw voltage measurements and temperature readings from a
 * Dissolved Oxygen (DO) sensor, applies either single-point or two-point calibration
 * based on the defined mode, and calculates the concentration of dissolved oxygen
 * in water.
 *
 * @param voltage_mv The raw voltage measurement from the DO sensor in millivolts.
 * @param temperature_c The temperature of the water in degrees Celsius from DS18B20.
 * @return The calculated concentration of dissolved oxygen in mg/L.
 */
float sen0237_processing(float voltage_mv, float temperature_c);

#endif
