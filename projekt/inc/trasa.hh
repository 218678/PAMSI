#ifndef _TRASA_HH
#define _TRASA_HH

#include "../inc/libs.hh"

typedef struct trasa {
  string trip_id;
  int stop_id;
  string arrival_time;
  struct tm struct_czas;
} Trasa;

#endif //_TRASA_HH
