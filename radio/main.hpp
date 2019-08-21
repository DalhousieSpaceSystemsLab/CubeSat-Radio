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



struct AX25_Frame
{

  // Frame type (I, U or S)
  char  type[1];

  //--- Frame Start ---//

  // Address Subfields
  char  addr_dest[7];
  char  addr_src[7];

  // Control field
  char  ctrl[1];
  char  ctrl_long[2];

  // Protocol Identifier (PID) field, if applicable
  char  pid[1];

  // Information (I) field
  char  i[256];

  // Frame Check Sequence (FCS) field
  char  fcs[2];

  //--- Frame End ---//

}ax25_frame;

/////////////////////////////////
//        AX.25 Methods        //
/////////////////////////////////

//--- Returns frame count required to parse data of known size ---//
unsigned int ax25_framecount
(
  size_t        data_len         // length in bytes of information to be parsed.
);



//-- Parse frames from data ---//
int ax25_parse
(
  char        frame_type[1],  // frame type ('i', 'u' or 's').
  size_t      data_len,       // length in bytes of information referenced by *data.
  void        *data,          // points to information desired to be parsed into frames.
  struct AX25_Frame *frames_out     // points to an array of AX25 frames of at least ax25_framec(data_len) long.
);


#endif //header guard
