#include <Adafruit_GFX.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define MAX_POSITIONS_1 2
#define MAX_POSITIONS_2 5

int read = 0, position = 0, menuNumber = 0, inputBuffer[10] = {0}, maxPositions,
    pageStart = 0;

Adafruit_SSD1306 display(128, 32, &Wire, -1);

// Generic arithmetic funtion
typedef void (*UpdateFunction)();
UpdateFunction UpdateDisplay;

void DisplayMenu1();
void DisplayMenu2();

void setup() {
    Serial.begin(9600);
    pinMode(2, INPUT_PULLUP);
    pinMode(3, INPUT_PULLUP);
    pinMode(4, INPUT_PULLUP);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Address 0x3C for 128x32
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.display();
    UpdateDisplay = DisplayMenu1;
    maxPositions = MAX_POSITIONS_1;
    UpdateDisplay();
}

void loop() {
    if (!digitalRead(2)) {
        position = (position - 1 + maxPositions) % maxPositions;
        read++;
    }
    if (!digitalRead(3)) {
        read++;
        if (menuNumber == 0) {
            if (position == 0) {
                menuNumber = 1;
                maxPositions = MAX_POSITIONS_2;
                UpdateDisplay = DisplayMenu2;
                position = 0;
            }
        } else if (menuNumber == 1) {
            if (position == 4) {
                menuNumber = 0;
                maxPositions = MAX_POSITIONS_1;
                UpdateDisplay = DisplayMenu1;
                position = 0;
            }
        }
    }
    if (!digitalRead(4)) {
        position = (position + 1) % maxPositions;
        read++;
    }
    if (read > 0) {
        UpdateDisplay();
        read--;
    }
    delay(250);
}

void DisplayMenu1() {
    display.clearDisplay();
    display.setCursor(10, 0);
    display.print("Automatico");
    display.setCursor(10, 8);
    display.print("Manual");
    display.setCursor(0, position * 8);
    display.print(">");
    display.display();
}

void DisplayMenu2() {
    display.clearDisplay();
    String opcoes[5] = {"Temp 1", "Timer 1", "Temp 2", "Timer 2", "Voltar"};
    int start = position > 3 ? 1 : 0;
    if (position > pageStart + maxPositions) {
        pageStart = position
    }
    Serial.print("Posição: ");
    Serial.println(position);
    Serial.print("Inicio: ");
    Serial.println(start);
    display.setCursor(0, (position - start) * 8);
    display.print(">");
    for (int pos = 0; pos < 4; pos++, start++) {
        display.setCursor(10, pos * 8);
        display.print(opcoes[start]);
    }
    display.display();
}
