
// 静荷重試験用プログラム
void static_load(){
  char buf[1024];
  imu.read();
  unsigned long _millis = millis();

  float a = calc_g_force(imu.a.x, imu.a.y, imu.a.z);
  Serial.print(_millis); Serial.print("\t\t");
  Serial.print(a);

  if(a >= 10){
    Serial.print("\t\tOver 10G!!!");
  }

  Serial.println("");
  
  String str = "";
  str += _millis;  str += ",";
  str += imu.a.x;   str += ",";
  str += imu.a.y;   str += ",";
  str += imu.a.z;   str += ",";
  str += a;
  str += "\n";

  int len = str.length();
  str.toCharArray(buf, len+1);

  writeFile("acc_g.csv", buf);
}
