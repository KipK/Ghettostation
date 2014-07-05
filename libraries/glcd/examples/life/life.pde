/* 
 * life.pde
 * John Conway's Game of Life for Arduino GLCD
 * 
 */

#include <glcd.h>
#include "fonts/SystemFont5x7.h"       // system font
 
/********* user configurable defines ************/
#define CELL_SIZE 4        // the height and width of each cell
#define DELAY    50        // number of milliseconds delay between the display of each generation

/******* end of user configurable defines ********/
#define ROWS    (DISPLAY_HEIGHT / CELL_SIZE)
#define COLUMNS  (DISPLAY_WIDTH / CELL_SIZE)

#define BYTES_PER_COLUMN ((COLUMNS + 7)/ 8)    //this is the number of bytes needed to hold a column

#define MAX_ITERATIONS  500  // iterations are terminated if this number is exceeded
#define STABLE_GENERATIONS 4  // must be at least 2, this is the number of back generations checked for stability

#define MIN_OBJECTS 2     // minmum number of objects to be seeded by the random generater 
#define MAX_OBJECTS 4     // maximum number of objects for random seed generator
#define MARGIN 4          // minimum distance from screen edge for randomly seeded objects 


#define BITVAL(_pos) (1<< (7-(_pos % 8)))  // macro to get a bitfield value

#define ALIVE true
#define DEAD false

/* ========   Life Objects   ========
 * these objects  can be displayed on the Arduino life screen
 * to add an object, create and initialise a structure following the examples here
 * add a reference to your structure to the object table, name your object by adding to the enum list        
 */

// structure template for life objects
typedef struct {
         byte height ; 
         byte width ;  
         byte bitfield[];
     } object_t, *object_ptr_t;

// definitions and initialisations for all defined objects here
struct {
         byte height ;
         byte width ;
         byte bitfield[2];
       } table = {2,4,  { 0b10010000 ,       //  *  *  
                          0b11110000 } };    //  **** 
                          
struct {
         byte height ; 
         byte width ;  
         byte bitfield[3];
       } glider  = {3,3, { 0b00100000 ,      //    *    
                           0b10100000 ,      //  * *
                           0b01100000 }};    //   **
                          
struct {
         byte height ; 
         byte width ;  
         byte bitfield[3];
       } glider2 = {3,3, { 0b10100000 ,      //  * *
                           0b11000000 ,      //  **
                           0b11110000 }};    //  ****                      
struct {
         byte height ; 
         byte width ;  
         byte bitfield[4];
       } loaf = {4,4,    { 0b01100000 ,     //   **
                           0b10010000 ,     //  *  *  
                           0b01010000 ,     //   * * 
                           0b00100000 }};   //    *                      
struct {
         byte height ; 
         byte width ;  
         byte bitfield[3];
       } ship = {3,3,    { 0b11000000 ,     //   **
                           0b10100000 ,     //   * *  
                           0b01100000 }};   //    **

struct {
         byte height ; 
         byte width ;  
         byte bitfield[4];
       } behive = {4,3,  { 0b01000000 ,     //   *    
                           0b10100000 ,     //  * *
                           0b10100000 ,     //  * * 
                           0b01000000}};    //   *
                           
struct {
         byte height ; 
         byte width ;  
         byte bitfield[1];
       } blinker = {1,3, { 0b11100000 }};   //  *** 
                          
                         
struct {
         byte height ; 
         byte width ;  
         byte bitfield[2];
       } block = {2,2, { 0b11000000 ,      //  **
                         0b11000000}};     //  **                     

// place all the objects in the object table                         
 object_ptr_t objectTable[] = {
             (object_ptr_t)&table,
             (object_ptr_t)&glider,
             (object_ptr_t)&loaf,
             (object_ptr_t)&ship,
             (object_ptr_t)&behive,
             (object_ptr_t)&blinker,
             (object_ptr_t)&block
 };   

#define OBJECT_COUNT (sizeof( objectTable/ sizeof(object_ptr_t)))

// enumerate all the objects, add any new objects to the end of this enum (note case difference )
// the life program references all objects using the names in this list
 enum  {Table,Glider,Loaf,Ship,Behive,Blinker,Block};

byte generations[STABLE_GENERATIONS][ROWS][BYTES_PER_COLUMN]; // array used to calculate, draw and check if iterations are changing     

void setup(){     
  GLCD.Init();   // initialise the library, non inverted writes pixels onto a clear screen
  GLCD.SelectFont(System5x7);
  seed(0);
}

void loop (void){     
  GLCD.ClearScreen();
  unsigned int i = generate();  
  GLCD.CursorTo(0,0); 
  GLCD.print(i);
  GLCD.print(" iterations");  
  delay(1500);   
  seed(1); // use random seed
}   

unsigned int generate(){
  unsigned int iteration = 0;
  byte thisGeneration,nextGeneration, previousGeneration;  
  thisGeneration = nextGeneration  = 0;
  //display the initial array
  for(int row = 0; row < ROWS; row++)
  {
      for(int column = 0; column < COLUMNS; column++)
      {
        if(isAlive(thisGeneration,row,column))     
           GLCD.DrawRoundRect(column * CELL_SIZE, row * CELL_SIZE,  CELL_SIZE-1, CELL_SIZE-1, CELL_SIZE/2, BLACK) ; 
        else
           GLCD.DrawRoundRect(column * CELL_SIZE, row * CELL_SIZE,  CELL_SIZE-1, CELL_SIZE-1, CELL_SIZE/2, WHITE) ; 
      }
  }

  delay(1500); // show the seeded condition a little longer 
  do{    
    thisGeneration  = iteration % STABLE_GENERATIONS;
    nextGeneration =  (thisGeneration+1) % STABLE_GENERATIONS;
    previousGeneration  =  (thisGeneration-1) % STABLE_GENERATIONS;
    delay(DELAY);     
    memset(generations[nextGeneration],0,sizeof(generations[0])); // clear the array for the next generation
    //see who lives and who dies
    for(int row = 0; row < ROWS; row++){       
      for(int column = 0; column < COLUMNS; column++){
        boolean cell =  isAlive(thisGeneration,row,column);
        byte n = getNeighborCount(thisGeneration,row,column);
        if(cell == DEAD){
          if(n==3) {
            setLife(nextGeneration,row,column,ALIVE); // birth     
            GLCD.DrawRoundRect(column * CELL_SIZE, row * CELL_SIZE,  CELL_SIZE-1, CELL_SIZE-1, CELL_SIZE/2, BLACK) ; 
          }
        }
        else{
          if(n==2 || n==3) {
            setLife(nextGeneration,row,column,ALIVE);  //survival
            // No need to draw cell as it is already there.
          } else { 
            setLife(nextGeneration,row,column,DEAD); //death 
            GLCD.DrawRoundRect(column * CELL_SIZE, row * CELL_SIZE,  CELL_SIZE-1, CELL_SIZE-1, CELL_SIZE/2, WHITE) ; 
          }
        }
      }
    } 
  }
  while(isStable(thisGeneration) == false  && ++iteration < MAX_ITERATIONS ) ;
  return iteration;
}

void seed(int seedValue){
  // a hard coded seed is used if seedvalue is zero, otherwise a random seed is used
  memset(generations,0,sizeof(generations)); // clear all generation arrays    
  if(seedValue == 0){
    // load default objects,  arguments are offsets from the center of the screen
    loadObject(Table,2,-9);    // put a table object 2 down from the center and 9 columns left or center     
    loadObject(Glider, -5,-9);   // put a glider 5 rows above the center and 9 columns left of center    
  }
  else{  
  // load some random objects and place at random positions 
     byte nbrObjects = random(MIN_OBJECTS,MAX_OBJECTS+1);  // make a random number of objects
     for(byte i=0; i < nbrObjects;i++){
        byte obj = random(7); // OBJECT_COUNT);
        int column = random(MARGIN - (COLUMNS/2), (COLUMNS/2) - MARGIN); 
        int row = random(MARGIN - (ROWS/2)   , (ROWS/2) - MARGIN); 
        loadObject(obj,row,column);
     }  
  }  
}

void loadObject(void * object, char y, char x){
  // object_ptr is the object to be loaded 
  // y and x are offsets from the center of the display
  object_ptr_t object_ptr = (object_ptr_t)object; 
  byte row = (ROWS/2) + y;
  byte column = (COLUMNS/2) + x;   
  for(byte ys = 0; ys < object_ptr->height; ys++ ){
    for(byte xs=0; xs < object_ptr->width; xs++ ){
      if( (row +ys < ROWS) && (column +xs < COLUMNS)){
        // this code only works on widths up to 8 bits 
        boolean value = object_ptr->bitfield[ys] & BITVAL(xs) ;   
        setLife(0, row+ys, column+xs, value) ; // objects always loaded into the first array       
      }   
    }          
  }      
}

void loadObject(int object, char y, char x){
  loadObject(objectTable[object], y, x );
}

boolean isStable(byte thisGeneration){
  // returns true if any two captured generations are the same
  for(byte i=0; i < STABLE_GENERATIONS; i++)
    if(i != thisGeneration) 
      if(memcmp(generations[thisGeneration], generations[i], sizeof(generations[0])) == 0)
        return true;
  return false;        
} 

boolean isAlive( byte generation, byte row, byte column){
  byte  b = generations[generation][row][column /8];
  return b &  BITVAL(column) ;   
}

void setLife( byte generation, byte row, byte column, boolean state){
  byte elem = column /8;  
  byte  b = generations[generation][row][elem];
  if(state != DEAD){
    b = b | BITVAL(column);
  }
  else{
    b &= ~(BITVAL(column)) ;  
  }
  generations[generation][row][elem] = b;
}

byte getNeighborCount( byte generation, byte row, byte column){
  byte count = 0;
  for(byte d = 0;d < 8; d++)
    if(isNeighborAlive(generation,row,column,d) != 0)
      count++;
  return count;
}

boolean isNeighborAlive( byte generation , byte row, byte column, byte dir){
  byte nrow=row;
  byte ncol=column;
  if(dir == 7 || dir == 0 || dir == 1) 
    nrow--;
  if(dir == 3 || dir == 4 || dir == 5) 
    nrow++;
  if(dir == 5 || dir == 6 || dir == 7) 
    ncol--;
  if(dir == 1 || dir == 2 || dir == 3) 
    ncol++;
  if(ncol==255) ncol = COLUMNS - 1;
  if(ncol==COLUMNS) ncol = 0;
  if(nrow==255) nrow = ROWS - 1;
  if(nrow==ROWS) nrow = 0;
  return isAlive(generation,nrow,ncol);
}