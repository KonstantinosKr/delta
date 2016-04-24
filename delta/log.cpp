/*
 The MIT License (MIT)

 Copyright (c) 2016 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "log.h"

void logg::performance(iREAL minsubtotal, iREAL maxsubtotal, iREAL avgsubtotal,
                      iREAL minbal, iREAL maxbal, iREAL avgbal,
                      iREAL minmig, iREAL maxmig, iREAL avgmig,
                      iREAL minde, iREAL maxde, iREAL avgde,
                      iREAL mindt1, iREAL maxdt1, iREAL avgdt1,
                      iREAL mindt2, iREAL maxdt2, iREAL avgdt2,
                      iREAL mindt3, iREAL maxdt3, iREAL avgdt3)
{
  char filename[100] = "output/data/performance.csv"; //care or buffer overflow
  printf("Performance Data: %s\n", filename);

  FILE *fp = fopen(filename, "w+");
  if( fp == NULL )
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fprintf(fp,"TotalRunMin, TotalRunMax, TotalRunAvg,"
             "BalanceMin, BalanceMax, BalanceAvg,"
             "MigrationMin, MigrationMax, MigrationAvg,"
             "DataXchangeMax, DataXchangeMin, DataXchangeAvg,"
             "DT1Min, DT1Max, DT1Avg,"
             "DT2Min, DT2Max, DT2Avg,"
             "DT2Min, DT2Max, DT2Avg,"
             "DT3Min, DT3Max, DT3Avg\n");

  fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
          minsubtotal, maxsubtotal, avgsubtotal,
          minbal, maxbal, avgbal,
          minmig, maxmig, avgmig,
          minde, maxde, avgde,
          mindt1, maxdt1, avgdt1,
          mindt2, maxdt2, avgdt2,
          mindt3, maxdt3, avgdt3);

  fclose(fp);
}

void logg::input(char *filename, int nt)
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
  printf("DELTA MASTER h:%i m:%i s%i \t| INPUT \t| %s::%i::\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, filename, nt);
}

void logg::initiate()
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
	printf("DELTA MASTER h:%i m:%i s%i \t| INITIATING EXPERIMENT\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void logg::slaveInitiate(int myrank, int nb)
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
	printf("DELTA SLAVE h:%i m:%i s%i \t| RANK:%i NB:%i\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, myrank, nb);
}

void logg::start(int nt, int nb)
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);

	printf("DELTA MASTER h:%i m:%i s%i \t| NUMBERS \t| NT:%i NB:%i\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, nt, nb);
	printf("----------------------------------------------------------------------------\n");
}

void logg::iteration(int timestep)
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
	printf("DELTA MASTER h:%i m:%i s%i \t| ITERATION \t| %i\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, timestep);
}

void logg::output(char *filename)
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
	printf("DELTA MASTER h:%i m:%i s%i \t| OUTPUT \t| %s\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec, filename);
}

void logg::end()
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
	printf("----------------------------------------------------------------------------\n");
	printf("DELTA MASTER h:%i m:%i s%i \t| COMPUTATION FINISHED\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void logg::postprocessing()
{
  time_t now = time(0);
  tm *ltm = std::localtime(&now);
  printf("DELTA MASTER h:%i m:%i s%i \t| POST-PROCESSING COMPLETE.\n", ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
}

void logg::contact()
{

}

void logg::force()
{

}

void logg::dynamic()
{

}
