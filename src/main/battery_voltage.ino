
// バッテリ電圧の監視
float battery_voltage(uint8_t pin){// batt1 or batt2
  
  return 3.3 / 1024 * 2 * analogRead(pin);
}