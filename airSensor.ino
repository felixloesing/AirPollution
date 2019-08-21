

//dust sensor
int pin = 8;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

String dataStringDust1 = "";
String dataStringDust2 = "";

//+++++++++Temp & Hum sensor
#include "DHT.h"

#define DHTPIN A0     // what pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE);

String dataStringTempHum = "";

//+++++++++Gas sensor
#include <Wire.h>
#include "MutichannelGasSensor.h"

String dataStringGas = "";

//+++++++++SD card datalogger
#include <SPI.h>
#include <SD.h>

const int chipSelect = 9;

//power
byte powerPin = 5;

void setup() {
  // put your setup code here, to run once:
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);

  Serial.begin(9600);
  /*
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  */
  //dust sensor
  pinMode(8,INPUT);
  starttime = millis();

  //Temp & Hum sensor
  dht.begin();

  //Gas sensor
  Serial.println("power on!");

  mutichannelGasSensor.begin(0x04);//the default I2C address of the slave is 0x04
  //mutichannelGasSensor.changeI2cAddr(0x10);
  //mutichannelGasSensor.doCalibrate();

  mutichannelGasSensor.powerOn();

  //+++++++ SD card
    Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  digitalWrite(powerPin, HIGH);
  mutichannelGasSensor.powerOff();
}

String measureDust() {

  String dust = ""; 

  starttime = millis();


  while ((millis()-starttime) < sampletime_ms) {
    duration = pulseIn(pin, LOW);
    lowpulseoccupancy = lowpulseoccupancy+duration;
  }
  
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=&gt;100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

    dust += "\n";
    dust += "c = ";
    dust += concentration;
    dust += " pcs/0.01cf";
    Serial.println(dust);
    lowpulseoccupancy = 0;
    starttime = millis();
    return(dust);
}

void measureTempHum() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(t) || isnan(h)) 
    {
        Serial.println("Failed to read from DHT");
    } 
    else 
    {
        dataStringTempHum += "H: ";
        dataStringTempHum += h;
        dataStringTempHum += " %\t";
        dataStringTempHum += "T: ";
        dataStringTempHum += t;
        dataStringTempHum += " *C";
        Serial.println(dataStringTempHum);
    }
}

void measureGas() {
    float c;
    
    c = mutichannelGasSensor.measure_NH3();
    dataStringGas += "NH3 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    c = mutichannelGasSensor.measure_CO();
    dataStringGas += "CO is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    c = mutichannelGasSensor.measure_NO2();
    dataStringGas += "NO2 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    c = mutichannelGasSensor.measure_C3H8();
    dataStringGas += "C3H8 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    c = mutichannelGasSensor.measure_C4H10();
    dataStringGas += "C4H10 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    c = mutichannelGasSensor.measure_CH4();
    dataStringGas += "CH4 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n"; 

    c = mutichannelGasSensor.measure_H2();
    dataStringGas += "H2 is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n"; 

    c = mutichannelGasSensor.measure_C2H5OH();
    dataStringGas += "C2H5OH is ";
    if(c>=0) dataStringGas += c;
    else dataStringGas += "invalid";
    dataStringGas += " ppm";
    dataStringGas += "\n";

    Serial.println(dataStringGas);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("new loop");
  delay(1560000); //26min
  digitalWrite(powerPin, LOW);
  mutichannelGasSensor.powerOn();
  delay(180000); //3min
  dataStringDust1 = measureDust(); //takes approx 30 sec
  measureTempHum();
  measureGas();
  delay(1000);
  mutichannelGasSensor.powerOff();
  dataStringDust2 = measureDust(); //takes approx 30 sec
  delay(1000);
  digitalWrite(powerPin, HIGH);
  delay(1000);
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataStringDust1);
    dataFile.println(dataStringTempHum);
    dataFile.println(dataStringGas);
    dataFile.println(dataStringDust2);
    dataFile.println("saved to sd card");
    Serial.println("saved to sd card");
    dataFile.close();

  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  
  dataStringDust1 = "";
  dataStringDust2 = "";
  dataStringTempHum = "";
  dataStringGas = "";
}
