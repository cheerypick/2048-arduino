#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define BEIGE_2 0xC618
#define BEIGE_4 0xEF19
#define ORANGE_8 0xF58F
#define PEACH_16 0xF4AC
#define RED_32 0xF3EB





// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <SPI.h>
#include <Wire.h>


// TFT pin definitions and declarations
#define CS   10
#define DC   9
#define RST  8


Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, RST);

long randNumber;

//Joystick pins
const int VRxPin = A2; //VRx pin connected to arduino pin A2
const int VRyPin = A3; //VRy pin connected to arduino in A3
const int SwButtonPin = 7; //SW pin connected to arduino pin D7
const int count = 4;

int pressed = -1; //this variable will determine whether joystick has been pressed down (selected)
int x = -1;
int y = -1; //this variable will hold the Y-coordinate value




int grid [4][4] = {
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
  {0, 0, 0, 0},   /*  initializers for row indexed by 2 */
  {0, 0, 0, 0}   /*  initializers for row indexed by 3 */
};

int i, j, xPos, yPos;

void setup() {


  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  //tft.setRotation(1);
  tft.fillScreen(BEIGE_2);

  //draw tiles

  for (xPos = 2; xPos <= 100; xPos += 31) {
    for (yPos = 2; yPos <= 100; yPos += 31) {
      tft.drawRoundRect(xPos, yPos, 32, 32, 3, BLACK);
    }
  }
  
  //generate random seed based on noise from unconnected pin 0
  randomSeed(analogRead(0));
  Serial.begin(57600);


  placeRandomTile();
  placeRandomTile();

  displayGrid();
  printGrid();

}

int getRandomTileValue() {
  int tile = random( 100 ) < 70 ? 2 : 4;
  return tile;
}

void processLine( int line[] ) {

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
    int x = random(count);
    int y = random (count);

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

  for (i = 0; i < count; i++) {
    for (j = 0; j < count; j++) {

      Serial.print(grid[i][j]);
    }
    Serial.println();
  }
}

void displayGrid() {

  tft.fillScreen(BEIGE_2);
  tft.setTextColor(RED);
  tft.setTextSize(2);


  for (yPos = 4, i = 0; i < count; yPos += 31, i++) {
    for (xPos = 4, j = 0; j < count; xPos += 31, j++) {

      tft.drawRoundRect(xPos - 2, yPos - 2, 32, 32, 3, BLACK);
      tft.setCursor(xPos, yPos);
      if (grid[i][j]) {
        tft.print(grid[i][j]);
      }
    }
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
        Serial.println("Up!");
        displayGrid();
        printGrid();
        placeRandomTile();

        break;

      case DOWN:
        moveDown();
        Serial.println("Down!");
        displayGrid();
        printGrid();
        placeRandomTile();
        break;


      case LEFT:
        moveLeft();
        Serial.println("Left!");
        displayGrid();
        printGrid();
        placeRandomTile();
        break;


      case RIGHT:
        moveRight();
        Serial.println("Right!");
        displayGrid();
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




