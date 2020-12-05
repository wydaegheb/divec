// Slightly adapted version of the MS5837 library of Blue Robotics.
// Using double iso float
// removed conversion, depth and altitude (calculations will be done by the client based on the returned pressure)

/* Blue Robotics Arduino MS5837-30BA Pressure/Temperature Sensor Library
------------------------------------------------------------
 
Title: Blue Robotics Arduino MS5837-30BA Pressure/Temperature Sensor Library

Description: This library provides utilities to communicate with and to
read data from the Measurement Specialties MS5837-30BA pressure/temperature 
sensor.

Authors: Rustom Jehangir, Blue Robotics Inc.
         Adam Šimko, Blue Robotics Inc.

-------------------------------
The MIT License (MIT)

Copyright (c) 2015 Blue Robotics Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/

#ifndef MS5837_H_BLUEROBOTICS
#define MS5837_H_BLUEROBOTICS

#include <Wire.h>
#include <Arduino.h>

class DIVEC_MS5837 {
public:
    static const uint8_t MS5837_30BA;
    static const uint8_t MS5837_02BA;

    bool init();

    /** Set model of MS5837 sensor. Valid options are MS5837::MS5837_30BA (default)
     * and MS5837::MS5837_02BA.
     */
    void setModel(uint8_t model);

    /** The read from I2C takes up to 40 ms, so use sparingly is possible.
     */
    void read();

    /** Pressure returned in mbar
     */
    uint32_t pressure();

    /** Temperature returned in centi deg C.
     */
    int32_t temperature();

private:
    uint16_t C[8];
    uint32_t D1, D2;
    int32_t TEMP;
    uint32_t P;
    uint8_t _model;

    /** Performs calculations per the sensor data sheet for conversion and
     *  second order compensation.
     */
    void calculate();

    uint8_t crc4(uint16_t n_prom[]);
};

#endif
