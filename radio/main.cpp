//Nick Hansen August 2019
#define DEBUG_RADIO
#include "main.hpp"

//whole thing will need to port this whole thing to linux.
//https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
//the main thing is how a virtual com port / serial device appears in linux (everything is a file)

//what even is best practice anyways :)
class RadioHelper{
public:
    //data
    std::ifstream inputFile;
    std::ofstream outputFile;
    std::streampos dpos;
    int rateFlag = 0;
    char tempData[TOTALSIZE];
    int cport_nr=5,        //  (COM6 on windows)
    bdrate=9600;       // 9600 baud
    //8 data bits, no parity, 1 stop bit
    char mode[4]= {'8','N','1', 0}; //Must match the serial config of the nL400
    std::vector<char> dataVec;
    //could save some memory (>1kb) by only making one of these at runtime
    ezpwd::RS<TOTALSIZE, HALFRATE> rsh;
    ezpwd::RS<TOTALSIZE, QUARTERRATE> rsq;
    ezpwd::RS<TOTALSIZE, TENTHRATE> rst;

//methods
int DUMMY_OPEN_COMPORT(){
    //TODO: HOWEVER SERIAL COMS WORKS IN LINUX ON OUR HARDWARE
    return 1;
}

int DUMMY_CLOSE_COMPORT(){
    return 1;
}

int DUMMY_SERIAL_SEND(){
    return 1;
}

int DUMMY_SERIAL_RECEIVE(){
    return 1;
}

//there is definitely a better way to achieve this
int readFromFile(){
    switch(rateFlag){
    case 1:{
        dataVec.reserve(HALFRATE);
        inputFile.read(tempData, HALFRATE);

        for(int i=0; i<HALFRATE; i++){
            dataVec.push_back(tempData[i]);
        }
        break;
    }
    case 2:{
        dataVec.reserve(QUARTERRATE);
        inputFile.read(tempData, QUARTERRATE);
        for(int i=0; i<QUARTERRATE; i++){
            dataVec.push_back(tempData[i]);
        }
        break;
    }
    case 3:{
        dataVec.reserve(TENTHRATE);
        inputFile.read(tempData, QUARTERRATE);
        for(int i=0; i<TENTHRATE; i++){
            dataVec.push_back(tempData[i]);
        }
    }
    case 4:{
        dataVec.reserve(TOTALSIZE);
        inputFile.read(tempData, TOTALSIZE);
        for(int i=0; i<TENTHRATE; i++){
            dataVec.push_back(tempData[i]);
        }
    }
    default:{
        D(std::cout << "error";)
        return 0;
    }
    }//switch rateFlag
    return 1;
}//setRate method

//once again working around using templates at runtime
int encode(){
    switch(rateFlag){
    case 1:{
        rsh.encode(dataVec);
        break;
    }
    case 2:{
        rsq.encode(dataVec);
        break;
    }
    case 3:{
        rst.encode(dataVec);
        break;
    }
    case 4:{
        D(std::cout << "receve only"<<std::endl;)
        break;
    }
    default:{
        D(std::cout << "error";)
        return 0;
    }
    }//end switch
    return 1;
}//encode method

int transmitRadio(){

    for(unsigned i=0; i<dataVec.size(); i++){
        if(DUMMY_SERIAL_SEND()){
            D(std::cout << "\nData Transmitted!\n";)
            return 1;
        }
    }
    return 0;
}

int receiveRadio(){
    if(DUMMY_SERIAL_RECEIVE()){
        return 1;
    }
    return 0;
}

};//RadioHelper Object



//expects parameters such as file name and encode rate
//USAGE:
//arg 0: String indicating path of executable (default in windows)
//arg 1: "1" = transmit or "0" = receive mode
//arg 2: path/name of input file containing data to transmit/receive
//arg 3: the FEC parity rate at which to transmit "1"=half "2"=quarter "3"=tenth
//example: 1 test 1 will transmit the contents of file "test" at FEC rate 1/2
int main(int argc, char *argv[])
{
    RadioHelper radioHelper;

    if(argc > 1){
        D(std::cout << "Args given: ");
        for(int i=0; i<argc; ++i){
            D(std::cout << argv[i] << std::endl;)
        }
        if(strcmp(argv[1],"0")==0){
            D(std::cout << "Receive mode selected" << std::endl;)
            radioHelper.rateFlag = 4; //Receiving works differently
        }
        else if(strcmp(argv[1],"1")==0){
            D(std::cout << "Transmit mode selected" << std::endl;)
if(strcmp(argv[3],"1")==0){
    D(std::cout << "halfrate selected\n";)
    radioHelper.rateFlag = 1;
}
else if(strcmp(argv[3],"2")==0){
    D(std::cout << "quarterrate selected\n";)
    radioHelper.rateFlag =2;
}
else if(strcmp(argv[3],"3")==0){
    D(std::cout << "tenthrate selected\n";)
    radioHelper.rateFlag = 3;
}
else{
    D(std::cout << "ERROR WRONG RATE";)
    return 0;
}//rate if
        }//transmit mode if
        else{
            D(std::cout << "ERROR WRONG MODE";)
            return 0;
        }
    }//if args
    else{
        D(std::cout << "Expecting Args, USAGE: ...todo";)
        return 0;
    }

    //open input and output files
    D(std::cout << "Opening File : " << argv[2] << std::endl;)
    radioHelper.inputFile.open(argv[2], std::ios::in | std::ios::binary);
    radioHelper.outputFile.open("output", std::ios::out | std::ios::ate);

    //open serial communications
    if(radioHelper.DUMMY_OPEN_COMPORT()==0)
    {
        D(std::cout << "Can not open comport\n";)
        return(0);
    }

    radioHelper.readFromFile();
    D(std::cout << "File contents read into vector: " <<std::endl; )
    D(std::cout << radioHelper.dataVec << std::endl;)

    radioHelper.encode();
    D(std::cout << "Encrypted: " << radioHelper.dataVec << std::endl;)

    radioHelper.transmitRadio();

    radioHelper.receiveRadio();
    D(std::cout << "Receved with corruptions: " << " " << std::endl;)
    radioHelper.dataVec.at(0)='x';
    radioHelper.dataVec.at(1)='x';
    radioHelper.dataVec.at(2)='x';
    radioHelper.dataVec.at(5)='x';
    radioHelper.dataVec.at(9)='x';
    radioHelper.dataVec.at(11)='x';
    radioHelper.dataVec.at(23)='x';
    D(std::cout << radioHelper.dataVec << std::endl;)

    int errorNum = radioHelper.rsh.decode(radioHelper.dataVec);
    D(std::cout<<"Errors Corrected: "<< errorNum <<std::endl;)
    D(std::cout << radioHelper.dataVec << std::endl;)

    //perhaps a destructor
    radioHelper.DUMMY_CLOSE_COMPORT();
    radioHelper.inputFile.close();
    radioHelper.outputFile.close();
    return 0;
}




