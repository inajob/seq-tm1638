#include "MultiTunes.h"
#include <TM1638lite.h>

TM1638lite tm(10,13,11);

#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988

volatile unsigned int tones[]= {
NOTE_C3,NOTE_CS3,
NOTE_D3,NOTE_DS3,
NOTE_E3,
NOTE_F3,NOTE_FS3,
NOTE_G3,NOTE_GS3,
NOTE_A3,NOTE_AS3,
NOTE_B3,
NOTE_C4,NOTE_CS4,
NOTE_D4,NOTE_DS4,
NOTE_E4,
NOTE_F4,NOTE_FS4,
NOTE_G4,NOTE_GS4,
NOTE_A4,NOTE_AS4,
NOTE_B4,
NOTE_C5,NOTE_CS5,
NOTE_D5,NOTE_DS5,
NOTE_E5,
NOTE_F5,NOTE_FS5,
NOTE_G5,NOTE_GS5,
NOTE_A5,NOTE_AS5,
NOTE_B5
};

char magic[] = {
0 , 2, 4, 5, 7, 9,11,
12,14,16,17,19,21,23,
24,26,28,29,31,33,35
};

byte rseq[]= {
  0x3f,
  0x3f,
  0x3f | 0x80,
  0,
  0x3f,
  0x3f,
  0x3f | 0x80,
  0
  };

int trigger[8];
byte cursor = 0;

#define SAMPLE 64

void mkWave(byte type){
  for(byte i=0;i < 16; i++){
    for(byte j=0;j < SAMPLE; j++){
      switch(type){
        case 0:
          wave[i][j] = i*j/(SAMPLE); // i : vol, j : t
          break;
        case 1:
          wave[i][j] = 2 * i * ((j & (SAMPLE>>1))?8:0)/(16+8);  // 8 : 0B100
          break;
        case 2:
          wave[i][j] = i*((sin(j*2*3.14/SAMPLE)+1)/2 * 8)/16;
          break;
      }
    }
  }
}

void setup(){
  for(byte i = 0; i < 8; i ++){
    trigger[i] = 0;
  }

  tm.reset();
  tm.displayText("Hello");

  d[0] = 400;
  vol[0] = 8;

  mkWave(0);
  soundSetup();
  tm.displayText("CHEAPSEQ");
}


unsigned int counter = 0;
unsigned int counter2 = 0;
void loop(){
  counter ++;

  if(vol[0] > 0) vol[0] --;
  if(nf != 0){
    nf = (nf >> 1);
  }
  if(nf2 != 0){
    nf2 = (nf2 >> 1);
  }


  if(counter%20 == 0){
    cursor = (cursor + 1)%8;

    if(rseq[cursor]){
      if((rseq[cursor] & 0x80) == 0x80){
        nf2 = rseq[cursor] & 0x7f;
      }else{
        nf = rseq[cursor];
      }
    }

  }

  //if(counter2 == 0){
  //  d[0] = 400;
  //  vol[0] = 12;
  //}
  counter2 ++;

  uint8_t buttons = tm.readButtons();
  for(uint8_t position = 0; position < 8; position ++){
    if((buttons & (1 << position)) != 0){
      // button press
      tm.setLED(position, true);
      d[0] = (tones[magic[position]] << 1);
      if(trigger[position] == 0){
        // on push
      }
      if(trigger[position] < 256){
        trigger[position] ++;
      }
      vol[0] = 12;
    }else{
      tm.setLED(position, false);
      if(trigger[position] > 0){
        //on release
      }
      trigger[position] = 0;
    }
  }
  tm.setLED(cursor, true);

}
