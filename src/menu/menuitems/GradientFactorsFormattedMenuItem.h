#ifndef DIVEC_GRADIENTFACTORSFORMATTEDMENUITEM_H
#define DIVEC_GRADIENTFACTORSFORMATTEDMENUITEM_H

#include <RuntimeMenuItem.h>
#include <domain/settings/Settings.h>

const char GRADIENT_FACTORS[] PROGMEM = "GF";

struct GradientFactorsStorage {
    uint8_t gfLowTens;
    uint8_t gfLowUnits;
    uint8_t gfHighTens;
    uint8_t gfHighUnits;

    GradientFactorsStorage() {
        gfLowTens = gfLowUnits = gfHighTens = gfHighUnits = 0;
    }

    explicit GradientFactorsStorage(uint8_t gfLow, uint8_t gfHigh) {
        if (gfLow == 100) gfLow = 99;
        if (gfHigh == 100) gfHigh = 99;
        gfLowTens = gfLow / 10;
        gfLowUnits = gfLow - gfLowTens * 10;
        gfHighTens = gfHigh / 10;
        gfHighUnits = gfHigh - gfHighTens * 10;
    }

    void updateGradientFactors() const {
        Settings::GF_LOW = (gfLowTens * 10 + gfLowUnits) / 100.0;
        Settings::GF_HIGH = (gfHighTens * 10 + gfHighUnits) / 100.0;
    }
};

class GradientFactorsFormattedMenuItem : public EditableMultiPartMenuItem<GradientFactorsStorage *> {
public:
    GradientFactorsFormattedMenuItem(uint16_t id, MenuItem *next = nullptr)
            : EditableMultiPartMenuItem(MENUTYPE_TEXT_VALUE, id, 4, gradientFactorsItemRenderFn, next) {
        setGradientFactorsStorage(new GradientFactorsStorage(Settings::GF_LOW * 100, Settings::GF_HIGH * 100));
    }

    GradientFactorsStorage *getGradientFactorsStorage() { return data; }

    void setGradientFactorsStorage(GradientFactorsStorage *gradientFactorsStorage) { data = gradientFactorsStorage; }


    static void wrapGasForEdit(uint8_t val, int idx, uint8_t row, char *buffer, int bufferSize) {
        --row;

        if (idx == row) appendChar(buffer, '[', bufferSize);
        fastltoa(buffer, val, 1, NOT_PADDED, bufferSize);
        if (idx == row) appendChar(buffer, ']', bufferSize);

    }

    static int
    gradientFactorsItemRenderFn(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize) {
        if (item->getMenuType() != MENUTYPE_TEXT_VALUE) return 0;
        auto gradientFactorsItem = reinterpret_cast<GradientFactorsFormattedMenuItem *>(item);
        auto idx = row - 1;
        auto gradientFactorsStorage = gradientFactorsItem->getGradientFactorsStorage();


        switch (mode) {
            case RENDERFN_NAME: {
                if (buffer) buffer[0] = 0;
                safeProgCpy(buffer, GRADIENT_FACTORS, bufferSize);
                return true;
            }
            case RENDERFN_VALUE: {
                buffer[0] = 0;
                wrapGasForEdit(gradientFactorsStorage->gfLowTens, 0, row, buffer, bufferSize);
                wrapGasForEdit(gradientFactorsStorage->gfLowUnits, 1, row, buffer, bufferSize);
                strcat(buffer, "/");
                wrapGasForEdit(gradientFactorsStorage->gfHighTens, 2, row, buffer, bufferSize);
                wrapGasForEdit(gradientFactorsStorage->gfHighUnits, 3, row, buffer, bufferSize);
                return true;
            }
            case RENDERFN_GETRANGE: {
                if (idx < 4) return 9;
                else return true;
            }
            case RENDERFN_GETPART: {
                if (idx == 0) return gradientFactorsStorage->gfLowTens;
                else if (idx == 1) return gradientFactorsStorage->gfLowUnits;
                else if (idx == 2) return gradientFactorsStorage->gfHighTens;
                else return gradientFactorsStorage->gfHighUnits;
            }
            case RENDERFN_SET_VALUE: {
                if (idx == 0) {
                    gradientFactorsStorage->gfLowTens = (uint8_t) buffer[0];
                } else if (idx == 1) {
                    gradientFactorsStorage->gfLowUnits = (uint8_t) buffer[0];
                } else if (idx == 2) {
                    gradientFactorsStorage->gfHighTens = (uint8_t) buffer[0];
                } else if (idx == 3) {
                    gradientFactorsStorage->gfHighUnits = (uint8_t) buffer[0];
                }
                return true;
            }
            case RENDERFN_INVOKE: {
                gradientFactorsStorage->updateGradientFactors();
                return true;
            }
            case RENDERFN_EEPROM_POS: {
                return -1;
            }
            default:
                return false;
        }
    }
};

#endif //DIVEC_GRADIENTFACTORSFORMATTEDMENUITEM_H
