# AirPollution
Arduino Air Pollution Measurement Station

![sensor](/sensor.jpg?raw=true "Sensor")

After taking an Environmental Science college class I was inspired to use citizen sensing to uncover problems in my hometown. I hoped uncovering those would get them fixed quickly, which would make life for people in my city better.
The biggest problem, which came to my mind immediately was the problem of dirty air caused by too many cars on the roads – especially diesel vehicles. The city of Duesseldorf is growing, there are more cars on the road than ever, but missing measurement stations make it hard to proof how much particle matter and toxic gasses are in the air.
This is why I started planning my own mobile measurement station, which would create reliable data everywhere. I envisioned it to be powered by a portable battery to make it independent from the electrical grid and to measure a wide variety of gasses.
I decided, that it is especially important to me to measure nitrogen dioxide because it is very harmful for humans and the environment, but is also very common in our air because cars pollute lots of it. I researched sensors, which would measure this gas and a few more, and also a sensor which measure pollution caused by cars. I also need a temperature and humidity sensor because a change in both has influence on how to calibrate the sensor.
I am using an Arduino Nano to control all the sensors, control the power supply and calculate the concentration of the gasses in air. I connected a SD card slot to the Arduino, which will log all the captured data as a text file to the SD card.
I created a highly flexible setup which I can use everywhere I want and gives me accurate results.
