#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// Color definitions from standard lib
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF


//Specific 2048 tile colors
#define TILE_2_COLOR 0xEF3B
#define TILE_4_COLOR 0xEF19
#define TILE_8_COLOR 0xF58F
#define TILE_16_COLOR 0xF4AC
#define TILE_32_COLOR 0xF3EB
#define TILE_64_COLOR 0xF2E7
#define TILE_128_COLOR 0xEE6E
#define TILE_256_COLOR 0xEE6C
#define TILE_512_COLOR 0xEE4A
#define TILE_1024_COLOR 0xEE27
#define TILE_2048_COLOR 0xEE05

#define TILE_WIDTH 30
#define BORDER_WIDTH 32

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

//Joystick state variables
int pressed = -1; //this variable will determine whether joystick has been pressed down (selected)
int x = -1;
int y = -1;

//Move directions
enum Direction {UNDEF, UP, DOWN, RIGHT, LEFT };
Direction  direction, action;

//Grid variables
int i, j, xPos, yPos;
int gameBoard [4][4] = {
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
  {0, 0, 0, 0},   /*  initializers for row indexed by 2 */
  {0, 0, 0, 0}   /*  initializers for row indexed by 3 */
};
int previousGameState[4][4];
int score, previousScore = 0;


void drawEmptyGrid() {
 
  for (xPos = 1; xPos <= 100; xPos += BORDER_WIDTH) {
    for (yPos = 1; yPos <= 100; yPos += BORDER_WIDTH) {
      tft.drawRoundRect(xPos, yPos, BORDER_WIDTH, BORDER_WIDTH, 3, BLACK);
    }
  }
}

void setup() {

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(WHITE);

  //draw tiles


  drawEmptyGrid();

  //generate random seed based on noise from unconnected pin 0
  randomSeed(analogRead(0));
  Serial.begin(57600);

  placeRandomTile();
  placeRandomTile();
  memcpy(previousGameState, gameBoard, sizeof(gameBoard));
  displayGameBoard();

}

int getRandomTileValue() {
  int tile = random( 100 ) < 70 ? 2 : 4;
  return tile;
}

void processLine( int line[], bool setScore ) {

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
      if (setScore) {
        score += line [j - 1];
        Serial.println(score);
      }
      for ( unsigned int k = j; k < count - removed; k++ ) {
        line[k] = line[k + 1];

      }

      line[count - removed - 1] = 0;
    }
  }

}


void moveUp(int grid[4][4], bool setScore) {


  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[j][i];
    }

    processLine( line, setScore );


    for (int j = 0; j < count; j++) {
      grid[j][i] = line[j];
    }

  }
}

void moveDown(int grid[4][4], bool setScore) {


  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[count - j - 1][i];
    }

    processLine( line, setScore );


    for (int j = 0; j < count; j++) {
      grid[j][i] = line[count - j - 1];
    }

  }
}

void moveRight(int grid[4][4], bool setScore) {

  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[i][count - j - 1];
    }

    processLine( line, setScore );

    for (int j = 0; j < count; j++) {
      grid[i][j] = line[count - j - 1];
    }

  }
}

void moveLeft(int grid[4][4], bool setScore) {

  for (int i = 0; i < count; i++) { // iterating rows first

    int line[count];

    for (int j = 0; j < count; j++) {
      line[j] = grid[i][j];
    }

    processLine( line, setScore );
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

    if (!gameBoard [x][y]) {
      gameBoard[x][y] = getRandomTileValue();
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

void printGameBoard() {
  Serial.println();
  for (i = 0; i < count; i++) {
    for (j = 0; j < count; j++) {
      Serial.print(gameBoard[i][j]);
    }
    Serial.println();
  }
}

int tileValue;


void displayTile(int col, int row, int value) {
  int yPosition, xPosition;

  yPosition = 1 + (col * BORDER_WIDTH);
  xPosition = 1 + (row * BORDER_WIDTH);
  tft.setCursor(xPosition + 7, yPosition + 7);
  // tft.drawRoundRect(xPosition - 2, yPosition - 2, BORDER_WIDTH, BORDER_WIDTH, 3, BLACK);

  tft.setTextSize(2);


  switch (value) {

    case 0:
      tft.drawRoundRect(xPosition, yPosition, BORDER_WIDTH, BORDER_WIDTH, 3, BLACK); 
      tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, WHITE);

      break;

    case 2: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_2_COLOR);
      break;

    case 4: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_4_COLOR);
      break;

    case 8: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_8_COLOR);
      break;

    case 16: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_16_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 32: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_32_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 64: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_64_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 128: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_128_COLOR);
      tft.setTextSize(1);
      break;

    case 256: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_256_COLOR);
      tft.setTextSize(1);
      break;

    case 512: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_512_COLOR);
      tft.setTextSize(1);
      break;

    case 1024: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_512_COLOR);
      tft.setTextSize(1);
      break;

    case 2048: tft.fillRoundRect(xPosition+1 , yPosition+1, TILE_WIDTH, TILE_WIDTH, 3, TILE_512_COLOR);
      tft.setTextSize(1);
      break;
  }
  if (value) {
    tft.print(value);
  }
}

void displayGameBoard() {



  //Console output for debugging purposes
  printGameBoard();


  // tft.fillScreen(WHITE);
  tft.setTextColor(BLACK);

  for (i = 0; i < count; i++) {
    for (j = 0; j < count; j++) {

      tileValue = gameBoard[i][j];

      if (previousGameState[i][j] != gameBoard[i][j]) {
        Serial.println("DiaplayTile");
        displayTile(i, j, tileValue);
      }
    }
  }

  memcpy(previousGameState, gameBoard, sizeof(gameBoard));
  
  tft.setTextSize(1);
  tft.setCursor(10, 140);
  tft.setTextColor(RED);

  tft.print("Score: ");
  tft.setTextSize(2);
  tft.setCursor(50, 135);
  if (previousScore != score) {
   // tft.drawRect(10, 140, 30, 60, WHITE);
    tft.fillRect(50, 135, 30, 60, WHITE);
  }
  tft.print(score);

}

void loop() {
  readJoystick();
  handleJoystick();
  delay(200);
}

bool canMoveLeft, canMoveRight, canMoveUp, canMoveDown;
int gridCopy[4][4], gridCopyInitial[4][4];


void checkAvailableMoves() {

  memcpy(gridCopyInitial, gameBoard, sizeof(gameBoard));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveLeft(gridCopy, 0);
  canMoveLeft = memcmp(gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveUp(gridCopy, 0);
  canMoveUp = memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveDown(gridCopy, 0);
  canMoveDown = memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveRight(gridCopy, 0);
  canMoveRight = memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

}

bool isGameOver() {
  return !canMoveLeft && !canMoveRight && !canMoveUp && !canMoveDown;
}

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
    checkAvailableMoves();

    //   while (!isGameOver) {
    switch (action) {
      case UP:

        Serial.print("UP");
        Serial.println(canMoveUp);
        if (canMoveUp) {
          moveUp(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();

        }
        break;

      case DOWN:
        Serial.print("DOWN");
        Serial.println(canMoveDown);
        if (canMoveDown) {

          moveDown(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();

        }
        break;


      case LEFT:
        Serial.print("LEFT");
        Serial.println(canMoveLeft);

        if (canMoveLeft) {
          moveLeft(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();

        }
        break;


      case RIGHT:
        Serial.print("RIGHT");
        Serial.println(canMoveRight);
        if (canMoveRight) {
          moveRight(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();


        }
        break;

    }

    //set direction and action to 0
    direction = UNDEF;
    action = UNDEF;


    checkAvailableMoves();


  }

}

void readJoystick() {
  pressed = !digitalRead(SwButtonPin);//reads whether joystick has been pressed down (selected) or not
  x = analogRead(VRxPin);//reads the X-coordinate value
  y = analogRead(VRyPin);//reads the Y-coordinate value
}




