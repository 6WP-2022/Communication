#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
const int chipSelect = 10;
float firsttime;
int i = 1 ;
//int x = 0;


#include <Arduino_LSM9DS1.h>
//float x, y, z;
//float M;
StaticJsonDocument<20000> doc;


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  // see if the card is present and can be initialized:
  //pinMode(10,OUTPUT);
  pinMode(chipSelect, OUTPUT);
  //digitalWrite(chipSelect, HIGH);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
  firsttime = millis();

}


void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  float  times = millis();
  float x, y, z;
  float M = sqrt(x * x + y * y + z * z);
  String dataString = "";
  float currenttime = (times - firsttime) / 1000;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    /* Serial.print(x);
      Serial.print('\t');
      Serial.print(y);
      Serial.print('\t');
      Serial.print(z);
      Serial.print('\t');
    */
    //  Serial.println(M);
  }
  String Data("Data");
  Data += String(i);
  Serial.print(Data);
  //時間とxyzからM加速度取得

  StaticJsonDocument<200> obj;
  obj["Time"] = currenttime;
  obj["Accel"] = M;
 // serializeJson(obj, Serial);
  doc[Data] = obj;
  //  StaticJsonDocument<200> doc;
  //  JsonObject obj = doc.createNestedObject(Data);
  //  obj["Time"] = currenttime;
  //  obj["Accel"] = M;
  //  doc[data]=obj;
  serializeJson(doc, Serial);
  
  Serial.println("");
  //  getaccel();
  // dataString += String(doc);
  // dataString += ",";
  // dataString += String(M);
  /* dataString += ",";
    dataString += String(y);
    dataString += ",";
    dataString += String(z);
  */
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //SD.remove("test.txt");

  File dataFile = SD.open("test.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {

    dataFile.available();
    serializeJson(doc, dataFile);
    dataFile.println("");
    dataFile.close();

    /*    Serial.print(times);
        Srial.print(",");
        Serial.println(M);
    */
    // x = x + 1;
    // delay(50);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");

  }
  i = i + 1;
}

float getaccel() {
  float x, y, z;
  float M;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    M = sqrt(x * x + y * y + z * z);
  }

  return M;
}
