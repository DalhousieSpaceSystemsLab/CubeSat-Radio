
//moving these to a header causes problems. ):
#include "lib/rs"
#include "lib/timeofday"
#include "lib/output"
#include "lib/rs232.h"
//standard stuff
#include <iostream>
#include <fstream>

//sizes for RS container for 1/2 encoding, and 3/4, 9/10
#define totalsize 255
#define halfrate 128
#define quarterrate 191
#define tenthrate 223


//debug macro thing
#ifdef DEBUG_RADIO
#define D(x) x
#else
#define D(x)
#endif
