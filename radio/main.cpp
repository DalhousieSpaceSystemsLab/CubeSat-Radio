//Nick Hansen August 2019
#define DEBUG_RADIO
#include "main.hpp"

//will need to port this to linux.
//https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

//expects parameters such as file name and encode rate
int main(int argc, char *argv[])
{
    std::ifstream inputdata;
    std::ofstream outputdata;
    int rateflag=0;

    std::streampos dpos;
    char* tempdata =  new char[totalsize];


    if(argc > 1){
        //eventually some parsing of arguments will take place here
        D(std::cout << "Args given: ");
        for(int i=0; i<argc; ++i){
            D(std::cout << argv[i] << std::endl);
        }
        if(strcmp(argv[2],"1")==0){
            D(std::cout << "halfrate selected\n";)
            rateflag = 1;
        }
        else if(strcmp(argv[2],"2")==0){
            D(std::cout << "quarterrate selected\n";)
            rateflag =2;
        }
        else if(strcmp(argv[2],"3")==0){
            D(std::cout << "tenthrate selected\n";)
            rateflag = 3;
        }
        else{
            D(std::cout << "ERROR WRONG RATE";)
            return 0;
        }
    }
    else{
        D(std::cout << "Expecting Args, such as file name and rate";)
        return 0;
    }

    //open input and output files
    D(std::cout << "Opening File : " << argv[1] << std::endl;)
    inputdata.open(argv[1], std::ios::in | std::ios::binary);
    outputdata.open("output", std::ios::out);

    //open COMPORT 6 on windows
    int cport_nr=5,        //  (COM6 on windows)
    bdrate=9600;       // 9600 baud
    //8 data bits, no parity, 1 stop bit
    //same settings as arduino... and can be for nL400
    char mode[]= {'8','N','1', 0};
    if(RS232_OpenComport(cport_nr, bdrate, mode, 0))
    {
        D(std::cout << "Can not open comport\n";)
        return(0);
    }

    std::vector<char> datavec;
    switch(rateflag){
    case 1:{
        datavec.reserve(halfrate);
        inputdata.read(tempdata, halfrate);
        for(int i=0; i<halfrate; i++){
            datavec.push_back(tempdata[i]);
        }
        break;
    }
    case 2:{
        datavec.reserve(quarterrate);
        inputdata.read(tempdata, quarterrate);
        for(int i=0; i<quarterrate; i++){
            datavec.push_back(tempdata[i]);
        }
        break;
    }
    case 3:{
        datavec.reserve(tenthrate);
        inputdata.read(tempdata, quarterrate);
        for(int i=0; i<tenthrate; i++){
            datavec.push_back(tempdata[i]);
        }
    }
    case 4:{ //receve a command
        datavec.reserve(totalsize);
    }
    default:{
        D(std::cout << "error";)
        return 0;
    }
    }

    D(std::cout << "File contents read into vector: " ; )

    D(std::cout << datavec << std::endl;)

    ezpwd::RS<totalsize, halfrate> rsh;
    ezpwd::RS<totalsize, quarterrate> rsq;
    ezpwd::RS<totalsize, tenthrate> rst;

    switch(rateflag){
    case 1:{
        rsh.encode(datavec);
        break;
    }
    case 2:{
        rsq.encode(datavec);
        break;
    }

    case 3:{
        rst.encode(datavec);
        break;
    }
    default:{
        D(std::cout << "error";)
        return 0;
    }
    }
    D(std::cout << "encrypted: " << datavec << std::endl;)
    std::vector<char> receiveStr(255);

    transmitRadio(datavec, cport_nr);

    receiveStr = receiveRadio(receiveStr, cport_nr, bdrate, mode);


    D(std::cout << "receved: " << receiveStr << std::endl;)
    int errornum = rst.decode(receiveStr);
    D(std::cout<<"Result: "<<errornum<<std::endl;)
    for(unsigned i=0; i<receiveStr.size(); i++){
        D(std::cout << receiveStr.at(i) << ' ,';)
    }

    RS232_CloseComport(5);
    inputdata.close();
    outputdata.close();
    return 0;
}

/*how to functionalize this file read...?*/
int readFromFile(char* dest, void* in, int rate){
D(std::cout << "\n Args: " << &dest << "\n" << in << "\n" << rate << std::endl;)

return 0;
}

int transmitRadio(std::vector<char> dataString, int cport_nr){

    for(unsigned i=0; i<dataString.size(); i++){
        if(RS232_SendByte(cport_nr, dataString[i])){
            D(std::cout << "error transmitting";)
        }
    }

    return 0;
}

std::vector<char> receiveRadio(std::vector<char> receiveStr, int cport_nr, int bdrate, const char* mode){
    int n=0;
    unsigned char buffer[255];

    n = RS232_PollComport(cport_nr, buffer, 255);
    if(n > 0){
        for(int i=0; i<n; i++){
            receiveStr[i]=buffer[i];
        }
    }
    return receiveStr;
}
