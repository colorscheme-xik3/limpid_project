/**
 * @file SEN_ph.h 
 * @brief Header file for PH_SEN sensors
 * @details This driver provides functions to initialize, configure, and process data from pH sensors.
 *          It supports both two-point and three-point calibration methods for accurate pH measurements.
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Francisco Duarte
 * @author Leonardo Rosa
 * 
 * @note This module requires the inclusion of "SEN_ph.h" for Potential Hydrogen processing functionality.
 */

#ifndef PH_DRIVER_H_
#define PH_DRIVER_H_

#include <stdio.h>
#include "math.h"
#include <stdio.h>



/**
 * @brief Calibration points configuration.
 *
 * Set this to 0 for 2 points calibration or 1 for 3 points calibration.
 */
#define CALIBRATION_POINTS   1

/**
 * @brief pH sensor type configuration.
 *
 * Set this to 0 for SEN_0161 or 1 for SEN_0169_V2.
 */
#define PH_SENSOR            1

/**
 * @brief pH calibration values.
 *
 * This array stores the pH calibration values used for calibration.
 */
extern float pHValues[];

/**
 * @brief Voltage readings.
 *
 * This array stores the voltage readings used for calibration.
 */
extern float voltageReadings[];

/**
 * @brief Voltage readings for SEN_0161.
 *
 * This array stores the voltage readings used for calibration with SEN_0161 sensor.
 */
extern float voltageReadings_SEN0161[];

/**
 * @brief Voltage readings for SEN_0169_V2.
 *
 * This array stores the voltage readings used for calibration with SEN_0169_V2 sensor.
 */
extern float voltageReadings_SEN0169[];



/* Driver functions instantiation*/
/**
 * @brief Initialize the pH sensor.
 *
 * This function initializes the pH sensor by choosing the appropriate sensor and setting the calibration values.
 *
 * @param[in] sensor_ph The pH sensor type to be used (0 for SEN_0161 or 1 for SEN_0169_V2).
 */
void LMPD_PH_selector(int sensor_ph);

/**
 * @brief Calibrate the pH sensor.
 *
 * This function calibrates the pH sensor based on the calibration points and voltage reading provided.
 *
 * @param[in] calib_points Number of calibration points (0 for 2 points calibration or 1 for 3 points calibration).
 * @param[in] voltage The voltage reading for calibration.
 * @return The calibrated pH value.
 */
float LMPD_PH_calibrator(int calib_points, int voltage, float temperature);

/**
 * @brief Calibrate the pH sensor using 3 points calibration.
 *
 * This function calibrates the pH sensor using 3 points calibration method.
 *
 * @param[in] voltage The voltage reading for calibration.
 * @param temperature Temperature of the solution.
 * @return The calibrated pH value.
 */
float LMPD_PH_calibrate3p(int voltage, float temperature, int sensor_ph);

/**
 * @brief Calibrate the pH sensor using 2 points calibration.
 *
 * This function calibrates the pH sensor using 2 points calibration method.
 *
 * @param[in] voltage The voltage reading for calibration.
 * @return The calibrated pH value.
 */
float LMPD_PH_calibrate2p(int voltage);

#endif /* PH_DRIVER_H_ */