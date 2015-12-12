
/* update obstacles */
void obstacles (int obsnum, int trirng[],
                iREAL point[], iREAL angular[], iREAL linear[],
                int trinum, iREAL * tri[3][3], iREAL step)
{
    iREAL DL[9], y[3];
    iREAL * x = point, *o = angular, *v = linear;

  for (uniform int i = 0; i < obsnum; i ++, x += 3, o += 6, v += 6)
  {
    y[0] = x[0] + step*v[0];
    y[1] = x[1] + step*v[1];
    y[2] = x[2] + step*v[2];

    expmap (step*o[0], step*o[1], step*o[2], DL[0], DL[1], DL[2], DL[3], DL[4], DL[5], DL[6], DL[7], DL[8]);

    for(int j = trirng[2];j<trirng[3];j++)
    {
      iREAL z[3];

      z[0] = tri[0][0][j] - x[0];
      z[1] = tri[0][1][j] - x[1];
      z[2] = tri[0][2][j] - x[2];

      tri[0][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[0][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[0][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];

      z[0] = tri[1][0][j] - x[0];
      z[1] = tri[1][1][j] - x[1];
      z[2] = tri[1][2][j] - x[2];

      tri[1][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[1][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[1][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];

      z[0] = tri[2][0][j] - x[0];
      z[1] = tri[2][1][j] - x[1];
      z[2] = tri[2][2][j] - x[2];

      tri[2][0][j] = DL[0]*z[0]+DL[3]*z[1]+DL[6]*z[2] + y[0];
      tri[2][1][j] = DL[1]*z[0]+DL[4]*z[1]+DL[7]*z[2] + y[1];
      tri[2][2][j] = DL[2]*z[0]+DL[5]*z[1]+DL[8]*z[2] + y[2];
    }

    x[0] = y[0];
    x[1] = y[1];
    x[2] = y[2];
  }
}
