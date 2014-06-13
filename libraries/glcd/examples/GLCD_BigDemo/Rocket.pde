/*
 * Rocket
 *
 * An asteroid type game made by biegematchbox
 * http://www.beigematchbox.co.uk/site/BeigeMatchbox.co.uk/Electronics/Electronics.html
 *
 * Connect a variable resistor to analog input pin (potPin) to control rocket position
 * A piezo (or speaker through a series resistor) can be connected to (speakerPin) for sound 
 *
 * Arduino forum discussion thread here: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1265067548
 *
 */


#include <glcd.h>
#include "fonts/SystemFont5x7.h"   // font
#include "bitmaps.h"         // bitmaps

/*
******************************
*  A few pin defines         *
******************************
*/

#include "BigDemo.h"

//#define brightnessPin 3   // optional output that can be used to control backlight

byte brightness = 64;

#define PLAYER 0
#define ROCK1 1
#define ROCK2 2
#define ROCK3 3
#define ROCK4 4
#define ROCK5 5
#define ROCK6 6
#define ROCK7 7
#define ROCK8 8
#define BONUS 9

#define EX1 0
#define EX2 1
#define EX3 2


#define playerX tracker[0][1]
#define playerY tracker[0][2]

#define enemy1type tracker[1][0]
#define enemy1X tracker[3][1]
#define enemy1Y tracker[4][2]

#define playerXOld trackerOld[0][1]
#define playerYOld trackerOld[0][2]
#define enemy1XOld trackerOld[3][1]
#define enemy1YOld trackerOld[4][2]

char lives = 2;
char score = 0;
char level = 0;
char fastSpeed = 25;
char rockAmount = 6;

long speedMillis[] = {0, 0, 0, 0, 0, 0};

char tracker[8][5] = {
// Bitmap,  Xpos,   Ypos,          Speed
  {PLAYER,  15,     32,            0}  ,
  {ROCK2,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK5,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK8,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK1,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK3,   119,    random(0, 56), random(25, 75)}  ,
  {BONUS,   119,    random(0, 56), random(25, 75)}
};

char trackerOld[8][5] = {
// Bitmap,  Xpos,   Ypos,          Speed
  {PLAYER,  15,     32,            0}  ,
  {ROCK2,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK5,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK8,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK1,   119,    random(0, 56), random(25, 75)}  ,
  {ROCK3,   119,    random(0, 56), random(25, 75)}  ,
  {BONUS,   119,    random(0, 56), random(25, 75)}
};

void rocket(int duration)
{
#ifdef speakerAPin
  pinMode(speakerAPin, OUTPUT);
#endif
  GLCD.SelectFont(System5x7);
#ifdef potPin
  randomSeed(analogRead(potPin));
#endif
  GLCD.DrawBitmap(startup, 0, 0, BLACK);
  delay(2000);
  GLCD.ClearScreen();
  for(unsigned long start = millis();  millis() - start < duration; )
  {
    mainLoop();
  }  
}

/*
******************************
*  Main loop                 *
*                            *
******************************
*/
void  mainLoop(){
  getControls();
  drawFrame();
  drawLives();
  drawScore(); 
}


/*
******************************
*  Draw the remaining lives  *
*                            *
******************************
*/
void drawLives(){
  GLCD.DrawBitmap(heart, 0, 0, BLACK);
  GLCD.CursorTo(1,0);
  GLCD.print((int)lives);
}


/*
******************************
*  Draw the current score    *
*                            *
******************************
*/
void drawScore(){
  GLCD.CursorTo(0,7);
  GLCD.print('S');
  GLCD.print((int)score);
}



/*
******************************
*  Get the cuttent stick     *
*  position                  *
******************************
*/
void getControls(){
#ifdef potPin
  playerY = map(analogRead(potPin), 0, 1023, 0, 56);
#else
/*
 * Not pot input so move the rocket up and down
 */
static uint8_t yval;
static uint8_t iter = 0;
static int8_t dir = 1;
  if(++iter == 0)
    yval += dir;

  if(yval == 0)
    dir = 1;

  if(yval == 56)
    dir = -1;
  playerY = yval;
#endif
}



/*
******************************
*  Update the position of    *
*  the rocks                 *
******************************
*/
void updatePos(int entity){
    if (millis() - speedMillis[entity-1] > tracker[entity][3]) {
      speedMillis[entity-1] = millis();
      tracker[entity][1] --;
      
        if(collision(entity) == true){
          BLOWUP();
        }
      
      if(tracker[entity][1] < 1){
        score++;
        level++;
        constrain(level, 0, 75);
        if(level > 75){
          fastSpeed --;
          constrain(fastSpeed, 10, 25);
        }
        
        tracker[entity][0] = random(1, 8);   // bitmap
        tracker[entity][1] = 119;            // start point
        tracker[entity][2] = random(8, 56);  // height
        tracker[entity][3] = random(fastSpeed, 100-level); // speed
      }  
  }
}


/*
******************************
*  Detect collisions         *
* *False triggers*           *
******************************
*/
boolean collision(char entity){
  for(char playerScanCOL = 0; playerScanCOL<=9; playerScanCOL++){
    if(tracker[0][1]+playerScanCOL == tracker[entity][1]+1){
      for(char playerScanROW = -1; playerScanROW < 6; playerScanROW  ++){
      for(char entityScanROW = -1; entityScanROW < 6; entityScanROW  ++){
        if(tracker[0][2]+playerScanROW == tracker[entity][2]+entityScanROW){
          return true;
         }
      }
     }
    }
  }
  return false;
}


/*
******************************
*  Blow up a ship            *
*                            *
******************************
*/
void BLOWUP(){
  lives--;
  digitalWrite(13, HIGH);
  explosionAnim(tracker[0][1]-2, tracker[0][2]-4);
  digitalWrite(13, LOW);
  GLCD.ClearScreen();
  if(lives < 0){
    gameOver();
    lives = 3;
    level = 0;
  }
  resetTracker();
}


/*
******************************
*  Reset the tracker to a    *
*  fresh state               *
******************************
*/
void resetTracker(){
  char trackerReset[8][5] = {
  {PLAYER, 15, 32, 0}  ,
  {ROCK2,   119, random(0, 56), random(25, 75)}  ,
  {ROCK5,   119, random(0, 56), random(25, 75)}  ,
  {ROCK8,   119, random(0, 56), random(25, 75)}  ,
  {ROCK1,   119, random(0, 56), random(25, 75)}  ,
  {ROCK3,   119, random(0, 56), random(25, 75)}  ,
  {BONUS,   119, random(0, 56), random(25, 75)}
  };
  
  for(byte entity = 0; entity <= 6; entity++){
    for(byte data = 0; data <= 3; data++){
      tracker[entity][data] = trackerReset[entity][data];
    }
  }
}


/*
******************************
*  Draw GAME OVER and reset  *
*  the score                 *
******************************
*/
void gameOver(){
  GLCD.DrawBitmap(gameover, 37, 0, BLACK);
  delay(5000);
  score = 0;
  GLCD.ClearScreen();
}

/*
******************************
* Animated 3 frame explosion *
* Needs the frams in 1 array *
******************************
*/
void explosionAnim(char xPos, char yPos){
  GLCD.DrawBitmap(ex1, xPos, yPos, BLACK);
#ifdef speakerAPin
  noise(175);
#else
  delay(175);
#endif
  GLCD.DrawBitmap(ex2, xPos, yPos, BLACK);
#ifdef speakerAPin
  noise(175);
#else
  delay(175);
#endif
  GLCD.DrawBitmap(ex3, xPos, yPos, BLACK);
#ifdef speakerAPin
  noise(175);
#else
  delay(175);
#endif

}


/*
******************************
*  Draw a frame, player and  *
*  rocks                     *
******************************
*/
void drawFrame(){
  for(int i=1; i<=rockAmount; i++){
    updatePos(i);
    drawRock(i);
  }
  if(playerY != playerYOld){
    GLCD.FillRect(playerXOld, playerYOld, 10, 8, WHITE);
    GLCD.DrawBitmap(player, playerX, playerY, BLACK);
  }

  for(byte entity = 0; entity <= 6; entity++){
    for(byte data = 0; data <= 3; data++){
      trackerOld[entity][data] = tracker[entity][data];
    }
  }
}


/*
******************************
*  Draw rocks, way too much  *
*  code...                   *
******************************
*/
void drawRock(char entity){
// Rock bitmaps really need to be in a 2D array, will cut down on the below
  
  if(tracker[entity][1] != trackerOld[entity][1]){

    GLCD.FillRect(trackerOld[entity][1], trackerOld[entity][2], 12, 8, WHITE);
    GLCD.DrawBitmap(rocks[tracker[entity][0]], tracker[entity][1], tracker[entity][2], BLACK);
  }
}

#ifdef speakerAPin
/*
******************************
*  make noise for the given
*  duration in milliseconds
******************************
*/
void noise(int duration)
{
  unsigned long start = millis();
  
  while( millis() - start < duration)
  {
    int period = random(8,25); 
    digitalWrite(speakerAPin, HIGH);
    delay(period);
    digitalWrite(speakerAPin, LOW);      
    delay(period);
  }
}  
#endif
