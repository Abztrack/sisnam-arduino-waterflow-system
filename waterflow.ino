#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 20, 4);
double flow; // Water flow L/Min 
double volume; // Total volume L
double pricePerLiter = 1000; // Harga per liter
int flowsensor = 2;
unsigned long currentTime;
unsigned long lastTime;
unsigned long pulse_freq;
unsigned long lastSaveTime = 0;
const unsigned long saveInterval = 15000; // menyimpan data setiap 15 detik
const unsigned long resetInterval = 604800000; // reset volume setiap satu minggu sekali (in milliseconds)

void pulse() // Interrupt function
{
  pulse_freq++;
}

void setup() {
  pinMode(flowsensor, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, pulse, RISING);
  currentTime = millis();
  lastTime = currentTime;
  lcd.init();
  rtc.begin();
  SD.begin(4);

  lcd.backlight();
  lcd.clear();
  lcd.print("Flow: ");
  lcd.setCursor(11, 0);
  lcd.print("L/min");
  lcd.setCursor(0, 1);
  lcd.print("Volume: ");
  lcd.setCursor(12, 1);
  lcd.print("L");
  lcd.setCursor(0, 2);
  lcd.print("Bayar: Rp");
}

void loop() {
  if (!SD.begin(4)) {
    Serial.println("Error initializing SD card!");
    return;
  }
  currentTime = millis();

  // Check if it's time to reset the volume
  if (currentTime - lastSaveTime >= resetInterval)
  {
    volume = 0; // Reset volume after one week
    lastSaveTime = currentTime;
  }

  if (currentTime >= (lastTime + 1000))
  {
    lastTime = currentTime;
    flow = (pulse_freq / 7.5) * 0.98;
    pulse_freq = 0;
    volume += flow / 60.0;
    Serial.print(flow, DEC);
    Serial.print(" L/Min | Volume: ");
    Serial.print(volume, 2);
    Serial.println(" L");
  }

  DateTime now = rtc.now();
  lcd.clear();
  lcd.setCursor(0, 3);
  lcd.print(now.hour());
  lcd.print(':');
  lcd.print(now.minute());
  lcd.print(':');
  lcd.print(now.second());
  lcd.print(" ");
  lcd.print(" ");

  lcd.setCursor(10, 3);
  lcd.print(now.year());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.day());

  lcd.setCursor(6, 0);
  lcd.print(flow, 3);

  lcd.setCursor(8, 1);
  lcd.print(volume, 2);

  double totalPayment = volume * pricePerLiter;
  lcd.setCursor(9, 2);
  lcd.print(totalPayment, 2);
  lcd.print(" ");

  lcd.setCursor(0, 0);
  lcd.print("Flow: ");
  lcd.setCursor(11, 0);
  lcd.print("L/min");
  lcd.setCursor(0, 1);
  lcd.print("Volume: ");
  lcd.setCursor(12, 1);
  lcd.print("L");
  lcd.setCursor(0, 2);
  lcd.print("Bayar: Rp");

  // Save data to SD card
  if (currentTime - lastSaveTime >= saveInterval)
  {
    saveDataToSD(now, volume, pricePerLiter);
    lastSaveTime = currentTime;
  }

  delay(1000);
}

void saveDataToSD(DateTime now, double volume, double pricePerLiter)
{
  char fileName[15];
  sprintf(fileName, "%04d%02d%02d.csv", now.year(), now.month(), now.day());

  File myFile = SD.open(fileName, FILE_WRITE);

  if (myFile)
  {
    if (myFile.size() == 0)
    {
      myFile.println("Tanggal Waktu,Volume (L),Total Pembayaran (Rp)");
    }

    myFile.print(now.year());
    myFile.print('/');
    myFile.print(now.month());
    myFile.print('/');
    myFile.print(now.day());
    myFile.print(" ");
    myFile.print(now.hour());
    myFile.print(':');
    myFile.print(now.minute());
    myFile.print(':');
    myFile.print(now.second());
    myFile.print(", ");
    myFile.print(volume, 2);
    myFile.print(", ");
    myFile.print(volume * pricePerLiter, 2);
    myFile.println();

    myFile.close();
    Serial.println("Data saved to " + String(fileName));
  }
  else
  {
    Serial.println("Error opening " + String(fileName));
  }
}
