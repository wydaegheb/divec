# divec
# Description
Software for my own dive computer using Buhlmann ZHL A/B/C algorithm with Gradient Factors (can be extended later to VPM) 
- I hope to evolve it one day in a CCR computer (not a controller but with cell measurements so it can be used as backup with manual control when my main controller computer breaks)

# Hardware
- Adafruit M4 Express (https://www.adafruit.com/product/3857 22$)
- Adafruit TFT featherwing (https://www.adafruit.com/product/3315 29.95$) - has a built in SD card slot!
- Adafruit Bluefruit LE SPI Friend - Bluetooth Low Energy (BLE) (https://www.adafruit.com/product/2633 17.50$)
- AliExpress piezo buttons - 2x - (https://nl.aliexpress.com/item/33006671392.html?spm=a2g0s.9042311.0.0.2bd84c4dFkYtQg 2,60 x 2 = 5.2 €)
- Banggood DS3231 real time clock (got 3 only need 1), (https://www.banggood.com/3pcs-DS3231-AT24C32-IIC-Precision-RTC-Real-Time-Clock-Memory-Module-p-1559315.html?rmmds=myorder&cur_warehouse=CN 4,10€/3 = 1.4€)
- MS5837-30ba pressure sensor.
  * technical sheet: https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5837-30BA%7FB1%7Fpdf%7FEnglish%7FENG_DS_MS5837-30BA_B1.pdf%7FCAT-BLPS0017
  * can be obtained in a "bulky" form from Blue Robotics: https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar30-sensor-r1/
  * AliExpress/Bangood CLAIM to sell them: e.g. https://nl.aliexpress.com/item/4000090014349.html?spm=a2g0o.productlist.0.0.49e11238R9cSl5&algo_pvid=3aee48c0-6f59-4f51-a77a-bed15220024e&algo_expid=3aee48c0-6f59-4f51-a77a-bed15220024e-1&btsid=0bb0624316048668410076729e0762&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_
(14 €) BUT beware even when it says 30BA it is in fact the 02BA (max depth 12 meter)
  * I ordered the correct standalone sensor from: https://www.digikey.be/ordering/shoppingcart 7,15 € and replaced the sensor on the aliexpress/banggood board with the right one (very fine soldering for a beginner like me!)

Total price of the parts is therefore just shy of 100 $/€

Still working on the housing (I have a home built CNC and a 3D printer and I am thinking about cnc-en the external housing from Delrin (POM) and 3D printing the "inner housing" and fill it up with epoxy.

# Software

Generic remark: available source code online is mainly for "deco planners" (DiveInno being the only exception). Many questions remain therefore "open".
- how to determine when dive starts/stops?
- what to take as surface pressure/surface interval?
- when do we log a dive?
- what is the "onset" of the low gradient factor, (lowest deepstop at the end of the dive or encountered during the dive? when we hit the first deepstop? last dive segment? ...)
- warnings for not being on the right gas. 
- adding gas switch stops, 
- what if the diver violates a stop?
- how to know when tissues are reset? (clock reset, out of battery, deco violation, ...)
- is "extra" safety needed on top of the gradient factors? (e.g. OSTC speeds up and slow down time while on/off gassing. others consider the diver to be one meter lower than he/she really is, ...)

**-> CONCLUSION: DO NOT TRUST YOUR LIFE TO THIS SOFTWARE!!!**

Development environment: Clion with PlatformIO.

Algorithm heavily based on:

1. Erik C. Baker - (fortran - original gradientfactor source code) 
http://uwexplorers.net/downloads/Baker/Gradient%20Factor%20Deco%20Program.pdf (an automatic translation to C# can be downloaded here: https://www.scubaboard.com/community/attachments/decocalc-zip.61302/)

2. Archisgore (Javascript).
github - https://github.com/archisgore/online-deco-console
source - https://github.com/archisgore/online-deco-console/blob/master/src/scuba-dive.js
gui (can be tried directly in a browser!) - https://deco-planner.archisgore.com/

3. DiveInno - dive computer project (including c++ source code) 
homepage - http://www.diveino.hu/
github - https://github.com/kornel-schrenk/DiveIno

3. DecoTengu (Python)
code: https://files.pythonhosted.org/packages/b2/ca/8ceb72cbf9082152f14496da0f5f7508353258c944aa1b8bc50a96621e9f/decotengu-0.14.1.tar.gz
 
Further background
https://thetheoreticaldiver.org/wordpress/index.php/tag/buhlmann/
https://thetheoreticaldiver.org/wordpress/index.php/2018/07/23/can-we-calculate-no-fly-times/


The Bluetooth app (Android) was forked from: https://github.com/adafruit/Bluefruit_LE_Connect_Android_V2/find/master