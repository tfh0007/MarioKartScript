#include <NintendoSwitchControlLibrary.h>
char userInput;
void setup()
{
    Serial.begin(9600); // sets up serial
    pinMode(LED_BUILTIN, OUTPUT);
    pushButton(Button::B, 500, 5); //turns on the controller by pressing B(any button works)
}


void loop(){
    pushButton(Button::X, 600);
    pushHat(Hat::RIGHT, 200);
    pushHat(Hat::DOWN, 200);
    pushHat(Hat::LEFT, 200);
    pushHat(Hat::UP, 200);
    pushButton(Button::X, 600);
    delay(1000);
}
