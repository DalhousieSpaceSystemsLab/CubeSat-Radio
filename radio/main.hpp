#ifndef RADIO_H
#define RADIO_H

#include "lib/rs"
#include "lib/timeofday"
#include "lib/output"
//standard stuff
#include <iostream>
#include <fstream>
#include <string>

//sizes for RS container for 1/2 encoding, and 3/4, 9/10
#define TOTALSIZE 255
#define HALFRATE 128
#define QUARTERRATE 191
#define TENTHRATE 223


//debug macro thing
#ifdef DEBUG_RADIO
#define D(x) x
#else
#define D(x)
#endif //debug macro



struct LORIS_Frame
{

  

}loris_frame;//Frame End



#endif //header guard
