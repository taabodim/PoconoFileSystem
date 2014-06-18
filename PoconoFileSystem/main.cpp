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

#include <sys/resource.h>

using namespace PoconoFileSystem;

std::string PoconoFileSystem::Configs::dataDir("/Users/mtaabodi/Documents/pico_data/");
std::string PoconoFileSystem::Configs::logDir("/Users/mtaabodi/Documents/pico_logs/");
std::string PoconoFileSystem::Configs::logFileName("");


void testWritingAndReadingSmallValuesInOneCollection()
{
    
    
    FileSystemAPI testFS;
    CollectionMetaDataPtr collection =testFS.openCollection("testCollection");
    
    int num = 10;
    for(int i=0;i<num;i++)
    {
        DataRecordPtr record(new DataRecord("smallKey","smallValue"));
        testFS.insertData(collection, record);
    }
    
    std::list<DataRecordPtr> allData;
     testFS.getAllData(allData,collection);
    for(std::list<DataRecordPtr>::iterator it = allData.begin();
        it!=allData.end();++it)
    {
        std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
    }
}
void testWritingAndReadingOneSmallValueInOneCollection() {
    FileSystemAPI testFS;
    std::string nameOfCollection("testCollection");
    CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);
    
    
    for(int i=0;i<1;i++)
    {
        DataRecordPtr record(new DataRecord("smallKey","smallValue***************************************************************************************EhdOfValue"));
        testFS.insertData(collection, record);
    }
    
    std::list<DataRecordPtr> allData;
    testFS.getAllData(allData,collection);
    for(std::list<DataRecordPtr>::iterator it = allData.begin();
        it!=allData.end();++it)
    {
        std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
    }

}
void testWritingAndReadingSmallValuesInThreeCollections()
{
    
    
    FileSystemAPI testFS;
    int numOfCollectionsInTest =3;
    
//    for(int col = 0 ; col<numOfCollectionsInTest;col++)
//    {
    std::string colName1 ("testCollection1");
    std::string colName2 ("testCollection2");
    std::string colName3 ("testCollection3");


    CollectionMetaDataPtr collection1 =testFS.openCollection(colName1);
    CollectionMetaDataPtr collection2 =testFS.openCollection(colName2);
    CollectionMetaDataPtr collection3 =testFS.openCollection(colName3);
    CollectionMetaDataPtr allCollections [3]={collection1,collection2,collection3};
    
    int num = 10;
    for(int j=0;j<3;j++)
    {
    for(int i=0;i<num;i++)
    {
        std::string key("smallKey");
        key.append(toStr(i));
        key.append(allCollections[j]->getNameOfCollectionAsString());
        
        std::string value("smallValue***************************************************************************************EhdOfValue");
        value.append(toStr(i));
        value.append(allCollections[j]->getNameOfCollectionAsString());
        
        DataRecordPtr record(new DataRecord(key,value));
        testFS.insertData(allCollections[j], record);
    }
    }
    
    for(int j=0;j<3;j++)
    {
        std::cout<<"data in the next collections *************"<<std::endl;
        
        std::list<DataRecordPtr> allData;
        testFS.getAllData(allData,allCollections[j]);
    for(std::list<DataRecordPtr>::iterator it = allData.begin();
        it!=allData.end();++it)
    {
        std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
    }
    
    }
    
//    }
}

void testWritingTenDataRecordAndFindingOne() {
    FileSystemAPI testFS;
    std::string nameOfCollection("testCollection");
    CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);
    
    
    for(int i=0;i<10;i++)
    {
       std::string key("smallKey");
        key.append(toStr(i));

        DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
        testFS.insertData(collection, record);
    }
    //find the one with key : smallKey8
    std::string key("smallKey8");
    DataRecordPtr foundRecord = testFS.find(nameOfCollection,key);
    assert(foundRecord!=NULL);
    assert(foundRecord->getKeyAsString().compare(key)==0);
    std::cout<<"foundRecord Data Record is "<<(foundRecord)->toString()<<std::endl;
    

}
void testDeletingFourCollections()
{
    FileSystemAPI testFS;
    std::string nameOfCollection("testCollection");
    std::string nameOfCollection1("testCollection1");
    std::string nameOfCollection2("testCollection2");
    std::string nameOfCollection3("testCollection3");
    
    testFS.openCollection(nameOfCollection);
    testFS.openCollection(nameOfCollection1);
    testFS.openCollection(nameOfCollection2);
    testFS.openCollection(nameOfCollection3);

    testFS.deleteCollection(nameOfCollection);
    testFS.deleteCollection(nameOfCollection1);
    testFS.deleteCollection(nameOfCollection2);
    testFS.deleteCollection(nameOfCollection3);

    list<std::string> allCollections= testFS.showAllCollections();
    for(list<std::string>::iterator iter=allCollections.begin();
        iter!=allCollections.end();++iter)
    {
        std::cout<<"collection name is : "<<*iter<<std::endl;
    }
    
    assert(allCollections.size()==0);
    
}

//incomplete , finish this test
void testWriting10DataRecordAndDeletingTheOneWithSmallKey8() {
    FileSystemAPI testFS;
    std::string nameOfCollection("testCollection");
    
    testFS.deleteCollection(nameOfCollection);
    
    CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);
    
    
    for(int i=0;i<100;i++)
    {
        std::string key("smallKey");
        key.append(toStr(i));
        
        DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
        testFS.insertData(collection, record);
    }
    //find the one with key : smallKey8
    std::string key("smallKey8");
    std::string response = testFS.deleteData(nameOfCollection,key);
    string respExpected("record was deleted with this key : ");
                respExpected.append(key);
     assert(respExpected.compare(response)==0);
    // assert(foundRecord->getKeyAsString().compare(key)==0);
     std::cout<<"response for deleting the Data Record is "<<response<<std::endl;
    
    
}
void allOfTests() {
    //testWritingAndReadingOneSmallValueInOneCollection();//stage 1
    //testWritingTenDataRecordAndFindingOne();
    //testWritingAndReadingSmallValuesInOneCollection();
    //testWritingAndReadingSmallValuesInThreeCollections();
    //testWriting10DataRecordAndDeletingTheOneWithSmallKey8();
    
}
int main(int argc, const char * argv[])
{
    // This is the PoconoFileSystem first commit
    bool homeSetting = false;
    //setTheStackSize();
    if(homeSetting)
    {
        Configs::logDir.clear();
        Configs::logDir.append("/Users/mahmoudtaabodi/Documents/pico_logs/");
        Configs::dataDir.clear();
        Configs::dataDir.append("/Users/mahmoudtaabodi/Documents/pico_data/");
        
    }
    std::string dataFilename(Configs::dataDir);
    dataFilename.append("test.dat");

    truncateTheFile(dataFilename);
    allOfTests();
  
    testDeletingFourCollections();
    
    return 0;
}

