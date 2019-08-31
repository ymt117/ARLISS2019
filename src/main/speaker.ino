#include "cansat_define.h"

//#define BEEP_OFF

#define mC 261.626
#define mD 293.665
#define mE 329.628
#define mF 349.228
#define mG 391.995
#define mA 440.000
#define mB 493.883
#define nn 0.0

// BOOT_UP
float mm1[] = {mE*4, mA*4, mB*4, mA*4, mE*2, mE*4, mB*4};
// SHUT_DOWN
float mm2[] = {mA*4, mE*4, mA*2, mB*4};
// Dango Big Family ♪だんご大家族
float mm3[] = {mE*4, mD*4, mG*4, nn,   mG*4, mG*4, mA*4, nn,       // ミレソ ソッラ
               mA*4, mA*4, mB*4, nn,   mG*4, mG*4, mD*4, nn,       // ラッシ ソッレ
               mE*4, mD*4, mG*4, nn,   mG*4, mG*4, mA*4, nn,       // ミレソ ソッラ
               mA*4, mA*4, mB*4, mB*4, mG*4, nn,                   // ラッシシソ
               mE*4, mD*4, mG*4, nn,   mG*4, mG*4, mA*4, nn,       // ミレソ ソッラ
               mA*4, mA*4, mB*4, nn,   mG*4, mG*4, mD*4, nn,       // ラッシ ソッレ
               mE*4, mD*4, mG*4, nn,                               // ミレソ
               mG*4, mG*4, mA*4, mA*4, mG*4, nn,   nn,             // ソッララソ
               mE*4, mD*4, mG*4, nn,   mG*4, nn,   mA*4, nn,       // ミレソ ソーラ
               mA*4, nn,   mB*4, nn,   mG*4, nn,   mA*4, nn,       // ラーシ ソーレ
               mE*4, mD*4, mG*4, nn,   mG*4, nn,   mA*4, nn,       // ミレソ ソーラ
               mA*4, nn,   mB*4, nn,   mA*4, nn,   nn,             // ラーシ ラ
               mE*4, mD*4, mG*4, nn,   mG*4, nn,   mA*4, nn,       // ミレソ ソーラ
               mA*4, nn,   mB*4, nn,   mG*4, nn,   mA*4, nn,       // ラーシ ソーレ
               mE*4, mD*4, mG*4, nn,   mG*4, nn,   mA*4, mG*4, nn, // ミレソ ソーラ ソ
               };
// CANSAT_ERROR: Failed initialize SD Card ♪シシシシ
float mm4[] = {mB*4, mB*4, mB*4, mB*4};
// PUSHED
float mm5[] = {mD*4};
// CANSAT_ERROR2: Failed initialize IMU ♪ララララ
float mm6[] = {mA*4, mA*4, mA*4, mA*4};
// LAUNCH_COMPLETE: complete launch detection ♪ド レ ミ(低音)
float mm7[] = {mC*2, mD*2, mE*2};
// RELEASE_COMPLETE: complete release detection ♪ミ ファ ソ(低音)
float mm8[] = {mE*2, mF*2, mG*2};
// DROP_COMPLETE: complete drop detection ♪ソ ラ シ
float mm9[] = {mG*2, mA*2, mB*2};
// GPS_POSITIONING: GPS sensor positioning ♪ド レ ミ(高音)
float mm10[] = {mC*4, mD*4, mE*4};
// DD_CALCLATION: distance and direction calclation ♪ミ ファ ソ(高音)
float mm11[] = {mE*4, mF*4, mG*4};

void speaker_init(){
  pinMode(sp, OUTPUT);
}

void _beep(float *mm, int m_size, int b_time){
  
  for(int i=0; i<m_size; i++){
    tone(sp, mm[i], b_time);
    delay(b_time);
  }
  noTone(sp);
}

void beep(int mode){
#ifdef BEEP_OFF
  return;
#endif

  if(mode == BOOT_UP){
    int m_size = sizeof(mm1)/sizeof(float);
    _beep(mm1, m_size, 150);
  }
  else if(mode == SHUT_DOWN){
    int m_size = sizeof(mm2)/sizeof(float);
    _beep(mm2, m_size, 150);
  }
  else if(mode == CLANNAD){
    int m_size = sizeof(mm3)/sizeof(float);
    _beep(mm3, m_size, 200);
  }
  else if(mode == CANSAT_ERROR){
    int m_size = sizeof(mm4)/sizeof(float);
    _beep(mm4, m_size, 100);
  }
  else if(mode == PUSHED){
    int m_size = sizeof(mm5)/sizeof(float);
    _beep(mm5, m_size, 150);
  }
  else if(mode == CANSAT_ERROR2){
    int m_size = sizeof(mm6)/sizeof(float);
    _beep(mm6, m_size, 100);
  }
  else if(mode == LAUNCH_COMPLETE){
    int m_size = sizeof(mm7)/sizeof(float);
    _beep(mm7, m_size, 150);
  }
  else if(mode == RELEASE_COMPLETE){
    int m_size = sizeof(mm8)/sizeof(float);
    _beep(mm8, m_size, 150);
  }
  else if(mode == DROP_COMPLETE){
    int m_size = sizeof(mm9)/sizeof(float);
    _beep(mm9, m_size, 150);
  }
  else if(mode == GPS_POSITIONING){
    int m_size = sizeof(mm10)/sizeof(float);
    _beep(mm10, m_size, 150);
  }
  else if(mode == DD_CALCULATION){
    int m_size = sizeof(mm11)/sizeof(float);
    _beep(mm11, m_size, 150);
  }
  else{
    // do nothing
  }
}
