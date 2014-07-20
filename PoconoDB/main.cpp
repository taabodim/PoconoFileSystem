//
//  main.cpp
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#include <iostream>
#include "FileSystemAPI.h"
#include "DataRecord.h"
#include "Configs.h"
#include "Utils.h"
#include "CRUDTests.h"
#include <sys/resource.h>
#include "ClientServerExample.h"
#include "DBTest.h"
using namespace PoconoDB;

std::string PoconoDB::Configs::dataDir("/Users/mtaabodi/Documents/pico_data/");
std::string PoconoDB::Configs::logDir("/Users/mtaabodi/Documents/pico_logs/");
std::string PoconoDB::Configs::logFileName("");

void memoryTest() {
    const long num = 10000;
//    string allStrings[num];
//    for(int j=0;j<num;j++)
//    {
//        for(int i=0;i<num;i++) {
//            std::string nameOfCollection("testCollection");
//            allStrings[i] = nameOfCollection;
//            std::cout<<"allocating "<<i<<std::endl;
//    }
//    }

    string* allStrings[num];
    for(int j=0;j<num;j++)
    {
        for(int i=0;i<num;i++) {
            std::string* nameOfCollection = new std::string("testCollection");
            allStrings[i] = nameOfCollection;
            std::cout<<"allocating "<<i<<std::endl;
        }
    }

}
int main(int argc, const char * argv[])
{
    try {

    // This is the PoconoDB first commit
    bool homeSetting = true;
   // setTheStackSize();
   // setTheStackSize();
    if(homeSetting)
    {
        Configs::logDir.clear();
        Configs::logDir.append("/tmp/");
        Configs::dataDir.clear();
        Configs::dataDir.append("/tmp/");

    }
    std::string dataFilename("test");

    dataFilename=PoconoDB::getFullCollectionName(dataFilename);
    openFileIfItDoesntExist(dataFilename);
    truncateTheFile(dataFilename);
        PoconoDB::allOfTests();
    //memoryTest();
        //clientServerExample();
    } catch( const std::exception& e ) { // reference to the base of a polymorphic object
        std::cout << "exception thrown : "<<e.what(); // information from length_error printed
    }
    return 0;
}

