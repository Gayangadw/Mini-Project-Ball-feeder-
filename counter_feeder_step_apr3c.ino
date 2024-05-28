#include <Stepper.h>
#include <LiquidCrystal_I2C.h>

const int stepsPerRevolution = 2048;  


#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17


int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);


Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear(); 
  lcd.print("Ball Feeder"); 
  delay(5000); 
  
  myStepper.setSpeed(5);
  
  Serial.begin(115200);
}

void loop() {
 
  for (int i = 0; i < 12; i++) {
    
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Ball count:");
    lcd.setCursor(0, 1);
    lcd.print(i + 1); 
    delay(1000);
    Serial.print("Rotation count: ");
    Serial.println(i + 1);
    myStepper.step(stepsPerRevolution);
    delay(1000);
  }

  while (true) {
    
  }
}
