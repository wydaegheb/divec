#ifndef DIVEC_GASFORMATTEDMENUITEM_H
#define DIVEC_GASFORMATTEDMENUITEM_H

#include <RuntimeMenuItem.h>
#include <domain/dive/Gas.h>


struct GasStorage {
    GasType gasType;
    bool active;
    uint8_t o2Tens;
    uint8_t o2Units;
    uint8_t heTens;
    uint8_t heUnits;

    GasStorage() {
        gasType = GAS_TYPE_OC;
        active = false;
        o2Tens = o2Units = heTens = heUnits = 0;
    }

    explicit GasStorage(Gas *gas) {
        gasType = gas->getGasType();
        active = gas->isActive();
        o2Tens = gas->getO2() / 10;
        o2Units = gas->getO2() - o2Tens * 10;
        heTens = gas->getHe() / 10;
        heUnits = gas->getHe() - heTens * 10;
    }

    void updateGas(Gas *gas) const {
        gas->setGasType(gasType);
        gas->setActive(active);
        gas->setO2(o2Tens * 10 + o2Units);
        gas->setHe(heTens * 10 + heUnits);
    }
};


class GasFormattedMenuItem : public EditableMultiPartMenuItem<GasStorage *> {
public:
    GasFormattedMenuItem(Gas *gas, uint16_t id, MenuItem *next = nullptr)
            : EditableMultiPartMenuItem(MENUTYPE_TEXT_VALUE, id, 6, gasItemRenderFn, next) {
        _gas = gas;
        setGasStorage(new GasStorage(gas));
    }

    GasStorage *getGasStorage() { return data; }

    void setGasStorage(GasStorage *newGasStorage) { data = newGasStorage; }

    void updateGas() {
        getGasStorage()->updateGas(_gas);
    }

    static void wrapGasForEdit(uint8_t val, int idx, uint8_t row, char *buffer, int bufferSize) {
        --row;

        if (idx == row) appendChar(buffer, '[', bufferSize);

        if (idx == 0) { // OC/CC
            strcat(buffer, (val == GAS_TYPE_OC ? "OC" : "CC"));
        } else if (idx == 1) { // active or not
            strcat(buffer, (val ? "On" : "Off"));
        } else { // O2 tens, O2 units, He tens, He units
            fastltoa(buffer, val, 1, NOT_PADDED, bufferSize);
        }

        if (idx == row) appendChar(buffer, ']', bufferSize);

    }

    static int gasItemRenderFn(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize) {
        if (item->getMenuType() != MENUTYPE_TEXT_VALUE) return 0;
        auto gasItem = reinterpret_cast<GasFormattedMenuItem *>(item);
        auto idx = row - 1;
        auto gasStorage = gasItem->getGasStorage();


        switch (mode) {
            case RENDERFN_NAME: {
                if (buffer) buffer[0] = 0;
                char label[10];
                label[0] = 0;
                fastltoa(label, item->getId() + 1, 2, '0', 4);
                safeProgCpy(buffer, label, bufferSize);
                return true;
            }
            case RENDERFN_VALUE: {
                buffer[0] = 0;
                wrapGasForEdit(gasStorage->gasType, 0, row, buffer, bufferSize);
                strcat(buffer, SPACER_15);
                wrapGasForEdit(gasStorage->active, 1, row, buffer, bufferSize);
                strcat(buffer, SPACER_10);
                wrapGasForEdit(gasStorage->o2Tens, 2, row, buffer, bufferSize);
                wrapGasForEdit(gasStorage->o2Units, 3, row, buffer, bufferSize);
                strcat(buffer, "/");
                wrapGasForEdit(gasStorage->heTens, 4, row, buffer, bufferSize);
                wrapGasForEdit(gasStorage->heUnits, 5, row, buffer, bufferSize);
                if (row == 0 || row > 6) {
                    strcat(buffer, SPACER_2);
                }
                return true;
            }
            case RENDERFN_GETRANGE: {
                if (idx == 0) return GAS_TYPE_CC;
                else if (idx == 1) return 1;
                else if (idx < 6) return 9;
                else return true;
            }
            case RENDERFN_GETPART: {
                if (idx == 0) return gasStorage->gasType;
                else if (idx == 1) return gasStorage->active;
                else if (idx == 2) return gasStorage->o2Tens;
                else if (idx == 3) return gasStorage->o2Units;
                else if (idx == 4) return gasStorage->heTens;
                else return gasStorage->heUnits;
            }
            case RENDERFN_SET_VALUE: {
                if (idx == 0) {
                    gasStorage->gasType = (GasType) buffer[0];
                } else if (idx == 1) {
                    gasStorage->active = (buffer[0] > 0);
                } else if (idx == 2) { // o2 tens
                    gasStorage->o2Tens = (uint8_t) buffer[0];
                } else if (idx == 3) { // o2 units
                    gasStorage->o2Units = (uint8_t) buffer[0];
                } else if (idx == 4) { // he tens
                    gasStorage->heTens = (uint8_t) buffer[0];
                } else if (idx == 5) { // he units
                    gasStorage->heUnits = (uint8_t) buffer[0];
                }
                return true;
            }
            case RENDERFN_INVOKE: {
                gasItem->updateGas();
                return true;
            }
            case RENDERFN_EEPROM_POS: {
                return -1;
            }
            default:
                return false;
        }
    }

private:
    Gas *_gas;
};

#endif //DIVEC_GASFORMATTEDMENUITEM_H
