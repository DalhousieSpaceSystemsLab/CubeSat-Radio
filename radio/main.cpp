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

    inputdata.read(tempdata, totalsize); //read a chunk into memory
    dsize += totalsize; //save the file read position

    std::string datastring(tempdata); //use string constructor for encoding
    D(std::cout << "File contents read into string: " << datastring << std::endl; )

    return 0;
}
