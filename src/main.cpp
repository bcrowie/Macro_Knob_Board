#include <Arduino.h>
#include <HID-Project.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Encoder
#define inputCLK 1
#define inputDT 2
#define inputSW 3

// Buttons
#define buttonOne 8
#define buttonTwo 6
#define buttonThree 4
#define buttonFour 10
#define buttonFive 7
#define buttonSix 5

// Display
#define screenHeight 64
#define screenWidth 128
#define oledReset -1

Adafruit_SSD1306 oled(screenWidth, screenHeight, &Wire, oledReset);

int currentStateCLK;
int previousStateCLK;
int mode = 1;
int maxModes = 4;

int buttonOneState = 0;
int buttonTwoState = 0;
int buttonThreeState = 0;
int buttonFourState = 0;
int buttonFiveState = 0;
int buttonSixState = 0;

void changeMode() {
    mode = (mode % maxModes) + 1;
}

void keyboardPress(size_t command) {
  Keyboard.press(command);
}

void rotateLeft() {
  switch(mode) {
    case 1:
      // Increase the volume.
      Consumer.write(MEDIA_VOLUME_DOWN);
      break;
    case 2:
      // Application Switcher Reverse
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_TAB);

      delay(100);
      Keyboard.release(KEY_TAB);
      break;
    case 3:
      // Browser Tab Control
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 4:
      // Zoom
      break;
  }
}

void rotateRight() {
  switch(mode) {
    case 1:
      // Decrease the volume.
      Consumer.write(MEDIA_VOLUME_UP);
      break;
    case 2:
      // Application Switcher Forward
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_TAB);

      delay(100);
      Keyboard.release(KEY_TAB);

      break;
    case 3:
      // Browser Tab Forward
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 4:
      // Photoshop Brush size
      Keyboard.press(']');
      break;
  }
}

void setup() {
  pinMode (inputCLK, INPUT);
  pinMode (inputDT, INPUT);
  pinMode (inputSW, INPUT);
  pinMode (buttonOne, INPUT);
  pinMode (buttonTwo, INPUT);
  pinMode (buttonThree, INPUT);
  pinMode (buttonFour, INPUT);
  pinMode (buttonFive, INPUT);
  pinMode (buttonSix, INPUT);

  Serial.begin (9600);
  Consumer.begin();
  previousStateCLK = digitalRead(inputCLK);

  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x78)) {
    Serial.println(F("SSD1306 Allocation Failed"));
    while(true);
  }

  delay(2000);
  oled.clearDisplay();

  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(0, 10);
  oled.println("Welcome");
  oled.display();
}

void loop() {
	currentStateCLK = digitalRead(inputCLK);

	if (currentStateCLK != previousStateCLK){
		if (digitalRead(inputDT) != currentStateCLK) {
			rotateRight();
		} else {
			rotateLeft();
		}
	}

	previousStateCLK = currentStateCLK;

        if(!digitalRead(inputSW)) {
            changeMode();
            delay(300);
        }

  buttonOneState = digitalRead(buttonOne);
  buttonTwoState = digitalRead(buttonTwo);
  buttonThreeState = digitalRead(buttonThree);
  buttonFourState = digitalRead(buttonFour);
  buttonFiveState = digitalRead(buttonFive);
  buttonSixState = digitalRead(buttonSix);

  switch (mode)
  {
  case 1:
    if (buttonOneState == HIGH) {
      Consumer.write(MEDIA_PREVIOUS);
    }

    if (buttonTwoState == HIGH) {
      Consumer.write(MEDIA_PLAY_PAUSE);
    }

    if (buttonThreeState == HIGH) {
      Consumer.write(MEDIA_NEXT);
    }
    break;
  case 2:
    if (buttonSixState == HIGH) {
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }
  break;
  default:
    break;
  }
}
