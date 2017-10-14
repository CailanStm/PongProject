
int dataPin = 2;
int latchPin = 3;
int clockPin = 4;

int player1Pin = 0; //analog 0
int player2Pin = 1;

int player1Position;
int player2Position;

int speed = 15;

int ballX = 3;
int ballY = 3;

boolean gameOn = true;

int ballMovingUp, ballMovingDown, ballMovingRight;

int board[8][8] = {
  {
    0,0,0,0,0,0,0,0      }
  , 
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
  ,
  {
    0,0,0,0,0,0,0,0      }
};

int pong[8][8] = {
  {
    1,1,1,0,1,1,1,1      }
  , 
  {
    1,0,1,0,1,0,0,1      }
  ,
  {
    1,1,1,0,1,0,0,1      }
  ,
  {
    1,0,0,0,1,1,1,1      }
  ,
  {
    1,0,0,1,1,1,1,0      }
  ,
  {
    1,1,0,1,1,0,1,1      }
  ,
  {
    1,0,1,1,1,0,0,1      }
  ,
  {
    1,0,0,1,1,1,1,1      }
};

byte dataOne = 0;
byte dataTwo = 0;

int count = 0;

void setup() {
  ballMovingUp = false;
  ballMovingDown = false;

  int b = (int) (random(2));
  if (b == 0) {
    ballMovingRight = false;
  } 
  else {
    ballMovingRight = true;
  }

  Serial.begin(9600);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 

  resetLEDs();
}

void loop() {
  //resetLEDs();
  //allOn();
  //displayImage(pong); 

  if (gameOn) {
    updatePlayer();

    if (count > speed) {
      updateBall();
      count = 0;
    }
    count++;

    displayImage(board);

    for (int i = 0; i < 8; i++) {
      Serial.print(board[3][i]);
    }
    Serial.println();
  } 
  else {
    //displayImage(pong);
    resetLEDs();
    gameOn = false;
  }

}

void updatePlayer() {
  resetCols();

  int player1PotentioValue = analogRead(player1Pin);
  player1Position = player1PotentioValue * (7.0/1023) + 1;
  player1Position = constrain(player1Position, 1, 7);
  board[player1Position][0] = 1;
  board[player1Position - 1][0] = 1;

  int player2PotentioValue = analogRead(player2Pin);
  player2Position = player2PotentioValue * (7.0/1023) + 1;
  player2Position = constrain(player2Position, 1, 7);
  board[player2Position][7] = 1;
  board[player2Position - 1][7] = 1;



}

void updateBall() {
  board[ballY][ballX] = 0;

  if ((ballY == 0) || (ballY == 7)) {
    if (ballMovingUp) {
      ballMovingUp = false;
      ballMovingDown = true;
    } 
    else if (ballMovingDown) {
      ballMovingUp = true;
      ballMovingDown = false;
    }
  }  

  if ((ballX == 1) && (!ballMovingRight)) {

    if ((player1Position == ballY || (player1Position - 1 == ballY))) {
      ballMovingRight = true;
      int r = (int) (random(3));

      if (r == 0) {
        ballMovingUp = true;
        ballMovingDown = false;
      } 
      else if (r == 1) {
        ballMovingUp = false;
        ballMovingDown = true;
      } 
      else {
        ballMovingUp = false;
        ballMovingDown = false;
      }
    } 
    else if ((ballMovingDown && (player1Position - 2 == ballY)) || (ballMovingUp && (player1Position == ballY - 1))) {
      ballMovingRight = true;
      int r = (int) (random(3));

      if (r == 0) {
        ballMovingUp = true;
        ballMovingDown = false;
      } 
      else if (r == 1) {
        ballMovingUp = false;
        ballMovingDown = true;
      } 
      else {
        ballMovingUp = false;
        ballMovingDown = false;
      }
    }
  }
  else if ((ballX == 6) && (ballMovingRight)) { 

    if (((player2Position == ballY) || (player2Position - 1 == ballY))) {
      ballMovingRight = false;
      int r = (int) (random(3));

      if (r == 0) {
        ballMovingUp = true;
        ballMovingDown = false;
      } 
      else if (r == 1) {
        ballMovingUp = false;
        ballMovingDown = true;
      } 
      else {
        ballMovingUp = false;
        ballMovingDown = false;
      }
    } 
    else if ((ballMovingDown && (player2Position - 2 == ballY)) || (ballMovingUp && (player2Position == ballY - 1))) {
      ballMovingRight = false;
      int r = (int) (random(3));

      if (r == 0) {
        ballMovingUp = true;
        ballMovingDown = false;
      } 
      else if (r == 1) {
        ballMovingUp = false;
        ballMovingDown = true;
      } 
      else {
        ballMovingUp = false;
        ballMovingDown = false;
      }
    }
  } 
  else if ((ballX == 0) || (ballX == 7)) {
    gameOn = false;
  } 

  if (ballMovingUp) {
    ballY++;
  } 
  else if (ballMovingDown) {
    ballY--;
  }
  
  if (ballMovingRight) {
    ballX++;
  } 
  else {
    ballX--;
  }

  board[ballY][ballX] = 1;
}

void displayImage(int image[8][8]) {
  for (int col = 0; col < 8; col++) {
    shiftBit(col, LOW);
    for (int r = 8; r < 16; r++) { //turns off each row in the column
      shiftBit(r, HIGH);
    }
    for (int row = 8; row < 16; row++) {
      if ((image[row - 8][col]) != 0) {
        shiftBit(row, LOW);
      }
    }
    shiftBit(col, HIGH);
    shiftWrite();
    delayMicroseconds(500);
    shiftBit(col, LOW);
    //shiftWrite();
  }
}

void resetLEDs() {
  for (int i = 0 ; i < 8; i++) {
    shiftBit(i, LOW);
  }
  for (int i = 8; i < 16; i++) {
    shiftBit(i, HIGH);
  }
  shiftWrite();
}

void resetCols() {
  for (int i = 0; i < 8; i++) {
    board[i][0] = 0;
    board[i][7] = 0;
  }
}

void resetBoard() { //could make this more efficient - only reset col 0 and 7
  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      if (board[r][c] != 0) {
        board[r][c] = 0;
      }
    }
  }
}

void allOn() {
  for (int i = 0 ; i < 8; i++) {
    shiftBit(i, HIGH);
  }
  for (int i = 8; i < 16; i++) {
    shiftBit(i, LOW);
  }
  shiftWrite();
}

void shiftBit(int desiredPin, boolean desiredState) {
  if (desiredPin < 8) {
    bitWrite(dataOne, desiredPin, desiredState);
  } 
  else {
    bitWrite(dataTwo, desiredPin - 8, desiredState);
  }
}


void shiftWrite(){
  shiftOut(dataPin, clockPin, MSBFIRST, dataTwo);
  shiftOut(dataPin, clockPin, MSBFIRST, dataOne);

  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}



