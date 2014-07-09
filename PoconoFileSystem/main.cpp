//
//  main.cpp
//  PoconoFileSystem
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

using namespace PoconoFileSystem;

std::string PoconoFileSystem::Configs::dataDir("/Users/mtaabodi/Documents/pico_data/");
std::string PoconoFileSystem::Configs::logDir("/Users/mtaabodi/Documents/pico_logs/");
std::string PoconoFileSystem::Configs::logFileName("");

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
     
    // This is the PoconoFileSystem first commit
    bool homeSetting = true;
    setTheStackSize();
    if(homeSetting)
    {
        Configs::logDir.clear();
        Configs::logDir.append("/Users/mahmoudtaabodi/Documents/pico_logs/");
        Configs::dataDir.clear();
        Configs::dataDir.append("/Users/mahmoudtaabodi/Documents/pico_data/");
        
    }
    std::string dataFilename("test");
    
    dataFilename=PoconoFileSystem::getFullCollectionName(dataFilename);
        
    truncateTheFile(dataFilename);
    PoconoFileSystem::allOfTests();
    //memoryTest();
    
    } catch( const std::exception& e ) { // reference to the base of a polymorphic object
        std::cout << "exception thrown : "<<e.what(); // information from length_error printed
    }
    return 0;
}

