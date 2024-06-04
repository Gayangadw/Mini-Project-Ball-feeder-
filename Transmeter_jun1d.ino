#include <esp_now.h>
#include <WiFi.h>

const int pushDown = 26;


uint8_t broadcastAddress[] = {0xE0, 0x5A, 0x1B, 0x6C, 0x67, 0x4C}; // E0:5A:1B:6C:67:4C

struct __attribute__((packed)) dataPacket {
  int state;
};

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: ");
  Serial.println(macStr);
  Serial.print("Last Packet Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  pinMode(pushDown, INPUT_PULLUP);

 
  Serial.begin(115200);

  
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  
  esp_now_register_send_cb(OnDataSent);

  
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  dataPacket packet;
  packet.state = digitalRead(pushDown);

  esp_now_send(broadcastAddress, (uint8_t *) &packet, sizeof(packet));

  delay(30);
}
