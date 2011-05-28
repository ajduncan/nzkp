#include <stdio.h>
#include <time.h>
// http://www.hinz.fdns.net/timestamp-in-c-and-python.html

void timestamp(FILE *out)
{
  struct tm *tmp;
  time_t t;

  t = time(NULL);
  tmp = localtime(&t); /* or gmtime, if you want GMT^H^H^HUTC */
  fprintf(out, "%02d.%02d.%04d %02d:%02d:%02d",
     tmp->tm_mday, tmp->tm_mon+1, tmp->tm_year+1900,
     tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
}

