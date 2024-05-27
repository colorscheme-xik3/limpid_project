/**
 * @file SEN_turb.h
 * @brief Header file for Turbidity Sensor Processing
 * @details This file contains the declaration of the sen0189_processing function,
 *          which processes raw voltage measurements from a turbidity sensor to determine
 *          the clarity of water. The clarity is categorized into four levels: clear, cloudy,
 *          moderate, and dirty, based on the calculated Nephelometric Turbidity Units (NTU).
 * @version 1.0
 * @date 2023-07-24
 * 
 * @author Francisco Duarte
 * 
 * @note The function sen0189_processing returns a string literal indicating the water clarity level.
 *       The caller should treat the returned string as immutable and not attempt to free it.
 *       The constants defined in this header file represent the thresholds for categorizing water clarity.
 */

#ifndef SEN_TURB_H_
#define SEN_TURB_H_

#include <stdint.h>
#include <stdio.h>

static const float NTU_CLEAR_THRESHOLD = 60;
static const float NTU_CLOUDY_THRESHOLD = 100;
static const float NTU_MODERATE_THRESHOLD = 400;
static const float MAX_NTU_VALUE = 3000;

/**
 * @brief Processes raw voltage measurements from a turbidity sensor to determine water clarity.
 *
 * This function calculates the Nephelometric Turbidity Units (NTU) based on the input voltage,
 * then categorizes the water clarity into one of four levels: clear, cloudy, moderate, or dirty.
 * The categorization is determined by comparing the calculated NTU against predefined thresholds.
 *
 * @param voltage The raw voltage measurement from the turbidity sensor.
 * @return A string literal indicating the water clarity level ("clear", "cloudy", "moderate", or "dirty").
 */
char* sen0189_processing(float voltage);

#endif 
