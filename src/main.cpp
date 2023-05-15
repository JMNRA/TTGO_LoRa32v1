#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LoRa32 v1 OLED PINS
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16

// LoRa32 v1(SX1276) LoRa PINS
#define LORA_SCK 5 // LORA_SCLK_PIN
#define LORA_MISO 19
#define LORA_MOSI 27
#define LORA_CS 18 // LORA_NSS
#define LORA_DIO0 26
#define LORA_RST 14
#define LORA_DIO1 33
#define LORA_BUSY 32

// #define LORA_IRQ 26 // LORA_DI0

SX1276 radio = new Module(LORA_CS, LORA_DIO0, LORA_RST, LORA_BUSY);
// put function declarations here:

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RST);

void initializeDisplay()
{

  Serial.println("Inicializando display...");
  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0X3c))
  {
    Serial.println("Fallo al iniciar el display");
    for (;;)
      ;
  }

  Serial.println("Display inicializado!!!");
  display.clearDisplay();

  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Welcome to LoRa");

  display.setTextSize(1);
  display.println("Lora Sender");
  display.display();
}

void initLoRa()
{
  Serial.println("Inicializando LoRa...");
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  int state = radio.begin();
  radio.setFrequency(915);
  radio.setSpreadingFactor(12);
  if (state == RADIOLIB_ERR_NONE)
  {
    Serial.println(F("EXITO!"));
  }
  else
  {
    display.println("ERROR :(");
    Serial.println(F("ERROR , CODE : "));
    Serial.println(state);
    for (;;)
      ;
  }
}
// int counter = 0;
void holamundolora()
{
  Serial.print(F("NODO 1 - TRANSMITIENDO PACKETE ... "));

  int state = radio.transmit("Te amo oso");
  if (state == RADIOLIB_ERR_NONE)
  {
    // counter++;
    // Si el paquete se transmitio correctamente
    // display.clearDisplay();
    // display.print("Paquete enviado!!");
    // display.print(counter);
    // display.display();

    // display.clearDisplay();

    Serial.print(F("NODO 1 - Datarate:\t"));
    Serial.print(radio.getDataRate());
    Serial.println(" bps");
  }
  else if (state == RADIOLIB_ERR_PACKET_TOO_LONG)
  {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));
    display.print("too long!");
  }
  else if (state == RADIOLIB_ERR_TX_TIMEOUT)
  {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));
    display.print("Timeout");
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

  // wait for a second before transmitting again
  delay(1000);
}

void resetDisplay()
{
  digitalWrite(OLED_RST, LOW);
  delay(25);
  digitalWrite(OLED_RST, HIGH);
}

void setup()
{
  Serial.begin(115200);
  initializeDisplay();
  initLoRa();
}

void loop()
{
  holamundolora();
}
