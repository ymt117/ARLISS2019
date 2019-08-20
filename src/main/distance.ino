#include "cansat_define.h"

// 2点の緯度/経度から距離を計算
// https://keisan.casio.jp/exec/system/1257670779
double distance(float lng1, float lat1, float lng2, float lat2){// x1, y1, x2, y2
  double x1, y1, x2, y2;
  double d;
  double sin_y1, sin_y2, cos_y1, cos_y2, cos_dx, arc_cos;
  double delta_x;
  double r = 6378137;

  x1 = ToRad(lng1);
  y1 = ToRad(lat1);
  x2 = ToRad(lng2);
  y2 = ToRad(lat2);
  delta_x = x1 - x2;

  sin_y1 = sin(y1);
  sin_y2 = sin(y2);
  cos_y1 = cos(y1);
  cos_y2 = cos(y2);
  cos_dx = cos(delta_x);

  arc_cos = acos(sin_y1 * sin_y2 + cos_y1 * cos_y2 * cos_dx);

  d = r * arc_cos;

  return d;
}