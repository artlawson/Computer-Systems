/*
 * Trivial implementation of converter function.
 * You may override this file or create your own.
 */
#include "converter.h"
#include <stdlib.h>
#include "string.h"

//4294967295

static int getDecDigit(const char str)
{
    int digit = 0;
    switch (str)
    {
        case '0':
        digit = 0;
        break;

        case '1':
        digit = 1;
        break;

        case '2':
        digit = 2;
        break;

        case '3':
        digit = 3;
        break;

        case '4':
        digit = 4;
        break;

        case '5':
        digit = 5;
        break;

        case '6':
        digit = 6;
        break;

        case '7':
        digit = 7;
        break;

        case '8':
        digit = 8;
        break;

        case '9':
        digit = 9;
        break;
    }
    return digit;
}

  int  apple(const char* str)
{

  const int declength =strlen(str);
   int Number =0;
   switch(declength)
    {
          // Convert characters to digits with another function.
          case 3:
          Number+=getDecDigit(str[0])*100;
          Number+=getDecDigit(str[1])*10;
          Number+=getDecDigit(str[2])*1;
          break;

          case 4:
          Number+=getDecDigit(str[0])*1000;
          Number+=getDecDigit(str[1])*100;
          Number+=getDecDigit(str[2])*10;
          Number+=getDecDigit(str[3])*1;
          break;

          case 5:
          Number+=getDecDigit(str[0])*10000;
          Number+=getDecDigit(str[1])*1000;
          Number+=getDecDigit(str[2])*100;
          Number+=getDecDigit(str[3])*10;
          Number+=getDecDigit(str[4])*1;
          break;
  }

  return Number;
}



void convert_all(unsigned nlines, char *lines[], quote_t nums[])
{
	for (unsigned i = 0; i < nlines; i++) {
    	nums[i] = apple(lines[i]); //i++; <<<<used for unrolling, but just one call ended up working optimally
      //nums[i] = apple(lines[i]); 

  }
}
