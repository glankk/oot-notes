
/* 800AA480 */
uint16_t f800AA480(float y, float x)
{
  uint16_t *atan_tab = (void*)0x800FE5A0;
  if (x == 0.)
    return atan_tab[0];
  else {
    int index = y / x * 1024. + 0.5;
    if (index < 0x0401)
      return atan_tab[index];
    else
      return atan_tab[0];
  }
}

/* 800AA4F8 */
int16_t atan2(float x, float y)
{
  if (y >= 0) {
    if (x >= 0) {
      if (x >= y)
        return f800AA480(y, x);
      else
        return 0x4000 - f800AA480(x, y);
    }
    else {
      if (-x < y)
        return 0x4000 + f800AA480(-x, y);
      else
        return 0x8000 - f800AA480(y, -x);
    }
  }
  else {
    if (x < 0) {
      if (-x >= -y)
        return 0x8000 + f800AA480(-y, -x);
      else
        return 0xC000 - f800AA480(-x, -y);
    }
    else {
      if (x < -y)
        return 0xC000 + f800AA480(x, -y);
      else
        return -f800AA480(-y, x);
    }
  }
}
