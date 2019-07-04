#define mC 261.626
#define mD 293.665
#define mE 329.628
#define mF 349.228
#define mG 391.995
#define mA 440.000
#define mB 493.883
#define nn 0.0

#define BEAT_TIME 200

#define BOOT_UP   1
#define SHUT_DOWN 2
#define CLANNAD   3

static const int sp = PB12;

// BOOT_UP
float mm1[] = {mE*4, mA*4, mB*4, mA*4, mE*2, mE*4, mB*4};

// SHUT_DOWN
float mm2[] = {mA*4, mE*4, mA*2, mB*4};

// Dango Big Family
float mm3[] = {mE*4, mD*4, mG*4, nn, mG*4, mG*4, mA*4, nn,
               mA*4, mA*4, mB*4, nn, mG*4, mG*4, mD*4, nn,
               mE*4, mD*4, mG*4, nn, mG*4, mG*4, mA*4, nn,
               mA*4, mA*4, mB*4, mB*4, mG*4};

void setup(){
  pinMode(sp, OUTPUT);
  beep(BOOT_UP);
}

void loop(){
  
}

void _beep(float *mm, int m_size, int b_time){
  
  for(int i=0; i<m_size; i++){
    tone(sp, mm[i], b_time);
    delay(b_time);
  }
  noTone(sp);
}

void beep(int mode){
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
  else{
    // do nothing
  }
}
