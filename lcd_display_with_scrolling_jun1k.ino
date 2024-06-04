#include <esp_now.h>
#include <WiFi.h>
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


const int lightME = 15;


struct __attribute__((packed)) dataPacket {
  int state;
};


volatile int lightState = LOW;


void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  dataPacket packet;
  memcpy(&packet, incomingData, sizeof(packet));
  Serial.print("button: ");
  Serial.println(packet.state);
  lightState = packet.state;
  digitalWrite(lightME, lightState);
}

void setup() {
  
  Serial.begin(115200);

  
  pinMode(lightME, OUTPUT);

  
  WiFi.mode(WIFI_STA);

  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  esp_now_register_recv_cb(OnDataRecv);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  
  myStepper.setSpeed(5);
}

void scrollMessage(String message, int delayTime) {
  int messageLength = message.length();
  for (int i = messageLength + lcdColumns; i >= 0; i--) {
    lcd.clear();
    if (i < messageLength) {
      
      lcd.setCursor(i < lcdColumns ? 0 : i - lcdColumns, 0);
      lcd.print(message.substring(i, min(i + lcdColumns, messageLength)));
    }
    if (i + lcdColumns < messageLength) {
      
      lcd.setCursor(0, 1);
      lcd.print(message.substring(i + lcdColumns, min(i + 2 * lcdColumns, messageLength)));
    }
    delay(delayTime);
  }
}

void loop() {
  if (lightState == HIGH) {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Feeding starting");

    
    delay(6000);

  
    for (int i = 0; i < 12; i++) {
      if (lightState == LOW) break; 
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

    
    lcd.clear();
    scrollMessage("Ball Feeder", 250);

    
    while (lightState == HIGH) {
      delay(100);
    }
  } else {
    
    scrollMessage("Ball Feeder", 250);
  }
  delay(100);
}
