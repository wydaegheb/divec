#ifndef DIVEC_DATETIMEFORMATTEDMENUITEM_H
#define DIVEC_DATETIMEFORMATTEDMENUITEM_H

#include <RuntimeMenuItem.h>
#include <time/Time.h>

const char DATE_TIME[] PROGMEM = "Date";

enum months : uint8_t {
    Jan = 0, Feb = 1, Mar = 2, Apr = 3, Mai = 4, Jun = 5, Jul = 6, Aug = 7, Sep = 8, Oct = 9, Nov = 10, Dec = 11
};

struct DateTimeStorage {
    uint8_t day;

    uint8_t month;

    uint8_t yearThousands;
    uint8_t yearHundreds;
    uint8_t yearTens;
    uint8_t yearUnit;

    uint8_t hour;
    uint8_t minute;

    DateTimeStorage() {
        day = 1;

        month = 1;

        yearThousands = 2;
        yearHundreds = 0;
        yearTens = 2;
        yearUnit = 1;

        hour = 0;
        minute = 0;
    }

    explicit DateTimeStorage(uint32_t timestamp) {
        setDateTime(timestamp);
    }

    void setDateTime(uint32_t timestamp) {
        DateTime dateTime = DateTime(timestamp);
        day = dateTime.day() - 1;

        month = dateTime.month() - 1;

        yearThousands = dateTime.year() / 1000;
        yearHundreds = (dateTime.year() - yearThousands * 1000) / 100;
        yearTens = (dateTime.year() - yearThousands * 1000 - yearHundreds * 100) / 10;
        yearUnit = dateTime.year() - yearThousands * 1000 - yearHundreds * 100 - yearTens * 10;

        hour = dateTime.hour();
        minute = dateTime.minute();
    }

    DateTime getDateTime() const {
        uint16_t year = yearThousands * 1000 + yearHundreds * 100 + yearTens * 10 + yearUnit;
        return DateTime(year, month + 1, day + 1, hour, minute, 0);
    }
};


class DateTimeFormattedMenuItem : public EditableMultiPartMenuItem<DateTimeStorage *> {
public:
    DateTimeFormattedMenuItem(uint16_t id, MenuItem *next = nullptr)
            : EditableMultiPartMenuItem(MENUTYPE_TEXT_VALUE, id, 8, dateTimeItemRenderFn, next) {
        setDateTimeStorage(new DateTimeStorage(Time::getTime()));
    }

    void refreshDateTime() {
        data->setDateTime(Time::getTime());
    }

    void updateDateTime() {
        Time::setDateTime(data->getDateTime());
    }

    DateTimeStorage *getDateTimeStorage() { return data; }

    void setDateTimeStorage(DateTimeStorage *dateTimeStorage) { data = dateTimeStorage; }


    static void wrapDateTimeForEdit(uint8_t val, int idx, uint8_t row, char *buffer, int bufferSize) {
        --row;

        if (idx == row) appendChar(buffer, '[', bufferSize);

        if (idx == 0) { // days have 2 digits but need an offset of 1 (day 0 doesn't exist)
            fastltoa(buffer, val + 1, 2, '0', bufferSize);
        } else if (idx == 6 ||
                   idx == 7) { // hours,minutes have 2 digits but can be zero (00h00 exists -> no offset needed)
            fastltoa(buffer, val, 2, '0', bufferSize);
        } else if (idx == 1) { // map month on label
            if (val == Jan) {
                strcat(buffer, "jan");
            } else if (val == Feb) {
                strcat(buffer, "feb");
            } else if (val == Mar) {
                strcat(buffer, "mar");
            } else if (val == Apr) {
                strcat(buffer, "apr");
            } else if (val == Mai) {
                strcat(buffer, "mai");
            } else if (val == Jun) {
                strcat(buffer, "jun");
            } else if (val == Jul) {
                strcat(buffer, "jul");
            } else if (val == Aug) {
                strcat(buffer, "aug");
            } else if (val == Sep) {
                strcat(buffer, "sep");
            } else if (val == Oct) {
                strcat(buffer, "oct");
            } else if (val == Nov) {
                strcat(buffer, "nov");
            } else if (val == Dec) {
                strcat(buffer, "dec");
            }
        } else { // the rest are digits of the year -> 1 digit
            fastltoa(buffer, val, 1, NOT_PADDED, bufferSize);
        }

        if (idx == row) appendChar(buffer, ']', bufferSize);

    }

    static int daysForMonth(uint8_t month, uint16_t year) {
        if (month == 4 || month == 6 || month == 9 || month == 11)
            return 30;
        else if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            return isLeap ? 29 : 28;
        } else return 31;
    }

    static int
    dateTimeItemRenderFn(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize) {
        if (item->getMenuType() != MENUTYPE_TEXT_VALUE) return 0;
        auto dateTimeItem = reinterpret_cast<DateTimeFormattedMenuItem *>(item);
        auto idx = row - 1;
        auto dateTimeStorage = dateTimeItem->getDateTimeStorage();

        switch (mode) {

            case RENDERFN_NAME: {
                if (buffer) buffer[0] = 0;
                safeProgCpy(buffer, DATE_TIME, bufferSize);
                return true;
            }

            case RENDERFN_VALUE: {
                Serial.println(F("RENDERFN_VALUE"));

                dateTimeItem->refreshDateTime(); // keep time up to date (avoids that in edit mode the time starts to lag)

                buffer[0] = 0;
                wrapDateTimeForEdit(dateTimeStorage->day, 0, row, buffer, bufferSize);
                strcat(buffer, "-");
                wrapDateTimeForEdit(dateTimeStorage->month, 1, row, buffer, bufferSize);
                strcat(buffer, "-");
                wrapDateTimeForEdit(dateTimeStorage->yearThousands, 2, row, buffer, bufferSize);
                wrapDateTimeForEdit(dateTimeStorage->yearHundreds, 3, row, buffer, bufferSize);
                wrapDateTimeForEdit(dateTimeStorage->yearTens, 4, row, buffer, bufferSize);
                wrapDateTimeForEdit(dateTimeStorage->yearUnit, 5, row, buffer, bufferSize);
                strcat(buffer, "   ");
                wrapDateTimeForEdit(dateTimeStorage->hour, 6, row, buffer, bufferSize);
                appendChar(buffer, 'h', bufferSize);
                wrapDateTimeForEdit(dateTimeStorage->minute, 7, row, buffer, bufferSize);
                return true;
            }

            case RENDERFN_GETRANGE: {
                Serial.println(F("RENDERFN_GETRANGE"));
                if (idx == 0) { // day
                    uint16_t year = dateTimeStorage->yearThousands * 1000 + dateTimeStorage->yearHundreds * 100 +
                                    dateTimeStorage->yearTens * 10 + dateTimeStorage->yearUnit;
                    return daysForMonth(dateTimeStorage->month + 1, year) - 1; // zero based
                } else if (idx == 1) { // month
                    return 11; // 0 based
                } else if (idx == 2 || idx == 3 || idx == 4 || idx == 5) { // years thousands, hundreds, decades, years
                    return 9;
                } else if (idx == 6) { // hours
                    return 23;
                } else if (idx == 7) { // minutes
                    return 59;
                } else return true;
            }

            case RENDERFN_GETPART: {
                Serial.println(F("RENDERFN_GETPART"));
                if (idx == 0) return dateTimeStorage->day;
                else if (idx == 1) return dateTimeStorage->month;
                else if (idx == 2) return dateTimeStorage->yearThousands;
                else if (idx == 3) return dateTimeStorage->yearHundreds;
                else if (idx == 4) return dateTimeStorage->yearTens;
                else if (idx == 5) return dateTimeStorage->yearUnit;
                else if (idx == 6) return dateTimeStorage->hour;
                else return dateTimeStorage->minute;
            }

            case RENDERFN_SET_VALUE: {
                Serial.println(F("RENDERFN_SET_VALUE"));
                if (idx == 0) dateTimeStorage->day = *((int *) buffer);
                else if (idx == 1) dateTimeStorage->month = *((int *) buffer);
                else if (idx == 2) dateTimeStorage->yearThousands = (uint8_t) buffer[0];
                else if (idx == 3) dateTimeStorage->yearHundreds = (uint8_t) buffer[0];
                else if (idx == 4) dateTimeStorage->yearTens = (uint8_t) buffer[0];
                else if (idx == 5) dateTimeStorage->yearUnit = (uint8_t) buffer[0];
                else if (idx == 6) dateTimeStorage->hour = *((int *) buffer);
                else dateTimeStorage->minute = *((int *) buffer);
                return true;
            }

            case RENDERFN_INVOKE: {
                Serial.print(F("RENDERFN_INVOKE:"));
                Serial.println(dateTimeItem == nullptr);
                dateTimeItem->updateDateTime();
                return true;
            }

            case RENDERFN_EEPROM_POS: {
                Serial.println(F("RENDERFN_EEPROM_POS"));
                return -1;
            }
            default:
                return false;
        }
    }
};

#endif //DIVEC_DATETIMEFORMATTEDMENUITEM_H
