#include <Arduino.h>
#include <HID-Project.h>

#define inputCLK 1
#define inputDT 2
#define inputSW 3

int currentStateCLK;
int previousStateCLK;
int mode = 1;
int maxModes = 4;

void changeMode() {
    mode = (mode % maxModes) + 1;
}

void rotateLeft() {
  switch(mode) {
    case 1:
      // Increase the volume.
      Consumer.write(MEDIA_VOLUME_UP);
      break;
    case 2:
      //Cltr + TAB
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 3:
      // Lightroom, PowerPoint
      Keyboard.press(KEY_UP_ARROW);
      break;
    case 4:
      // Photoshop Brush size
      Keyboard.press('&#91;');
      break;
  }
}

void rotateRight() {
  switch(mode) {
    case 1:
      // Decrease the volume.
      Consumer.write(MEDIA_VOLUME_DOWN);
      break;
    case 2:
      //Cltr + SHIFT + TAB
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_TAB);
      Keyboard.releaseAll();
      break;
    case 3:
      // Lightroom, PowerPoint
      Keyboard.press(KEY_DOWN_ARROW);
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

  Serial.begin (9600);

  Consumer.begin();

  previousStateCLK = digitalRead(inputCLK);
}

void loop() {
	// Read the current state of inputCLK
	currentStateCLK = digitalRead(inputCLK);

	// If the previous and the current state of the inputCLK are different then a pulse has occured
	if (currentStateCLK != previousStateCLK){
		// If the inputDT state is different than the inputCLK state then
		// the encoder is rotating counterclockwise
		if (digitalRead(inputDT) != currentStateCLK) {
			rotateRight();
		} else {
			rotateLeft();
		}
	}
	// Update previousStateCLK with the current state
	previousStateCLK = currentStateCLK;

        if(!digitalRead(inputSW)) {
            changeMode();
            delay(300);
        }
}
