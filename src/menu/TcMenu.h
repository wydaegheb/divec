#ifndef DIVEC_TCMENU_H
#define DIVEC_TCMENU_H

int CALLBACK_FUNCTION
fnDefineGassesGassesRtCall(RuntimeMenuItem
*item,
uint8_t row, RenderFnMode
mode,
char *buffer,
int bufferSize
) {
switch (mode) {
case RENDERFN_INVOKE:
// TODO - your code to invoke goes here - row is the index of the item
return true;
case RENDERFN_NAME:
// TODO - each row has it's own name (for list items row==LIST_PARENT_ITEMPOS is back)
ltoaClrBuff(buffer, row,
3, NOT_PADDED, bufferSize);
return true;
case RENDERFN_VALUE:
// TODO - return a value for the row (for list items row==LIST_PARENT_ITEMPOS is back)
buffer[0] = 'V';
buffer[1] = 0;
fastltoa(buffer, row,
3, NOT_PADDED, bufferSize);
return true;
case RENDERFN_EEPROM_POS:
return 0xFFFF; // lists are generally not saved to EEPROM
default:
return false;
}
}

const AnyMenuInfo PROGMEM
minfoExit = {"Exit", 5, 0xFFFF, 0, &Menu::onExit};
ActionMenuItem menuExit(&minfoExit, NULL);

ListRuntimeMenuItem menuDefineGassesGasses(7, 6, fnDefineGassesGassesRtCall, NULL);
const SubMenuInfo PROGMEM
minfoDefineGasses = {"Define Gasses", 6, 0xFFFF, 0, NO_CALLBACK};
RENDERING_CALLBACK_NAME_INVOKE(fnDefineGassesRtCall, backSubItemRenderFn,
"Define Gasses", -1, NO_CALLBACK)

BackMenuItem menuBackDefineGasses(fnDefineGassesRtCall, &menuDefineGassesGasses);
SubMenuItem menuDefineGasses(&minfoDefineGasses, &menuBackDefineGasses, &menuExit);

const char enumStrDensity_0[]
PROGMEM = "Salt";
const char enumStrDensity_1[]
PROGMEM = "Fresh";
const char *const enumStrDensity[]
PROGMEM = {enumStrDensity_0, enumStrDensity_1};
const EnumMenuInfo PROGMEM
minfoDensity = {"Density", 3, 0xFFFF, 1, &Menu::onWaterDensity, enumStrDensity};
EnumMenuItem menuDensity(&minfoDensity, 0, &menuDefineGasses);

const char enumStrLastStop_0[]
PROGMEM = "3 m";
const char enumStrLastStop_1[]
PROGMEM = "6 m";
const char *const enumStrLastStop[]
PROGMEM = {enumStrLastStop_0, enumStrLastStop_1};
const EnumMenuInfo PROGMEM
minfoLastStop = {"Last Stop", 11, 0xFFFF, 1, &Menu::onLastStop, enumStrLastStop};
EnumMenuItem menuLastStop(&minfoLastStop, 0, &menuDensity);

const char enumStrDecoStepSize_0[]
PROGMEM = "3 m";
const char enumStrDecoStepSize_1[]
PROGMEM = "6 m";
const char *const enumStrDecoStepSize[]
PROGMEM = {enumStrDecoStepSize_0, enumStrDecoStepSize_1};
const EnumMenuInfo PROGMEM
minfoDecoStepSize = {"Deco Step Size", 10, 0xFFFF, 1, &Menu::onDecoStepSize, enumStrDecoStepSize};
EnumMenuItem menuDecoStepSize(&minfoDecoStepSize, 0, &menuLastStop);

const char enumStrGFHigh_0[]
PROGMEM = "100";
const char enumStrGFHigh_1[]
PROGMEM = "95";
const char enumStrGFHigh_2[]
PROGMEM = "90";
const char enumStrGFHigh_3[]
PROGMEM = "85";
const char enumStrGFHigh_4[]
PROGMEM = "80";
const char enumStrGFHigh_5[]
PROGMEM = "75";
const char enumStrGFHigh_6[]
PROGMEM = "70";
const char enumStrGFHigh_7[]
PROGMEM = "65";
const char enumStrGFHigh_8[]
PROGMEM = "60";
const char enumStrGFHigh_9[]
PROGMEM = "55";
const char enumStrGFHigh_10[]
PROGMEM = "50";
const char enumStrGFHigh_11[]
PROGMEM = "45";
const char enumStrGFHigh_12[]
PROGMEM = "40";
const char enumStrGFHigh_13[]
PROGMEM = "35";
const char enumStrGFHigh_14[]
PROGMEM = "30";
const char enumStrGFHigh_15[]
PROGMEM = "25";
const char enumStrGFHigh_16[]
PROGMEM = "20";
const char enumStrGFHigh_17[]
PROGMEM = "15";
const char enumStrGFHigh_18[]
PROGMEM = "10";
const char enumStrGFHigh_19[]
PROGMEM = "5";
const char *const enumStrGFHigh[]
PROGMEM = {enumStrGFHigh_0, enumStrGFHigh_1, enumStrGFHigh_2, enumStrGFHigh_3, enumStrGFHigh_4, enumStrGFHigh_5, enumStrGFHigh_6, enumStrGFHigh_7, enumStrGFHigh_8, enumStrGFHigh_9, enumStrGFHigh_10,
           enumStrGFHigh_11, enumStrGFHigh_12, enumStrGFHigh_13, enumStrGFHigh_14, enumStrGFHigh_15, enumStrGFHigh_16, enumStrGFHigh_17, enumStrGFHigh_18, enumStrGFHigh_19};
const EnumMenuInfo PROGMEM
minfoGFHigh = {"GF High", 9, 0xFFFF, 19, &Menu::onGfHigh, enumStrGFHigh};
EnumMenuItem menuGFHigh(&minfoGFHigh, 0, &menuDecoStepSize);

const char enumStrGFLow_0[]
PROGMEM = "5";
const char enumStrGFLow_1[]
PROGMEM = "10";
const char enumStrGFLow_2[]
PROGMEM = "15";
const char enumStrGFLow_3[]
PROGMEM = "20";
const char enumStrGFLow_4[]
PROGMEM = "25";
const char enumStrGFLow_5[]
PROGMEM = "30";
const char enumStrGFLow_6[]
PROGMEM = "35";
const char enumStrGFLow_7[]
PROGMEM = "40";
const char enumStrGFLow_8[]
PROGMEM = "45";
const char enumStrGFLow_9[]
PROGMEM = "50";
const char enumStrGFLow_10[]
PROGMEM = "55";
const char enumStrGFLow_11[]
PROGMEM = "60";
const char enumStrGFLow_12[]
PROGMEM = "65";
const char enumStrGFLow_13[]
PROGMEM = "70";
const char enumStrGFLow_14[]
PROGMEM = "75";
const char enumStrGFLow_15[]
PROGMEM = "80";
const char enumStrGFLow_16[]
PROGMEM = "85";
const char enumStrGFLow_17[]
PROGMEM = "90";
const char enumStrGFLow_18[]
PROGMEM = "95";
const char enumStrGFLow_19[]
PROGMEM = "100";
const char *const enumStrGFLow[]
PROGMEM = {enumStrGFLow_0, enumStrGFLow_1, enumStrGFLow_2, enumStrGFLow_3, enumStrGFLow_4, enumStrGFLow_5, enumStrGFLow_6, enumStrGFLow_7, enumStrGFLow_8, enumStrGFLow_9, enumStrGFLow_10, enumStrGFLow_11,
           enumStrGFLow_12, enumStrGFLow_13, enumStrGFLow_14, enumStrGFLow_15, enumStrGFLow_16, enumStrGFLow_17, enumStrGFLow_18, enumStrGFLow_19};
const EnumMenuInfo PROGMEM
minfoGFLow = {"GF Low", 8, 0xFFFF, 19, &Menu::onGfLow, enumStrGFLow};
EnumMenuItem menuGFLow(&minfoGFLow, 0, &menuGFHigh);

const char enumStrAlgorithm_0[]
PROGMEM = "ZHL16B";
const char enumStrAlgorithm_1[]
PROGMEM = "ZHL16B";
const char *const enumStrAlgorithm[]
PROGMEM = {enumStrAlgorithm_0, enumStrAlgorithm_1};
const EnumMenuInfo PROGMEM
minfoAlgorithm = {"Algorithm", 4, 0xFFFF, 1, &Menu::onAlgorithm, enumStrAlgorithm};
EnumMenuItem menuAlgorithm(&minfoAlgorithm, 0, &menuGFLow);

RENDERING_CALLBACK_NAME_INVOKE(fnTimeRtCall, timeItemRenderFn,
"Time", -1, &Menu::onTime)
TimeFormattedMenuItem menuTime(fnTimeRtCall, 2, (MultiEditWireType) EDITMODE_TIME_24H, &menuAlgorithm);

RENDERING_CALLBACK_NAME_INVOKE(fnDateRtCall, dateItemRenderFn,
"Date", -1, &Menu::onDate)
DateFormattedMenuItem menuDate(fnDateRtCall, 1, &menuTime);
#endif //DIVEC_TCMENU_H
