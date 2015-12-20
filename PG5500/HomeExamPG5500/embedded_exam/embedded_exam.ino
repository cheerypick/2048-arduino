#include <SD.h>
#include <TFT.h>  
#include <SPI.h>
#include "pitches.h"

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

#define BUZZER_PIN 6

// TFT pin definitions and declarations
#define CS   10
#define DC   9
#define RST  8
#define SD_CS 4

Adafruit_ST7735 tft = Adafruit_ST7735(CS, DC, RST);
PImage logo;

long randNumber;

//Joystick pins
const int VRxPin = A2; //VRx pin connected to arduino pin A2
const int VRyPin = A3; //VRy pin connected to arduino in A3
const int SwButtonPin = 7; //SW pin connected to arduino pin D7


const int count = 4;

//Joystick state variables
int pressed = -1;
int x = -1;
int y = -1;

enum Direction {UNDEF, UP, DOWN, RIGHT, LEFT };
Direction  direction, action;

bool canMoveLeft, canMoveRight, canMoveUp, canMoveDown, waitingMode, gameMode, isGameOver;
File myFile;
int score, previousScore = 0;
int tileValue;

//Grid variables
int i, j, xPos, yPos;
int gameBoard [4][4] = {
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 0 */
  {0, 0, 0, 0} ,   /*  initializers for row indexed by 1 */
  {0, 0, 0, 0},   /*  initializers for row indexed by 2 */
  {0, 0, 0, 0}   /*  initializers for row indexed by 3 */
};

int gridCopy[4][4], gridCopyInitial[4][4];

int  previousGameState [4][4] =  {
  {0, 0, 0, 0} ,
  {0, 0, 0, 0} ,
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

int  emptyGameState [4][4] =  {
  {0, 0, 0, 0} ,
  {0, 0, 0, 0} ,
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};


void drawEmptyGrid() {

  for (xPos = 1; xPos <= 100; xPos += BORDER_WIDTH) {
    for (yPos = 1; yPos <= 100; yPos += BORDER_WIDTH) {
      tft.drawRoundRect(xPos, yPos, BORDER_WIDTH, BORDER_WIDTH, 3, BLACK);
    }
  }
}




void setup() {

  initializeSD();
  waitingMode = true;

  pinMode(SwButtonPin, INPUT_PULLUP); //sets the joystick SW as input button

  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(WHITE);

  //generate random seed based on noise from unconnected pin 0
  randomSeed(analogRead(0));
  Serial.begin(57600);
  displayImage();
  writeHighScore(315);
  readHighScore();
}


void loop() {

  readJoystick();
  if (pressed && waitingMode) {
    startGame();
  }

  if (!waitingMode && !isGameOver) {
    playGame();
  }

  if (isGameOver) {
    waitingMode = true;
    printGameOver();
  }
  delay(50);
}

void writeHighScore(int score)  {
  if (SD.exists("score.txt")) {
    SD.remove("score.txt");
    myFile = SD.open("score.txt", FILE_WRITE);
    myFile.println(score);
    myFile.close();
  } else {
    Serial.println("File doesn't exist.");
  }
}


void readHighScore()  {
  if (SD.exists("score.txt")) {
    myFile = SD.open("score.txt");
    if (myFile) {
      while (myFile.available()) {
        int highScore = myFile.parseInt();
        Serial.println(highScore);
      }
      myFile.close();
    } else {
      Serial.println("Error opening file. ");
    }
  } else {
    Serial.println("File doesn't exist.");
  }
}


void printGameOver () {
  tft.fillScreen(WHITE);
  tft.stroke(RED);
  tft.text("Game Over", 10, 10);
  tft.setTextSize(1);
  tft.text("Press joystick \n to restart game", 10, 60);
  printScore();
}


void initializeSD() {
  if (!SD.begin(SD_CS)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));

  logo = tft.loadImage("2048.bmp");
  if (!logo.isValid()) {
    Serial.println(F("error while loading 2048.bmp"));
  }
}

void displayImage() {

  tft.image(logo, 0, 0);
  tft.stroke(WHITE);
  tft.text("Press joystick \n to start", 10, 60);
}

void startGame() {

  waitingMode = false;
  resetGameState();
  tft.fillScreen(WHITE);
  drawEmptyGrid();
  placeRandomTile();
  placeRandomTile();
  displayGameBoard();
}

void resetGameState() {
 memcpy (previousGameState, emptyGameState, sizeof(emptyGameState));
 memcpy (gameBoard, emptyGameState, sizeof(emptyGameState));
 isGameOver = false;
 score, previousScore = 0;
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
        playSound();
        score += line [j - 1];
      }
      for ( unsigned int k = j; k < count - removed; k++ ) {
        line[k] = line[k + 1];
      }

      line[count - removed - 1] = 0;
    }
  }

}


void moveUp(int grid[4][4], bool setScore) {


  for (int i = 0; i < count; i++) {
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


  for (int i = 0; i < count; i++) {
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


void displayTile(int col, int row, int value) {
  int yPosition, xPosition;

  yPosition = 1 + (col * BORDER_WIDTH);
  xPosition = 1 + (row * BORDER_WIDTH);
  tft.setCursor(xPosition + 7, yPosition + 7);
  tft.setTextSize(2);


  switch (value) {

    case 0:
      tft.drawRoundRect(xPosition, yPosition, BORDER_WIDTH, BORDER_WIDTH, 3, BLACK);
      tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, WHITE);

      break;

    case 2: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_2_COLOR);
      break;

    case 4: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_4_COLOR);
      break;

    case 8: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_8_COLOR);
      break;

    case 16: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_16_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 32: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_32_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 64: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_64_COLOR);
      tft.setCursor(xPosition + 4, yPosition + 7);
      break;

    case 128: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_128_COLOR);
      tft.setCursor(xPosition + 7, yPosition + 13);
      tft.setTextSize(1);
      break;

    case 256: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_256_COLOR);

      tft.setCursor(xPosition + 7, yPosition + 13);
      tft.setTextSize(1);
      break;

    case 512: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_512_COLOR);
      tft.setCursor(xPosition + 7, yPosition + 13);
      tft.setTextSize(1);
      break;

    case 1024: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_1024_COLOR);
      tft.setCursor(xPosition + 7, yPosition + 13);
      tft.setTextSize(1);
      break;

    case 2048: tft.fillRoundRect(xPosition + 1 , yPosition + 1, TILE_WIDTH, TILE_WIDTH, 3, TILE_2048_COLOR);
      tft.setCursor(xPosition + 7, yPosition + 13);
      tft.setTextSize(1);
      break;
  }
  if (value) {
    tft.print(value);
  }
}

void displayGameBoard() {

  tft.setTextColor(BLACK);

  for (i = 0; i < count; i++) {
    for (j = 0; j < count; j++) {
      tileValue = gameBoard[i][j];
      if (previousGameState[i][j] != gameBoard[i][j]) {
        displayTile(i, j, tileValue);
      }
    }
  }

  memcpy(previousGameState, gameBoard, sizeof(gameBoard));
  printScore();

}

void printScore () {
  tft.setTextSize(1);
  tft.setCursor(10, 140);
  tft.setTextColor(RED);
  tft.print("Score: ");
  tft.setTextSize(2);
  tft.setCursor(50, 135);
  if (previousScore != score) {
    tft.fillRect(50, 135, 50, 50, WHITE);
  }
  tft.print(score);
}



void checkAvailableMoves() {

  memcpy(gridCopyInitial, gameBoard, sizeof(gameBoard));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveLeft(gridCopy, 0);
  canMoveLeft = memcmp(gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveUp(gridCopy, 0);
  canMoveUp =  memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveDown(gridCopy, 0);
  canMoveDown =  memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  memcpy(gridCopy, gridCopyInitial, sizeof(gridCopyInitial));
  moveRight(gridCopy, 0);
  canMoveRight =  memcmp (gridCopy, gridCopyInitial, sizeof (gridCopyInitial));

  isGameOver = !(canMoveLeft || canMoveRight || canMoveUp || canMoveDown);

}

void playGame() {
  if (x < 5) {
    direction = DOWN;
  }
  if (x > 1020) {
    direction = UP;
  }
  if (y < 5) {
    direction = RIGHT;
  }
  if (y > 1020) {
    direction = LEFT;
  }
  if (x > 450 && x < 550 && y > 450 && y < 550 && direction) {
    action = direction;
    checkAvailableMoves();

    switch (action) {
      case UP:

        if (canMoveUp) {
          moveUp(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();
        }
        break;

      case DOWN:

        if (canMoveDown) {
          moveDown(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();
        }
        break;

      case LEFT:

        if (canMoveLeft) {
          moveLeft(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();
        }
        break;


      case RIGHT:

        if (canMoveRight) {
          moveRight(gameBoard, 1);
          displayGameBoard();
          placeRandomTile();
          displayGameBoard();
        }
        break;
    }

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



////SOUNDS

void playSound() {
 tone(BUZZER_PIN,NOTE_G4,35);
  delay(35);
  tone(BUZZER_PIN,NOTE_G5,35);
  delay(35);
  tone(BUZZER_PIN,NOTE_G6,35);
  delay(35);
  noTone(BUZZER_PIN);
  noTone(BUZZER_PIN);
}

