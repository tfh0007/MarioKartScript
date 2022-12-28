#include <NintendoSwitchControlLibrary.h>
#include <TM1637.h>
char userInput;

// Joystick setup
const int joystickX = A0;
const int joystickY  = A1;
const int joyStickButtonPress = 1023;
const int joyStickLowThreashold = 300;
const int joyStickHighThreashold = 650;
const int validScriptNums[8] {1, 2, 3, 4, 5, 6, 7, 8};
int lastButtonPress = -1;
int curScriptNum = 1;
int confirmScriptNum = false;
bool waitForInput = true;
bool automatedKartSelectionAlreadyUsed = false;
bool characterAlreadySelected = false;
bool currentlyPlayer1 = false;

// Display Clock setup
const int clock = 2;
const int dioClock = 3;
TM1637 tm(clock, dioClock);

// Buzzer setup
const int buzzer = 3;

const int touchSensor = 4;

/* Joy stick range
  y-axis: 250-775 : stationary: 514
  x-axis: 250-775 : stationary: 508
  x-pushButton: 1023
  y-pushButton: No Change
  Left press will be: x < 300
  Right press will be: x > 650
  Up press will be: y > 650
  Down press will be y < 300

*/

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pushButton(Button::B, 500, 5); //turns on the controller by pressing B(any button works)

  // Display clock setup
  tm.init();
  tm.set(5); //set brightness 0-7

  Serial.begin(9600);
  pinMode(joystickX, INPUT);
  pinMode(joystickY, INPUT);
  pinMode(touchSensor,INPUT);
  pinMode(buzzer, OUTPUT);
  tm.display(3, curScriptNum);
}

void waitForScriptSelection()
{
  //Serial.println("Press a Button:");

  // Set the arduino to wait for input
  while (waitForInput == true) {
    int joyStickXValue = analogRead(joystickX);
    int joyStickYValue = analogRead(joystickY);

    // Button inner press (selection confirmed)
    if (joyStickXValue == joyStickButtonPress)
    {
      // Serial.println("Inner Button Pressed");
      // resetClock();
      tm.display(3, 10);

      // 2000 to 5000 is best frequency for humans
      tone(buzzer, 2000, 1000); // activate the buzzer
      lastButtonPress = 0;
      confirmScriptNum = true;

      //waitForInput = false;
      break;
    }

    // Down button press
    if (joyStickYValue < joyStickLowThreashold && curScriptNum - 1 > 0)
    {
      curScriptNum = curScriptNum - 1;
      tm.display(3, curScriptNum);
      waitForInput = false;
      lastButtonPress = 3;
    }
    // Up button press
    else if (joyStickYValue > joyStickHighThreashold && curScriptNum + 1 < 16)
    {
      curScriptNum = curScriptNum + 1;
      tm.display(3, curScriptNum);
      waitForInput = false;
      lastButtonPress = 4;
    }
    // Touch Sensor press
    int touchSensorBtnState = digitalRead(touchSensor);
    if(touchSensorBtnState==1) {
      // Check if the sensor is being held
      int timer = 0;
      while(digitalRead(touchSensor)== 1) {
        timer++;
        delay(50);
      }
      // held for 600+ miliseconds
      if(timer > 11) {
        pushButton(Button::A, 1);
      }
      // button clicked quickly
      else {
        pushButton(Button::B, 1);
      }
      
    }

    delay(200);

  }

  waitForInput = true;

}

void DisplayForAll(int value) {
  tm.display(0,value);
  tm.display(1,value);
  tm.display(2,value);
  tm.display(3,value);
}

bool checkIfCurSelectionValid()
{
  // Here we need to check for situations such as blue shells only which is invalid
  return true;


}

void DisplayFullTwoDigitNumber(int item)
{
  ClearDisplay();
  
  if (item < 10) {
    tm.display(2, item);
  }
  else if (item < 20)
  {
    tm.display(1, 1);
    tm.display(2, item - 10);
  }
  else if (item < 30)
  {
    tm.display(1, 2);
    tm.display(2, item - 20);

  }
  else if (item < 40)
  {
    tm.display(2, 3);
    tm.display(2, item - 30);

  }
  else if (item < 50)
  {
    tm.display(2, 4);
    tm.display(2, item - 40);

  }
  else if (item < 60)
  {
    tm.display(2, 5);
    tm.display(2, item - 50);

  }
  else if (item < 70)
  {
    tm.display(2, 6);
    tm.display(2, item - 60);

  }
  else if (item < 80)
  {
    tm.display(2, 7);
    tm.display(2, item - 70);

  }
  else if (item < 90)
  {
    tm.display(2, 8);
    tm.display(2, item - 80);

  }
  else if (item < 100)
  {
    tm.display(2, 9);
    tm.display(2, item - 90);

  }


}

void ManualControllerInput()
{
  //delay(2000);
  tm.clearDisplay();
  //Serial.println("Press a Button:");
  lastButtonPress = -1;
  
  // Set the arduino to wait for input

  tone(buzzer, 2000, 2000); // activate the buzzer
  WaitForInput();
}

void WaitForInput() {
  ClearDisplay();
  delay(500);
  tm.display(0,10);
  while (true) {
    int joyStickXValue = analogRead(joystickX);
    int joyStickYValue = analogRead(joystickY);
    int touchSensorBtnState = digitalRead(touchSensor);

    // Touch Sensor press
    if(touchSensorBtnState==1) {
      // Check if the sensor is being held
      int timer = 0;
      while(digitalRead(touchSensor)== 1) {
        timer++;
        delay(50);
      }
      // held for 600+ miliseconds
      if(timer > 11) {
        WaitForAlternateInput();
      }
      // button clicked quickly
      else {
        pushButton(Button::B, 1);
      }
      
    }
    // Button inner press
    if (joyStickXValue > 1000)
    {
      int timer = 0;
      while(joyStickXValue > 1000) {
        delay(50);
        joyStickXValue = analogRead(joystickX);
        timer++;
      }
      // Button was held for 600+ miliseconds
      if(timer > 11) {
        break;
      }
      else {
        pushButton(Button::A, 1);
        delay(50);
      }
    }
    // Left button press
    else if (joyStickXValue < joyStickLowThreashold)
    {
      while(joyStickXValue < joyStickLowThreashold) {
        Serial.println("Left DPad Pressed");
        pushHat(Hat::LEFT, 1);
        delay(50);
        joyStickXValue = analogRead(joystickX);
      }
    }
    // Right button press
    else if (joyStickXValue > joyStickHighThreashold)
    {
      while(joyStickXValue > joyStickHighThreashold) {
        Serial.println("Right DPad Pressed");
        pushHat(Hat::RIGHT, 1);
        delay(50);
        joyStickXValue = analogRead(joystickX);
      }
    }
    // Down button press
    else if (joyStickYValue < joyStickLowThreashold)
    {
      while (joyStickYValue < joyStickLowThreashold)
      {
        pushHat(Hat::DOWN, 1);
        delay(50);
        Serial.println("Down DPad Pressed");
        joyStickYValue = analogRead(joystickY);
      }
    }
    // Up button press
    else if (joyStickYValue > joyStickHighThreashold)
    {
      while (joyStickYValue > joyStickHighThreashold)
      {
        pushHat(Hat::UP, 1);
        delay(50);
        Serial.println("Up DPad Pressed");
        joyStickYValue = analogRead(joystickY);
      }
    }
  }
  ClearDisplay();
}

void WaitForAlternateInput() 
{
  ClearDisplay();
  tm.display(0,11);
    while (true) {
    int joyStickXValue = analogRead(joystickX);
    int joyStickYValue = analogRead(joystickY);
    int touchSensorBtnState = digitalRead(touchSensor);

    // Touch Sensor press
    if(touchSensorBtnState==1) {
      // Check if the sensor is being held
      int timer = 0;
      while(digitalRead(touchSensor)== 1) {
        timer++;
        delay(50);
      }
      // held for 600+ miliseconds
      if(timer > 11) {
        break;
      }
      // button clicked quickly
      else {
        pushButton(Button::HOME, 1);
        delay(50);
      }
      
    }
    // Button inner press
    if (joyStickXValue > 1000)
    {
      int timer = 0;
      while(joyStickXValue > 1000) {
        delay(50);
        timer++;
        joyStickXValue = analogRead(joystickX);
      }
      // Button was held for 600+ miliseconds
      if(timer > 11) {
        pushButton(Button::ZL, 1);
      }
      else {
        pushButton(Button::PLUS, 1);
      }

    }
    // Left button press
    else if (joyStickXValue < joyStickLowThreashold)
    {
      while(joyStickXValue < joyStickLowThreashold) {
        Serial.println("Left DPad Pressed");
        pushButton(Button::Y, 1);
        delay(50);
        joyStickXValue = analogRead(joystickX);
      }
    }
    // Right button press
    else if (joyStickXValue > joyStickHighThreashold)
    {
      while(joyStickXValue > joyStickHighThreashold) {
        Serial.println("Right DPad Pressed");
        pushButton(Button::R, 1);
        delay(50);
        joyStickXValue = analogRead(joystickX);
      }
    }
    // Down button press
    else if (joyStickYValue < joyStickLowThreashold)
    {
      while (joyStickYValue < joyStickLowThreashold)
      {
        pushButton(Button::ZR, 1);
        delay(50);
        Serial.println("Down DPad Pressed");
        joyStickYValue = analogRead(joystickY);
      }
    }
    // Up button press
    else if (joyStickYValue > joyStickHighThreashold)
    {
      while (joyStickYValue > joyStickHighThreashold)
      {
        pushButton(Button::L, 1);
        delay(50);
        Serial.println("Up DPad Pressed");
        joyStickYValue = analogRead(joystickY);
      }
    }

   ClearDisplay();
   tm.display(0,10);
  }
}

void runMainProgram()
{
  // Script for picking random items from the custom items menu
  if (curScriptNum == 1)
  {
    Automated1PlayerPairing();
  }
  else if (curScriptNum == 2)
  {
    ManualControllerInput();
  }
  else if (curScriptNum == 3)
  {
    AutomatedCharacterSelection();
  }
  else if (curScriptNum == 4) 
  {
    AutomatedKartSelection();
  }
  else if (curScriptNum == 5) {
    AutomatedMenuSelection();
  }
  else if (curScriptNum == 6) {
    AutomatedItemSelection();
  }
  else if (curScriptNum == 7) {
    AutomatedRepairingToUser();
  }
  else if (curScriptNum == 8) {
    AutomatedSinglePlayerSelection();
  }


}

void Automated1PlayerPairing() {

  
  // We need to activate the switch menu
    pushButton(Button::HOME,500);

    // Now we need to move down once
    pushHat(Hat::DOWN, 300);
    

    // Now we need to move right 4 times to get to the controller settings
    
      pushHat(Hat::RIGHT, 25,4);
   
    // Now we click A for the controller options
    // The first option will be to Change Grip/Order
    pushButton(Button::A,500);
    pushButton(Button::A,200);
    

    
    // Now we are at the pairing screen
    
      // We need to pair the controller then exit back to the game
      pushButton(Button::A,1000);
      // Now that the controller is paired as the first player we need to go back to the switch menu
      pushButton(Button::A,1000);
      
      pushButton(Button::B,500);
      //Now we can go back to th main menu of the switch
      pushButton(Button::B,500);
      
      pushButton(Button::HOME,500);
    
}

void AutomatedCharacterSelection()
{
  delay(500);
  // Clear out display
  ClearDisplay();
  
  int randomRow = random(0, 7);
  int randomCol = random(0, 8);
  
  // Set start position [0,0] --> Beforehand
  int curRow = 0;
  int curCol = 0;
  tm.display(1, randomRow);
  tm.display(2, randomCol);

  // Now we need to navigate to the correct square
        while(curRow != randomRow || curCol != randomCol) {
           // UP_LEFT
          if(curRow > randomRow && curCol > randomCol) {
            pushHat(Hat::UP_LEFT, 50);
            curRow--;
            curCol--;
          }
          // UP_RIGHT
          else if (curRow > randomRow && curCol < randomCol) {
            pushHat(Hat::UP_RIGHT, 50);
            curRow--;
            curCol++;
          }
          // DOWN_RIGHT
          else if (curRow < randomRow && curCol < randomCol) {
            pushHat(Hat::DOWN_RIGHT, 50);
            curRow++;
            curCol++;
          }
          // DOWN_LEFT
          else if (curRow < randomRow && curCol > randomCol) {
            pushHat(Hat::DOWN_LEFT, 50);
            curRow ++;
            curCol --;
          }          
          // RIGHT
          if (curCol < randomCol) {
            pushHat(Hat::RIGHT, 50);
            curCol = curCol+1;
          }
          // LEFT
          else if (curCol > randomCol) {
            pushHat(Hat::LEFT, 50);
            curCol = curCol- 1;
          }
          // DOWN
          if (curRow < randomRow) {
            pushHat(Hat::DOWN, 50);
            curRow = curRow + 1;
          }
          // UP
          else if (curRow > randomRow) {
            pushHat(Hat::UP, 50);
            curRow = curRow - 1;
          }
        }
        pushButton(Button::A, 50); // Select the item
  // Some characters have multiple options --> Let's select the first one
  delay(500);
  pushButton(Button::A, 50);
  delay(2000);

  // Clear out display
  ClearDisplay();

}

void ClearDisplay() {
  tm.display(0, 16);
  tm.display(1, 16);
  tm.display(2, 16);
  tm.display(3, 16);
}

void AutomatedKartSelection() 
{  
  // Let's reveal the stats of the current selection
 
  pushButton(Button::PLUS, 25);
  // Clear out display
  delay(200);
  ClearDisplay();
  
  // We always start with kart selection
  

  // There is a kart,wheels, and glider that we need to randomize
  for(int i = 0; i < 3; i++) {
    int randomNum = random(0, 31);
    
    for(int j = 0; j < randomNum; j++) {
      pushHat(Hat::DOWN, 25);
    }
    pushButton(Button::A, 50); // Now we move to the next one

  }
  // Now we select this option
  pushButton(Button::A, 25);
 characterAlreadySelected = false;
}

void AutomatedMenuSelection() {

  // There are seven rows of menu options --> for versus mode
  /* 
    {
      Row 1: 5 Options
      Row 2: 2 Options
      Row 3: 9 Options
      Row 4: 3 Options
      Row 5: 3 Options
      Row 6: 3 Options
      Row 7: 8 Options
    }
   */

   // Row 1 Selection
   int randomNum = random(0, 6);
   int curNum = 0;
   for(int i = 0; i < randomNum; i++) {
    pushHat(Hat::RIGHT, 25);
   }
   pushHat(Hat::DOWN, 50);
    
   // Row 2 Selection
   //randomNum = random(0, 3);
   //curNum = 0;
   //for(int i = 0; i < randomNum; i++) {
    //pushHat(Hat::RIGHT, 25);
   //}
   pushHat(Hat::DOWN, 25);

   // Row 3 Selection
   //randomNum = random(0, 10);
   //curNum = 0;
   //for(int i = 0; i < randomNum; i++) {
    //pushHat(Hat::RIGHT, 25);
   //}
   pushHat(Hat::DOWN, 25);

   // Row 4-5 Selection
   for (int j = 0; j < 2; j++) {
     randomNum = random(0, 4);
     curNum = 0;
     for(int i = 0; i < randomNum; i++) {
      pushHat(Hat::RIGHT, 25);
     }
     pushHat(Hat::DOWN, 50);
   }

   // Row 6 selection
   //randomNum = random(0, 4);
     //curNum = 0;
     //for(int i = 0; i < randomNum; i++) {
      //pushHat(Hat::RIGHT, 25);
     //}
     pushHat(Hat::DOWN, 25);
   //}

   // Row 7 Selection
   randomNum = random(0, 9);
   curNum = 0;
   for(int i = 0; i < randomNum; i++) {
    pushHat(Hat::RIGHT, 25);
   }
   delay(500);
   pushButton(Button::A, 50); // Confirm the settings
   // delay(1000);
   // pushButton(Button::A, 50); // We may be in teams mode so we need to click again
  
}
void AutomatedItemSelection()
{
  // Clear out the display
  ClearDisplay();

  // We are going to start from the ok button
  pushHat(Hat::UP, 200);
  // We are either at the option Random, All off, or All on
  //pushHat(Hat::RIGHT, 50,2);
  
  pushButton(Button::A, 200);
  pushHat(Hat::DOWN, 200);
  pushHat(Hat::DOWN, 200);
  // Now we are at the top row 
  

  // This script will only work properly if the the cursor starts at 0,0 and all items are disabled

  // The item selection grid layout
  /*
    {
    Banna      ,    Triple Banna ,    Green Shell ,     Triple Green Shell,       Red Shell,       Triple Red Shell
    Blue Shell ,             Bomb,        mushroom,        Triple Mushroom,       Golden mushroom, Bullet Bill
    Squid      ,       lightning ,            star,            fire flower,       bomarang,        pirana plant
    boom box   ,    crazy 8,                  coin,                    boo

    }

  */
  int board[4][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

  int numberOfPicks = random(1, 25); // Randomly choose the number of items to select // The top number is not included while the bottom one is
  int itemsSelected = 0;
  Serial.print(" numberOfPicks = ");
  Serial.println(numberOfPicks);
  Serial.println("-----------------");

  
  DisplayFullTwoDigitNumber(numberOfPicks);
  delay(200);





  int curRow = 0;
  int curCol = 0;
  
  while (itemsSelected < numberOfPicks)
  {
    // We need to keep looping until we have a valid item selection even if we have to go over the number
    // of picks we wanted
    
      int randomRow = random(0, 4);
      int randomCol = random(0, 6);
      // There are 2 squares that do not conatain an item so let's ignore those
      //if(randomRow == 3 && (randomCol = 4 || randomCol == 5)) 
      //{
        //continue;
      //}
      
      Serial.print(" Trying to select: ["); Serial.print(randomRow); Serial.print(","); Serial.print(randomCol); Serial.print("]");
      tm.clearDisplay();
      //delay(500);
      // We are now selecting this square
      if (board[randomRow][randomCol] == 0)
      {
        //tm.display(0, 10); // a -> accepted
        tm.display(2, randomRow);
        tm.display(3, randomCol);
        // Now we need to navigate to the correct square
        while(curRow != randomRow || curCol != randomCol) {
          
           // UP_LEFT
          if(curRow > randomRow && curCol > randomCol) {
            pushHat(Hat::UP_LEFT, 50);
            curRow--;
            curCol--;
          }
          // UP_RIGHT
          else if (curRow > randomRow && curCol < randomCol) {
            pushHat(Hat::UP_RIGHT, 50);
            curRow--;
            curCol++;
          }
          // DOWN_RIGHT
          else if (curRow < randomRow && curCol < randomCol) {
            pushHat(Hat::DOWN_RIGHT, 50);
            curRow++;
            curCol++;
          }
          // DOWN_LEFT
          else if (curRow < randomRow && curCol > randomCol) {
            pushHat(Hat::DOWN_LEFT, 50);
            curRow ++;
            curCol --;
          }
          
          // RIGHT
          if (curCol < randomCol) {
            pushHat(Hat::RIGHT, 50);
            curCol = curCol+1;
          }
          // LEFT
          else if (curCol > randomCol) {
            pushHat(Hat::LEFT, 50);
            curCol = curCol- 1;
          }
          // DOWN
          if (curRow < randomRow) {
            pushHat(Hat::DOWN, 50);
            curRow = curRow + 1;
          }
          // UP
          else if (curRow > randomRow) {
            pushHat(Hat::UP, 50);
            curRow = curRow - 1;
          }
        }
        pushButton(Button::A, 50); // Select the item

        board[randomRow][randomCol] = 1;
        Serial.println("--> Accepted");
        // Display that we picked this row col pair
        itemsSelected++;
      }
      //else {
        //Serial.println("--> Rejected");
        //tm.display(0, 14); // e -> exculded
        //tm.display(2, randomRow);
        //tm.display(3, randomCol);

      //}
      //tm.clearDisplay();
      // We need to check if the number is bigger than 10 or 20
      //DisplayFullTwoDigitNumber(itemsSelected);
      // delay(500);



    

  }
  // Now let's display the resulting array

  Serial.println("{");

  // loop through array's rows
  for ( int i = 0; i < 4; i++ ) {
    // loop through columns of current row
    for ( int j = 0; j < 6; j++ ) {
      Serial.print (board[ i ][ j ] );
      Serial.print(" ") ; // start new line of output
    }
    Serial.println();
  }

  Serial.println("}");
  ClearDisplay();

  tm.display(0, 10);
  tm.display(1, 10);
  tm.display(2, 10);
  tm.display(3, 10);


  tone(buzzer, 2000, 1000); // activate the buzzer

  // Once the items are selected we need to scroll down to the bottom and click OK
  while(curRow < 5) {
    pushHat(Hat::DOWN, 25);
    curRow ++;
  }

  pushButton(Button::A, 25);
  characterAlreadySelected = false;
  automatedKartSelectionAlreadyUsed = false;


}

void AutomatedRepairingToUser() 
{
  
  // We need to activate the switch menu
    pushButton(Button::HOME,500);

    // Now we need to move down once
    pushHat(Hat::DOWN, 300);
    

    // Now we need to move right 4 times to get to the controller settings
    
      pushHat(Hat::RIGHT, 50,4);
   
    // Now we click A for the controller options
    // The first option will be to Change Grip/Order
    pushButton(Button::A,500);
    pushButton(Button::A,200);

    // Now we are at the pairing screen
    pushButton(Button::A,1000);
}

void AutomatedSinglePlayerSelection() 
{

  // Make sure the controller is paired
  pushButton(Button::Y, 50);
  delay(500);
  
  // Assumes we start at the main menu
  Automated1PlayerPairing();
  delay(1000);
  
  // We need to click for single player'
  pushButton(Button::A,25);
  delay(2000);
  
  //Click to enter the game mode
  pushButton(Button::A,25);
  delay(1000);
  AutomatedCharacterSelection();
  delay(1000);
  AutomatedKartSelection();
  delay(1000);
  AutomatedMenuSelection();
  delay(1000);
  AutomatedItemSelection();
  delay(1000);
  // Start the game
  pushButton(Button::A,25);
  
  AutomatedRepairingToUser();
  
}

void buttonMashing()
{
  while (true) {
    pushButton(Button::X, 600);
    pushHat(Hat::RIGHT, 200);
    pushHat(Hat::DOWN, 200);
    pushHat(Hat::LEFT, 200);
    pushHat(Hat::UP, 200);
    //pushButton(Button::X, 600);
    delay(1000);
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  noTone(buzzer);

  //digitalWrite(buzzer,LOW);
  //Serial.print("X: ");
  //Serial.print(joyStickXValue);
  //Serial.print(" Y: ");
  //Serial.println(joyStickYValue);
  waitForScriptSelection();

  /* Display Clock
    In: 0-9 Out: 0-9
    In: A   Out: 10
    In: 11  Out: B
    In: 12  Out: C
    In: 13  Out: D
    In: 14  Out: E
    In: 15  Out: F
    tm.display(position,character)
    position: 0-3
  */

  if (confirmScriptNum == true)
  {
    // Check if script command is valid
    bool validScriptCommand = false;
    for (int i = 0; i < sizeof(validScriptNums); i++)
    {
      if (validScriptNums[i] == curScriptNum)
      {
        validScriptCommand = true;
        break;
      }
    }
    if (validScriptCommand == true)
    {
      runMainProgram();
    }
    // Display command invalid text
    else
    {
      // Display Error
      // Clear display
      tm.display(0, 16);
      tm.display(1, 16);
      tm.display(2, 16);
      tm.display(3, 16);

      delay(1000);

      tm.display(0, 14);
      tm.display(1, 14);
      tm.display(2, 14);
      tm.display(3, 14);
      tone(buzzer, 6000, 1000); // activate the buzzer
      delay(1000);

    }

    tm.clearDisplay();
    confirmScriptNum = false;
    tm.display(3, curScriptNum);
  }
}
