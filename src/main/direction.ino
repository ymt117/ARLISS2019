#include "cansat_define.h"

// 2点の緯度/経度から方位を計算
// http://kwikwi.cocolog-nifty.com/blog/2005/12/delphi_2_fd2c.html
double direction(float lng1, float lat1, float lng2, float lat2){
  double x1, y1, x2, y2;
  double phi;
  double X, Y;
  double sin_y1, sin_y2, cos_y1, cos_y2;
  double sin_dx, cos_dx;
  double delta_x;

  x1 = ToRad(lng1);
  y1 = ToRad(lat1);
  x2 = ToRad(lng2);
  y2 = ToRad(lat2);
  delta_x = x2 - x1;

  sin_y1 = sin(y1);
  sin_y2 = sin(y2);
  cos_y1 = cos(y1);
  cos_y2 = cos(y2);
  sin_dx = sin(delta_x);
  cos_dx = cos(delta_x);

  Y = cos_y2 * sin_dx;
  X = cos_y1 * sin_y2 - sin_y1 * cos_y2 * cos_dx;

  phi = atan2(Y, X);

  phi = ToDeg(phi);
  if(phi < 0) phi += 360;

  return phi;
}