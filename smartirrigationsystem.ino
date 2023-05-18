// imported libraries for LCD Display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2); 
//Soil Sensor
#define limit 450 
// initializing the LEDs and Buzzer, PIN numbers
int yellow = 8;
int blue = 7;
int red = 11;
int buzzer = 9;
int pump = 13; //initialises the water pump to pin 13

// Water Sensor
#define waterLow 400  //After testing, the range was determined
#define waterGood 370  

void setup() {
  pinMode(red, OUTPUT);
  pinMode(pump, OUTPUT); //Water pump 
  pinMode(buzzer,OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  lcd.init(); //set up for the LCD screen
  lcd.backlight();
  lcd.clear();
  // Establishing communication with the code and the serial monitor
  Serial.begin(9600);
}

void loop() {
  //creating a variable called moisture to give the values recieved from the sensor to
  int moisture = analogRead(A1); //reads water sensor values from pin A1
  Serial.print("Water Level: "); //displays on the serial monitor
  Serial.println(moisture); //prints the values from the water sensor
  
  int soilLevel = analogRead(A0); //reads soil sensor values from pin A0
  Serial.print("Soil: ");
  Serial.println(soilLevel);


  //checks if the water level is between the given range, meaning that it is almost empty, giving ample time to refill the tank
  if(moisture >= waterGood && moisture < waterLow) {
    Serial.println("WARNING! CHECK WATER TANK!!");
    noTone(buzzer);
    delay(200);
  }
  // checks if the the water in the water tank is less
  else if(moisture >= waterLow){ //meaning that the water tank would require refilling
    Serial.println("WATER CRITICAL!!!");
    digitalWrite(red, HIGH); //red LED blinks
    delay(100);
    digitalWrite(red, LOW);
    delay(100);
    tone(buzzer,258); //buzzer turns on sigaling the empty tank
    delay(200);
    digitalWrite(red, HIGH);
    delay(100);
    tone(buzzer, 258);
    digitalWrite(red, LOW);
    delay(200);
    digitalWrite(pump, LOW);
  }
  // checks if the the water level in tank is good 
  else if(moisture < waterGood){
    Serial.println("WATER LEVEL PERFECT!");
    delay(200);
    noTone(buzzer);
  }
  else{
    Serial.println("Unable to detect"); //default statement
    delay(200);
  }

  Serial.println("......."); //divider
  
  // checks if the soil moisture level is greater than the limit
  if(soilLevel > limit && moisture < waterGood){ //if the soil level is greater that means the soil is dry
    lcd.setCursor(3,0);
    lcd.print("Soil Dry :("); //displays this on two lines on the LCD screen
    lcd.setCursor(2,1);
    lcd.print("Watering now...");
    Serial.println("Dry Soil");
    digitalWrite(yellow, HIGH);//the yellow LED turns on
    digitalWrite(blue, LOW);
    digitalWrite(pump, HIGH); // as well as the water pump
    delay(5);
  }
  else if(soilLevel > limit){ //if the soil level is greater that means the soil is dry
    lcd.setCursor(3,0);
    lcd.print("Soil Dry :("); //displays this on two lines on the LCD screen
    lcd.setCursor(2,1);
    lcd.print("Tank Empty");
    Serial.println("Dry Soil + Empty Tank");
    digitalWrite(yellow, HIGH);//the yellow LED turns on
    digitalWrite(blue, LOW);
    digitalWrite(pump, LOW); // as well as the water pump
    delay(5);
  }
  // checks if the soil moisture level is lesser than the limit
  else if(soilLevel < limit){
    Serial.println("Good Soil"); //prints on the serial monitor
    digitalWrite(blue, HIGH); //turns the blue LCD on 
    digitalWrite(yellow, LOW);
    lcd.setCursor(3,0);
    lcd.print("Perfect Soil :)"); //displays this one two lines on the LCD
    digitalWrite(pump, LOW);
    lcd.setCursor(2,1);
    lcd.print("Good Job!");
    delay(5);
  }
  else{
    Serial.println("Unable to detect"); //default statement
    delay(5);
  }
}