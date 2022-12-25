#include <TM1637.h>
// Joystick setup
const int joystickX = A0;
const int joystickY = A1;
const int joyStickButtonPress = 1023;
const int joyStickLowThreashold = 300;
const int joyStickHighThreashold = 650;
const int validScriptNums[1] {1};
int lastButtonPress = -1;
int curScriptNum = 1;
int confirmScriptNum = false;
bool waitForInput = true;

// Display Clock setup
const int clock = 2;
const int dioClock = 3;
TM1637 tm(clock,dioClock);

// Buzzer setup
const int buzzer = 3;

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

void setup() {
  // put your setup code here, to run once:

  // Display clock setup
  tm.init();
  tm.set(5); //set brightness 0-7

  //tm.point(true);   // enable the colon


  Serial.begin(9600);
  pinMode(joystickX,INPUT);
  pinMode(joystickY,INPUT);
  pinMode(buzzer,OUTPUT);
  tm.display(3,curScriptNum);



}

void resetClock() 
{
  tm.clearDisplay();
}

void waitForScriptSelection() 
{
  //Serial.println("Press a Button:");

  // Set the arduino to wait for input
    while(waitForInput == true) {
      int joyStickXValue = analogRead(joystickX);
      int joyStickYValue = analogRead(joystickY);

       // Button inner press (selection confirmed)
        if(joyStickXValue == joyStickButtonPress) 
        {
          // Serial.println("Inner Button Pressed");
          // resetClock();
          tm.display(3,10);

          // 2000 to 5000 is best frequency for humans
          tone(buzzer,2000, 1000); // activate the buzzer
          lastButtonPress = 0;
          confirmScriptNum = true;

          //waitForInput = false;
          break;
        }
       
       /*
        // Left button press
        else if(joyStickXValue < joyStickLowThreashold && lastButtonPress != 1) 
        {
          Serial.println("Left Button Pressed");
          resetClock();
          tm.display(0,1);
          waitForInput = false;
          lastButtonPress = 1;
          
        }
        // Right button press
        else if (joyStickXValue > joyStickHighThreashold && lastButtonPress != 2) 
        {
          Serial.println("Right Button Pressed");
          resetClock();
          tm.display(3,1);
          waitForInput = false;
          lastButtonPress = 2;
        }

        */

        // Down button press
        if (joyStickYValue < joyStickLowThreashold && curScriptNum-1 > 0) 
        {
          curScriptNum = curScriptNum - 1;
          tm.display(3,curScriptNum);
          waitForInput = false;
          lastButtonPress = 3;
        }
        // Up button press
        else if (joyStickYValue > joyStickHighThreashold && curScriptNum+1 < 16) 
        {
          curScriptNum = curScriptNum + 1;
          tm.display(3,curScriptNum);
          waitForInput = false;
          lastButtonPress = 4;
        }

        delay(200);
        
    }

    
  
    waitForInput = true;
  
}
/*
void displayScriptNum(int someValue) 
{
  int valLen = 0;

  // Figure out how many digits we need to use
  if(someValue > 9999)
    valLen = 5;
  else if(someValue > 999)
    valLen = 4;
  else if(someValue > 99)
    valLen = 3;
  else if(someValue > 9)
    valLen = 2;
  else
    valLen = 1;


  if(valLen == 1) 
  {
    tm.display(3,someValue);
  }
  else if (valLen == 2) 
  {
    tm.display(2,someValue % 100);
    tm.display(3,someValue % 10);
  }
  else if (valLen == 3) 
  {
    tm.display(1,someValue % 1000);
    tm.display(2,someValue % 100);
    tm.display(3,someValue % 10);

  }
  else if (valLen == 4) 
  {
    tm.display(0,someValue % 10000);
    tm.display(1,someValue % 1000);
    tm.display(2,someValue % 100);
    tm.display(3,someValue % 10);

  }
}

*/

bool checkIfCurSelectionValid() 
{
  // Here we need to check for situations such as blue shells only which is invalid
  return true;


}

void ScriptForPickingRandomItems() 
{
  // Clear out the display
  tm.display(0,16);
  tm.display(1,16);
  tm.display(2,16);
  tm.display(3,16);
  delay(1000);
   //tm.clearDisplay();
   //delay(1000);


   Serial.begin(9600);
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
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0}
    };

    int numberOfPicks = random(1,25); // Randomly choose the number of items to select // The top number is not included while the bottom one is
    int itemsSelected = 0;
    Serial.print(" numberOfPicks = ");
    Serial.println(numberOfPicks);
    Serial.println("-----------------");
    
    delay(2000);
    DisplayFullTwoDigitNumber(numberOfPicks);
    delay(1000);


    


    bool validSelection = false;
    while(itemsSelected < numberOfPicks) 
    {
      validSelection = false;
      // We need to keep looping until we have a valid item selection even if we have to go over the number
      // of picks we wanted
      while(validSelection == false) {
        int randomRow = random(0,4);
        int randomCol = random(0,6);
        Serial.print(" Trying to select: ["); Serial.print(randomRow); Serial.print(","); Serial.print(randomCol); Serial.print("]");
        tm.clearDisplay();
        delay(500);
        // We are now selecting this square
        if(board[randomRow][randomCol] == 0) 
        {
          board[randomRow][randomCol] = 1;
          Serial.println("--> Accepted");
          // Display that we picked this row col pair
          
          tm.display(0,10); // a -> accepted
          tm.display(2,randomRow);
          tm.display(3,randomCol);
          validSelection = checkIfCurSelectionValid();
          itemsSelected++;
        }
        else {
          Serial.println("--> Rejected");
          tm.display(0,14); // e -> exculded
          tm.display(2,randomRow);
          tm.display(3,randomCol);

        }

        delay(500);
        tm.clearDisplay();
        // We need to check if the number is bigger than 10 or 20
        DisplayFullTwoDigitNumber(itemsSelected);
        delay(500);

        
        
      }

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
    tm.clearDisplay();

    tm.display(0,10);
    tm.display(1,10);
    tm.display(2,10);
    tm.display(3,10);


    tone(buzzer,2000, 2000); // activate the buzzer
    delay(5000);


}
void DisplayFullTwoDigitNumber(int item) 
{
  if(item < 10) {
        tm.display(2,item);
        }
        else if(item < 20) 
        {
          tm.display(1,1);
          tm.display(2,item-10);
        }
        else if (item < 30) 
        {
          tm.display(1,2);
          tm.display(2,item-20);

        }
        else if (item < 40) 
        {
          tm.display(2,3);
          tm.display(2,item-30);

        }
        else if (item < 50) 
        {
          tm.display(2,4);
          tm.display(2,item-40);

        }
        else if (item < 60) 
        {
          tm.display(2,5);
          tm.display(2,item-50);

        }
        else if (item < 70) 
        {
          tm.display(2,6);
          tm.display(2,item-60);

        }
        else if (item < 80) 
        {
          tm.display(2,7);
          tm.display(2,item-70);

        }
        else if (item < 90) 
        {
          tm.display(2,8);
          tm.display(2,item-80);

        }
        else if (item < 100) 
        {
          tm.display(2,9);
          tm.display(2,item-90);

        }
        
        
}

void runMainProgram() 
{
  // Script for picking random items from the custom items menu
  if(curScriptNum == 1) 
  {
    ScriptForPickingRandomItems();

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

  if(confirmScriptNum == true) 
  {
    // Check if script command is valid
    bool validScriptCommand = false;
    for (int i = 0; i < sizeof(validScriptNums); i++) 
    {
      if(validScriptNums[i] == curScriptNum) 
      {
        validScriptCommand = true;
        break;
      }
    }
    if(validScriptCommand == true) 
    {
      runMainProgram();
    }
    // Display command invalid text
    else 
    {
      // Display Error
      
      
      // Clear display
      tm.display(0,16);
      tm.display(1,16);
      tm.display(2,16);
      tm.display(3,16);
      
      delay(1000);

      tm.display(0,14);
      tm.display(1,14);
      tm.display(2,14);
      tm.display(3,14);
      tone(buzzer,6000, 1000); // activate the buzzer
      delay(3000);
      
      
    }

    tm.clearDisplay();
    confirmScriptNum = false;
    tm.display(3,curScriptNum);

  }


  

}
