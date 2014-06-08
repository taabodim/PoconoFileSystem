//
//  main.cpp
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#include <iostream>
#include "FileSystemTest.h"
#include "DataRecord.h"
#include "Configs.h"

std::string PoconoFileSystem::Configs::dataDir("/Users/mtaabodi/Documents/pico_data/");
std::string PoconoFileSystem::Configs::logDir("/Users/mtaabodi/Documents/pico_logs/");
std::string PoconoFileSystem::Configs::logFileName("");

int main(int argc, const char * argv[])
{
    using namespace PoconoFileSystem;
    // This is the PoconoFileSystem first commit
    bool homeSetting = true;
    
    if(homeSetting)
    {
        Configs::logDir.clear();
        Configs::logDir.append("/Users/mahmoudtaabodi/Documents/pico_logs/");
        Configs::dataDir.clear();
        Configs::dataDir.append("/Users/mahmoudtaabodi/Documents/pico_data/");
        
        
    }
    FileSystemTest testFS;
    CollectionMetaDataPtr collection =testFS.openCollection("testCollection");
    int num = 10;
    for(int i=0;i<num;i++)
    {
    DataRecordPtr record(new DataRecord("smallKey","smallValue"));
    testFS.insertData(collection, record);
    }
    
    std::shared_ptr<std::list<DataRecordPtr>> allData = testFS.getAllData(collection);
        for(std::list<DataRecordPtr>::iterator it = allData->begin();
            it!=allData->end();++it)
        {
            std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
        }
    
    return 0;
}

