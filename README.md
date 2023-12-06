# sisnam-arduino-waterflow-system
**Arduino based Waterflow Measure System**<br>
An arduino program that measures the flow of the water flowing into the water sensor (YFS201) then converts it to volumes which will be calculated for the price we have to pay based on the water volumes. The data will then be saved in a microSD card. Also there's a Python script that automatically create the graph from the data saved for analyzing purposes.

**This system has 3 modules and 1 sensor which are:**
1. Real-time clock (RTC) Module
2. MicroSD Reader Writer Module
3. LCD 20x4 I2C Module
4. YFS201 Waterflow sensor

**How it works:**<br>
First, a water will flow through the YFS201 sensor, the sensor sends the output of signal pulse to the microcontroller (Arduino Uno) and then display it on the LCD screen simultaneously calculating the volume and the total payment, every x seconds the microcontroller will then save the data on the screen to the microSD card in a .csv file and Python script is used for analyzing the data.
