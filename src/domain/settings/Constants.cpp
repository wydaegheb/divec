#include "Constants.h"

// N2HalfTime, N2AValue, N2BValue, HeHalfTime, HeAValue, HeBValue
double Constants::BUHLMANN_ZHL16_A_TABLE[16][6] = {
        {5.0,   1.2599, 0.5050, 1.88,   1.6189, 0.4770},
        {8.0,   1.0000, 0.6514, 3.02,   1.3830, 0.5747},
        {12.5,  0.8618, 0.7222, 4.72,   1.1919, 0.6527},
        {18.5,  0.7562, 0.7725, 6.99,   1.0458, 0.7223},
        {27.0,  0.6667, 0.8125, 10.21,  0.9220, 0.7582},
        {38.3,  0.5933, 0.8434, 14.48,  0.8205, 0.7957},
        {54.3,  0.5282, 0.8693, 20.53,  0.7305, 0.8279},
        {77.0,  0.4701, 0.8910, 29.11,  0.6502, 0.8553},
        {109.0, 0.4187, 0.9092, 41.20,  0.5950, 0.8757},
        {146.0, 0.3798, 0.9222, 55.19,  0.5545, 0.8903},
        {187.0, 0.3497, 0.9319, 70.69,  0.5333, 0.8997},
        {239.0, 0.3223, 0.9403, 90.34,  0.5189, 0.9073},
        {305.0, 0.2971, 0.9477, 115.29, 0.5181, 0.9122},
        {390.0, 0.2737, 0.9544, 147.42, 0.5176, 0.9171},
        {498.0, 0.2523, 0.9602, 188.24, 0.5172, 0.9217},
        {635.0, 0.2327, 0.9653, 240.03, 0.5119, 0.9267}
};

// N2HalfTime, N2AValue, N2BValue, HeHalfTime, HeAValue, HeBValue
double Constants::BUHLMANN_ZHL16_B_TABLE[16][6] = {
        {5.0,   1.2599, 0.5050, 1.88,   1.6189, 0.4770},
        {8.0,   1.0000, 0.6514, 3.02,   1.3830, 0.5747},
        {12.5,  0.8618, 0.7222, 4.72,   1.1919, 0.6527},
        {18.5,  0.7562, 0.7725, 6.99,   1.0458, 0.7223},
        {27.0,  0.6667, 0.8125, 10.21,  0.9220, 0.7582},
        {38.3,  0.5600, 0.8434, 14.48,  0.8205, 0.7957},
        {54.3,  0.4947, 0.8693, 20.53,  0.7305, 0.8279},
        {77.0,  0.4500, 0.8910, 29.11,  0.6502, 0.8553},
        {109.0, 0.4187, 0.9092, 41.20,  0.5950, 0.8757},
        {146.0, 0.3798, 0.9222, 55.19,  0.5545, 0.8903},
        {187.0, 0.3497, 0.9319, 70.69,  0.5333, 0.8997},
        {239.0, 0.3223, 0.9403, 90.34,  0.5189, 0.9073},
        {305.0, 0.2971, 0.9477, 115.29, 0.5181, 0.9122},
        {390.0, 0.2737, 0.9544, 147.42, 0.5176, 0.9171},
        {498.0, 0.2523, 0.9602, 188.24, 0.5172, 0.9217},
        {635.0, 0.2327, 0.9653, 240.03, 0.5119, 0.9267}
};

// N2HalfTime, N2AValue, N2BValue, HeHalfTime, HeAValue, HeBValue
double Constants::BUHLMANN_ZHL16_C_TABLE[16][6] = {
        {5.0,   1.2599, 0.5050, 1.88,   1.6189, 0.4770},
        {8.0,   1.0000, 0.6514, 3.02,   1.3830, 0.5747},
        {12.5,  0.8618, 0.7222, 4.72,   1.1919, 0.6527},
        {18.5,  0.7562, 0.7725, 6.99,   1.0458, 0.7223},
        {27.0,  0.6200, 0.8125, 10.21,  0.9220, 0.7582},
        {38.3,  0.5043, 0.8434, 14.48,  0.8205, 0.7957},
        {54.3,  0.4410, 0.8693, 20.53,  0.7305, 0.8279},
        {77.0,  0.4000, 0.8910, 29.11,  0.6502, 0.8553},
        {109.0, 0.3750, 0.9092, 41.20,  0.5950, 0.8757},
        {146.0, 0.3500, 0.9222, 55.19,  0.5545, 0.8903},
        {187.0, 0.3295, 0.9319, 70.69,  0.5333, 0.8997},
        {239.0, 0.3065, 0.9403, 90.34,  0.5189, 0.9073},
        {305.0, 0.2835, 0.9477, 115.29, 0.5181, 0.9122},
        {390.0, 0.2610, 0.9544, 147.42, 0.5176, 0.9171},
        {498.0, 0.2480, 0.9602, 188.24, 0.5172, 0.9217},
        {635.0, 0.2327, 0.9653, 240.03, 0.5119, 0.9267}
};