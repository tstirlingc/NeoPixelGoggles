// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN 0

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;

uint32_t red = pixels.Color(85,0,0);
uint32_t green = pixels.Color(0,85,0);
uint32_t blue = pixels.Color(0,0,85);

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
  prevTime = millis();
}

void all_off() {
  for (int i=0 ; i<32 ; ++i) {
    pixels.setPixelColor(i,0);
  }
  pixels.show();
}

void right_CCW() {
  for (int i=0 ; i<16 ; ++i) {
    all_off();
    pixels.setPixelColor(i,red);
    pixels.show();
    delay(20);    
  }
}

void left_CCW() {
  for ( int i=0 ; i<16 ; ++i) {
    int j= i+16 ; 
    all_off () ; 
    pixels.setPixelColor(j,red);
    pixels.show();
    delay(20);    
  }
}


void left_right_CCW(){
  for ( int i=0 ; i<16 ; ++i) {
    int j= i+16 ; 
    all_off () ;
    pixels.setPixelColor(i,red); 
    pixels.setPixelColor(j,red);
    pixels.show();
    delay(20);    
  }
   
}

int fix_left_range(int n) {
  if ( n<16) {
    return( n+16);
  }
  if (n > 31){
    return ( n-16) ;
  }
  return (n);
}

void left_right_sync_CCW(){
  for ( int i=0 ; i<16 ; ++i) {
    int j= fix_left_range(i+16+3); 
    all_off();
    
    pixels.setPixelColor(i,red); 
    pixels.setPixelColor(j,red);
    pixels.show();
    delay(250);    
  }
}

void infinity(){
  int sequence[32] = {27, 28, 29, 30, 31, 16, 17, 18, 7, 6, 5, 4, 3, 2, 1,
  0, 15, 14, 13, 12, 11, 10, 9, 8, 19, 20, 21, 22, 23, 24, 25, 26};
  for (int i=0 ; i<32 ; ++i) { 
    all_off();
    pixels.setPixelColor(sequence[i],red);
    pixels.show();
    delay(250);
  }
}

int wrap(int n, int M) {
  int result = n;
  while (result < 0) {
    result += M;
  }
  while (result >= M) {
    result -= M;
  }
  return(result);
}

void infinity_tail(){
  int sequence[32] = {27, 28, 29, 30, 31, 16, 17, 18, 7, 6, 5, 4, 3, 2, 1,
  0, 15, 14, 13, 12, 11, 10, 9, 8, 19, 20, 21, 22, 23, 24, 25, 26};
  for (int i=0 ; i<32 ; ++i) { 
    all_off();
    pixels.setPixelColor(sequence[i],pixels.Color(255,0,0));
    for (int j=1 ; j<5 ; ++j) {
      pixels.setPixelColor(sequence[wrap(i-j,32)], pixels.Color(85,0,0));
    }
    pixels.show();
    delay(10);
  }
}

int sequence[32] = {27, 28, 29, 30, 31, 16, 17, 18, 7, 6, 5, 4, 3, 2, 1,
  0, 15, 14, 13, 12, 11, 10, 9, 8, 19, 20, 21, 22, 23, 24, 25, 26};

void snake(int head, int tail_length, uint32_t color_head, uint32_t color_tail) {
  pixels.setPixelColor(sequence[head],color_head);
    for (int j=1 ; j<tail_length+1 ; ++j) {
      pixels.setPixelColor(sequence[wrap(head-j,32)], color_tail);
    }
}
void infinity_tails(){

  for (int i=0 ; i<32 ; ++i) { 
    int j=wrap(i-8,32);
    all_off();
    snake(i,3, pixels.Color(255,0,0), pixels.Color(85,0,0));
    snake(j,2, pixels.Color(0,255,0), pixels.Color(0,85,0));
    pixels.show();
    delay(50);
  }
}

void identify_0_16() {
  pixels.setPixelColor(0,red);
  pixels.setPixelColor(1,green);
  pixels.setPixelColor(16,blue);
  pixels.setPixelColor(17,green);
  pixels.show();
}
void loop() {
//  right_CCW();
//  left_CCW();
//  left_right_CCW();
//  left_right_sync_CCW();
//  infinity ();
//  infinity_tail();
  infinity_tails();
//identify_0_16();
}

  
//void loop() {
//  uint8_t  i;
//  uint32_t t;
//
//  switch(mode) {
//
//   case 0: // Random sparks - just one LED on at a time!
//    i = random(32);
//    pixels.setPixelColor(i, color);
//    pixels.show();
//    delay(10);
//    pixels.setPixelColor(i, 0);
//    break;
// 
//   case 1: // Spinny wheels (8 LEDs on at a time)
//    for(i=0; i<16; i++) {
//      uint32_t c = 0;
//      if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
//      pixels.setPixelColor(   i, c); // First eye
//      pixels.setPixelColor(31-i, c); // Second eye (flipped)
//    }
//    pixels.show();
//    offset++;
//    delay(50);
//    break;
//  }
//
//  t = millis();
//  if((t - prevTime) > 8000) {      // Every 8 seconds...
//    mode++;                        // Next mode
//    if(mode > 1) {                 // End of modes?
//      mode = 0;                    // Start modes over
//      color >>= 8;                 // Next color R->G->B
//      if(!color) color = 0xFF0000; // Reset to red
//    }
//    for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
//    prevTime = t;
//  }
//}
