#ifndef UTIL_H
#define UTIL_H



double sgn(double x);

void RotatePoints(size_t n, double *px, double *py, double cx, double cy, double a);



#ifdef UTIL_IMPLEMENTATION_H

double sgn(double x)
{
    if (x > 0.0)
        return  1.0;
    if (x < 0.0)
        return -1.0;
    return x;
}

void RotatePoints(size_t n, double *px, double *py, double cx, double cy, double a) {
  double cs = cos(a);
  double sn = sin(a);
  double temp;
  for (int i = 0; i < n; i++) {
    temp = ((px[i] - cx) * cs - (py[i] - cy) * sn) + cx;
    py[i] = ((px[i] - cx) * sn + (py[i] - cy) * cs) + cy;
    px[i] = temp;
  }
}



#endif // UTIL_IMPLEMENTATION_H

#endif // UTIL_H
