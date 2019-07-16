
// BOOT_UP
float mm1[] = {mE*4, mA*4, mB*4, mA*4, mE*2, mE*4, mB*4};

// SHUT_DOWN
float mm2[] = {mA*4, mE*4, mA*2, mB*4};

// Dango Big Family
float mm3[] = {mE*4, mD*4, mG*4, nn, mG*4, mG*4, mA*4, nn,
               mA*4, mA*4, mB*4, nn, mG*4, mG*4, mD*4, nn,
               mE*4, mD*4, mG*4, nn, mG*4, mG*4, mA*4, nn,
               mA*4, mA*4, mB*4, mB*4, mG*4};

// CANSAT_ERROR
float mm4[] = {mB*4, mB*4, mB*4, mB*4};

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
  else{
    // do nothing
  }
}
