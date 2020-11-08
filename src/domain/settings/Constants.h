#ifndef DIVEC_CONSTANTS_H
#define DIVEC_CONSTANTS_H

// Color definitions
#define BLACK 0x0000       ///<   0,   0,   0
#define NAVY 0x000F        ///<   0,   0, 123
#define DARKGREEN 0x03E0   ///<   0, 125,   0
#define DARKCYAN 0x03EF    ///<   0, 125, 123
#define MAROON 0x7800      ///< 123,   0,   0
#define PURPLE 0x780F      ///< 123,   0, 123
#define OLIVE 0x7BE0       ///< 123, 125,   0
#define LIGHTGREY 0xC618   ///< 198, 195, 198
#define DARKGREY 0x7BEF    ///< 123, 125, 123
#define BLUE 0x001F        ///<   0,   0, 255
#define GREEN 0x07E0       ///<   0, 255,   0
#define CYAN 0x07FF        ///<   0, 255, 255
#define RED 0xF800         ///< 255,   0,   0
#define MAGENTA 0xF81F     ///< 255,   0, 255
#define YELLOW 0xFFE0      ///< 255, 255,   0
#define ORANGE 0xFD20      ///< 255, 165,   0
#define GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define PINK 0xFC18        ///< 255, 130, 198
#define WHITE 0xFFFF       ///< 255, 255, 255

class Constants {
public:
    // WATER_VAPOR - need to choose between these 3. Schreiner being most conservative - US Navy in the middle - Buhlmann least conservative
    // Note: 1 msw = 0.1 bar
    // 0.493 msw = 0.0493 bar (Schreiner value - respiratory quotient 0.8)
    // 0.567 msw = 0.0567 bar (U.S. Navy value - respiratory quotient 0.9)
    // 0.627 msw = 0.0627 bar (Bühlmann value - respiratory quotient 1.0)
    constexpr static const double WATER_VAPOR = 0.0567; // bar. water vapor pressure (U.S. Navy value - at 35.5 Celsius temp in lungs);
    constexpr static const double FRESH_WATER_PRESSURE = 0.0980665; // bar/m
    constexpr static const double SALT_WATER_PRESSURE = 0.1010084; // bar/m

    static double BUHLMANN_ZHL16_A_TABLE[17][6];
    static double BUHLMANN_ZHL16_B_TABLE[17][6];
    static double BUHLMANN_ZHL16_C_TABLE[17][6];
};


#endif
