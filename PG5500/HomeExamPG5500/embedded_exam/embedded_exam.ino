

#include <TFT.h>  // Arduino LCD library
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <SPI.h>
#include <Wire.h>


// TFT pin definitions and declarations
#define CS   10
#define DC   9
#define RST  8


TFT TFTscreen = TFT(CS, DC, RST);

long randNumber;

//Joystick pins
const int VRxPin = A2; //VRx pin connected to arduino pin A2
const int VRyPin = A3; //VRy pin connected to arduino in A3
const int SwButtonPin = 7; //SW pin connected to arduino pin D7
const int count = 4;

int pressed = -1; //this variable will determine whether joystick has been pressed down (selected)
int x = -1;
int y = -1; //this variable will hold the Y-coordinate value
//String direction, action;

bool stickReleased = true;



int grid [4][4] = {
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
  {0, 0, 0, 0},   /*  initializers for row indexed by 2 */
  {0, 0, 0, 0}   /*  initializers for row indexed by 3 */
};

int i, j;

void setup() {


  //generate random seed based on noise from unconnected pin 0
  randomSeed(analogRead(0));


  // pinMode(SwButtonPin, INPUT_PULLUP); //sets the SW switch as input
  Serial.begin(57600);

 
  placeRandomTile();
  placeRandomTile();
  //moveLeft();
  
  printGrid();
  // TFTscreen.begin();
  // clear the screen with a black background
  // TFTscreen.background(0, 0, 0);
  // TFTscreen.stroke(255, 255, 255);
  // set the font size
  // TFTscreen.setTextSize(2);
}

int getRandomTileValue() {
  int tile = random( 100 ) < 70 ? 2 : 4;
  return tile;
}


void processLine( int line[] ) {

  //printLine(line);

  unsigned int l = 0, removed = 0;

  for ( unsigned int j = 0; j < count; j++ ) {
    if ( line[j] != 0 )
      line[l++] = line[j];
    else
      removed++;
  }

  for ( unsigned int j = 0; j < removed; j++ ) {
    line[count - removed + j] = 0;
  }

  for ( unsigned int j = 1; j < count - removed; j++ ) {
    if ( line[j] == line[j - 1] ) {

      line[j - 1] *= 2;

      for ( unsigned int k = j; k < count - removed; k++ ) {
        line[k] = line[k + 1];

      }

      line[count - removed - 1] = 0;


    }
  }


}


void moveUp() {


  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[j][i];
    }

    processLine( line );


    for (int j = 0; j < count; j++) {
      grid[j][i] = line[j];
    }

  }
}

void moveDown() {


  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[count - j - 1][i];
    }

    processLine( line );


    for (int j = 0; j < count; j++) {
      grid[j][i] = line[count - j - 1];
    }

  }
}

void moveRight() {

  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[i][count - j - 1];
    }

    processLine( line );


    for (int j = 0; j < count; j++) {
      grid[i][j] = line[count - j - 1];
    }

  }
}

void moveLeft() {

  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[i][j];
    }

    processLine( line );


    for (int j = 0; j < count; j++) {
      grid[i][j] = line[j];
    }

  }

}



void placeRandomTile() {
  bool found = false;
  while (!found) {
    int x = random(4);
    int y = random (4);

    //  Serial.print("X:" + x);
    //  Serial.print("Y:" + y);
    if (!grid [x][y]) {
      grid[x][y] = getRandomTileValue();
      found = true;
    }
  }
}

void printLine( int line[] ) {

  Serial.print("line ");
  for (j = 0; j < count; j++) {

    Serial.print(line[j]);
  }
  Serial.println();

}

void printGrid() {
  Serial.println();
  
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {

      Serial.print(grid[i][j]);
    }
    Serial.println();
  }
}

void loop() {
 readJoystick();
 handleJoystick();

  delay(200);
}

enum Direction {UNDEF, UP, DOWN, RIGHT, LEFT };

Direction  direction;
Direction action;

void handleJoystick() {
  if (x < 5) {
    direction = UP;
  }
  if (x > 1020) {
    direction = DOWN;
  }
  if (y < 5) {
    direction = LEFT;
  }
  if (y > 1020) {
    direction = RIGHT;
  }
  if (x > 450 && x < 550 && y > 450 && y < 550 && direction) {
    action = direction;

    switch (action) {
      case UP:
        moveUp();
        printGrid();
        placeRandomTile();
        
        break;

      case DOWN:
        moveDown();
        printGrid();
        placeRandomTile();
        break;


      case LEFT:
        moveLeft();
        printGrid();
        placeRandomTile();
        break;


      case RIGHT:
        moveRight();
        printGrid();
        placeRandomTile();
        break;

    }

    //set direction and action to 0
    direction = UNDEF;
    action = UNDEF;
    

  }

}



void readJoystick() {
  pressed = !digitalRead(SwButtonPin);//reads whether joystick has been pressed down (selected) or not
  x = analogRead(VRxPin);//reads the X-coordinate value
  y = analogRead(VRyPin);//reads the Y-coordinate value
}




