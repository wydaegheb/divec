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
  * After several failures in getting the right version from Ali Express (I always got the 2 bar version instead of the 30 bar even when it clearly stated that it was the 30 bar version)
    I ordered the following sensor from Ali Express after reading the tips from Damien Siviero on Facebook (CCR custom builders)
    This is the correct version
    Tips:
    1. Be super gentle, flat push insert only. Twist and you break the sensor.
    2. The tiny orings are common for watch crown seals, 3.6x1.7x0.8mm fits nicely. … Meer weergeven
    US $10.75 8% OFF|1pcs GY MS5837 02BA 30BA High Precision Gas Liquid Waterproof Pressure Sensor Module|Integrated Circuits| - AliExpress
    https://l.facebook.com/l.php?u=https%3A%2F%2Fwww.aliexpress.com%2Fitem%2F33053533496.html%3Fspm%3Da2g0o.productlist.0.0.1e526f11lvHxKL%26algo_pvid%3Dcf88aa25-5d39-4089-a435-d474402eaa22%26algo_expid%3Dcf88aa25-5d39-4089-a435-d474402eaa22-9%26btsid%3D0b0a555c16072441409755296e1b72%26ws_ab_test%3Dsearchweb0_0%252Csearchweb201602_%252Csearchweb201603_%26fbclid%3DIwAR2_En5OnTAFz1-E-Kj1jDiq46TaQEaeLJBpWRg_znSUolYqYtpPiXtMK9I&h=AT3I5KasDn15Y9scs_qPzXhQdkdxlQuTLAxG3Q8PxK9cfM9YXb-7-NuFdJS9rY6cxD-7hHLRtmm4TJLzl3v5uRim8Qjo5eEcSwk9tqqXfxsLf18sc7H7mBhSuaZkxwtd8peLffEBdqSdRT6rwnChu71I3PXW2Q&__tn__=R]-R&c[0]=AT2xw-07sfkdsU6QnTeylD063FRV6I4EIfuiJqon8zmGhtNPT0we9DXLuNJUjltbBPkKZWOH7NtJewQsWjFgFZ0OXDL7Ob6LEg0UNiJxzdUNHZBBqSmxlhYF3tedTndu27qqV0NRZYkgrutUC278FpABH0bcIDyIqkJpTGLGksjSvi3qJMHQhmfxFZtSkOcabRKEkVVS9AASBrLQlw

  * technical sheet of this sensor: https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FMS5837-30BA%7FB1%7Fpdf%7FEnglish%7FENG_DS_MS5837-30BA_B1.pdf%7FCAT-BLPS0017
  * can also be obtained in a "bulky" form from Blue Robotics: https://bluerobotics.com/store/sensors-sonars-cameras/sensors/bar30-sensor-r1/
  * The sensor can also be ordered without PCB (beware it is very small!) from: https://www.digikey.be/ordering/shoppingcart 7,15 € and replaced the sensor on the aliexpress/banggood board with the right one (very fine soldering for a beginner like me!)

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
