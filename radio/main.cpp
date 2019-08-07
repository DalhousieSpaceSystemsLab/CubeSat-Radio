//Nick Hansen August 2019
#define DEBUG_RADIO
#include "main.hpp"

//expects parameters such as file name and encode rate
int main(int argc, char *argv[])
{
    std::ifstream inputdata;
    std::ofstream outputdata;

    std::streampos dsize;
    char* tempdata =  new char[totalsize];

    if(argc > 1){
        //eventually some parsing of arguments will take place here
        D(std::cout << "Args given: ");
        for(int i=0; i<argc; ++i){
            D(std::cout << argv[i] << std::endl);
        }
    }
    else{
        D(std::cout << "Expecting Args, such as file name";)
        return 0;
    }

    //open input and output files
    D(std::cout << "Opening File : " << argv[1] << std::endl;)
    inputdata.open(argv[1], std::ios::in | std::ios::binary);
    outputdata.open("output", std::ios::out);

    inputdata.read(tempdata, halfrate); //read a chunk into memory
    dsize += halfrate; //save the file read position

    std::string datastring(tempdata); //use string constructor for encoding
    D(std::cout << "File contents read into string: " << datastring << std::endl; )

    D(std::cout << "this is the file: " << &inputdata <<std::endl;)
    readFromFile(tempdata, &inputdata, halfrate);

    ezpwd::RS<totalsize, halfrate> rs;
    rs.encode(datastring);

/*
    D(std::cout << "encrypted: " << datastring << std::endl;)
    datastring.replace(1, 1, "~");
    D(std::cout << "corrupted: " << datastring << std::endl;)
    int numfixed = rs.decode(datastring);
    D(std::cout << "fixed: " << numfixed << std::endl;)
*/

    transmitRadio(datastring);

    std::string receiveStr;
    D(std::cout << receiveRadio(receiveStr) << std::endl;)

    return 0;
}

/*how to functionalize this file read...?*/
int readFromFile(char* dest, void* in, int rate){
D(std::cout << "\n Args: " << &dest << "\n" << in << "\n" << rate << std::endl;)

return 0;
}

int transmitRadio(std::string dataString){
    int cport_nr=5,        //  (COM6 on windows)
    bdrate=9600;       // 9600 baud
    //8 data bits, no parity, 1 stop bit
    //same settings as nL400
    char mode[]= {'8','N','1', 0};
    if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
    {
        D(std::cout << "Can not open comport\n";)
        return(0);
    }

    for(unsigned i=0; i<dataString.size(); i++){
        RS232_SendByte(cport_nr, dataString[i]);
    }

    RS232_CloseComport(cport_nr);

    return 0;
}

std::string receiveRadio(std::string receiveStr){
    int cport_nr=5,        //  (COM6 on windows)
    bdrate=9600;       // 9600 baud
    char mode[]= {'8','N','1', 0};
    int n=0;

    if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
    {
        D(std::cout << "Can not open comport\n";)
        return(0);
    }

    while(n==0){
    n = RS232_PollComport(cport_nr, (unsigned char*)receiveStr.c_str(), totalsize);
    }

    D(std::cout << "Bytes Received: " << n << std::endl;)

    return receiveStr;
}
