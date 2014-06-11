//
//  FileSystem.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_FileSystem_h
#define PoconoFileSystem_FileSystem_h
#include "FileWriter.h"
#include "FileReader.h"
#include "CollectionIndex.h"
#include <list>
#include "SuperBlock.h"
#include "Utils.h"
#include "DataRecordMetaData.h"
namespace PoconoFileSystem {
    class FileSystem {
        private :
        std::string filename;
        FileReaderPtr fileReader;
        FileWriterPtr fileWriter;
        std::list<CollectionMetaDataPtr> allCollectionsMap; //change this to map later

        
        SuperBlock superBlock;
        public :
        
        
        FileSystem(std::string fileName):filename(fileName),fileReader(new FileReader(filename)),
        fileWriter(new FileWriter(filename))
        {
            filename = PoconoFileSystem::getFullCollectionName(filename);
        //read all the collectionOffsets from the file , and load the collectionMap
            loadAllCollectionMap();
            
        }
        CollectionMetaDataPtr openCollection(std::string nameOfCollection)
        {
            
            CollectionMetaDataPtr collMetaData = getCollectionMetaData(nameOfCollection);
            if(collMetaData)
            {
                return collMetaData;
            }
            else {
               
                CollectionMetaDataPtr colIndex(new CollectionMetaData(nameOfCollection));
                allCollectionsMap.push_back(colIndex);
                colIndex->offsetOfCollectionMetaDataInFile =STARTING_OFFSET_OF_COLLECITON_INDEXS + ((allCollectionsMap.size()-1) * sizeof(class CollectionMetaData));
                fileWriter->writeCollectionMetaData(colIndex);
                collMetaData = colIndex;
            }
            return collMetaData;
        }
        void loadAllCollectionMap()
        {
            offsetType offsetToGetMetaDataFrom = STARTING_OFFSET_OF_COLLECITON_INDEXS;
            while(offsetToGetMetaDataFrom<ENDING_OFFSET_OF_COLLECITON_INDEXS)
            {
            
                CollectionMetaDataPtr collMetaData = fileReader->readCollectionMetaDataFromFile(offsetToGetMetaDataFrom);
                if(collMetaData->offsetOfCollectionMetaDataInFile!=-1)
                {
                    if(!collMetaData->getNameOfCollectionAsString().empty())
                    {
                    allCollectionsMap.push_back(collMetaData);
                    }
                }
                offsetToGetMetaDataFrom +=(sizeof(class  CollectionMetaData));
            }
            
            std::cout<<"allCollectionsMap size is "<<allCollectionsMap.size()<<std::endl;
            
            
        }
        std::string createCollection(std::string nameOfCollection){
            //1. Check if collection doesnt exits,if it exists , return false
            //2.if not,create a record in collectionMap , and append the record in file later change these to vars ,
            std::string res;
            if(!collectionExists(nameOfCollection))
            {
                size_t offset = PoconoFileSystem::getEndOfFileOffset(filename);
                CollectionMetaDataPtr colIndex(new CollectionMetaData(nameOfCollection));
                
                size_t offsetOfCollectionIndex =STARTING_OFFSET_OF_COLLECITON_INDEXS+ allCollectionsMap.size()*sizeof(class CollectionMetaData);
                std::cout<< " offsetOfCollectionIndex : "<<offsetOfCollectionIndex<<std::endl;
                
                fileWriter->writeCollectionMetaData(colIndex,offsetOfCollectionIndex);
                allCollectionsMap.push_back(colIndex);
                res.append("collection was added");
                return res;
            }
            res.append("collection already exists");
            return res;
        }
        bool collectionExists(std::string nameOfCollection) {
            return false;//for now
        }
        void insertDataRecordMetaData( CollectionMetaDataPtr collectionMetaData,DataRecordMetaDataPtr dataRecordMetaDataPtr)
        {

//            dataRecordMetaDataPtr->offsetOfDataRecordMetaData =  offset; //it should be always right after
            //the DataRecordMetaData
            //DataRecordPtr lastRecordOfCollection = getLastRecordOfColelction(collectionMetaData);

            //get the last data record meta data that collection meta data points to
            DataRecordMetaDataPtr lastdataRecordMetaDataOfCollection = getLastDataRecordMetaDataOfCollection(collectionMetaData);
           
            if(lastdataRecordMetaDataOfCollection)
                
            {

                lastdataRecordMetaDataOfCollection->offsetOfNextDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData; //make the last
                //dataRecordMetaData of collection points to the new data record 
                
                updateThisDataRecordMetaData(lastdataRecordMetaDataOfCollection);//updates the last data record meta data 
                //that points to the data record that we are about to save right after it.


                dataRecordMetaDataPtr->offsetOfPreviousDataRecordMetaData = lastdataRecordMetaDataOfCollection->offsetOfDataRecordMetaData;
                collectionMetaData->offsetOfLastDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                fileWriter->writeCollectionMetaData(collectionMetaData);
            }
            else {
            //this is the case that we want to write the first data row in colleciton, the collection
                //doesnt point to anything 
                dataRecordMetaDataPtr->offsetOfPreviousDataRecordMetaData = -1;
                collectionMetaData->offsetOfFirstDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                collectionMetaData->offsetOfLastDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                
                fileWriter->writeCollectionMetaData(collectionMetaData);
            }
            dataRecordMetaDataPtr->offsetOfNextDataRecordMetaData = -1;
            dataRecordMetaDataPtr->offsetOfCollection = collectionMetaData->offsetOfCollectionMetaDataInFile;
            appendThisRecordMetaData(dataRecordMetaDataPtr);
            fileWriter->writeCollectionMetaData(collectionMetaData);
            fileWriter->writeDataRecordMetaData(dataRecordMetaDataPtr);


        }
        void insertDataRecord(CollectionMetaDataPtr collectionMetaData,DataRecordPtr record)
        {

                        
            DataRecordMetaDataPtr lastdataRecordMetaDataOfCollection = getLastDataRecordMetaDataOfCollection(collectionMetaData);
                       if(lastdataRecordMetaDataOfCollection)
                
            {
                lastdataRecordMetaDataOfCollection->offsetOfDataRecord = record->offsetOfDataRecord;//make the last record
                //meta data point to the next data record
                updateThisDataRecordMetaData(lastdataRecordMetaDataOfCollection);//update the last record meta data
            }
            else {
            
            }
            appendThisRecord(record);
            fileWriter->writeCollectionMetaData(collectionMetaData);
            

        }
        void appendThisRecordMetaData(DataRecordMetaDataPtr dataRecordMetaDataPtr)
        {
            fileWriter->writeDataRecordMetaData(dataRecordMetaDataPtr);

        }
        void insertData( CollectionMetaDataPtr collectionMetaData,DataRecordPtr record)
        {
            //1.read the end of DataBlock  offset
            //2.add the record to file to the an offset which is multiple of BLOCK_SIZE =1024
            //3.add the new offset to the next of the collection.offsetOfLastDataRecord
            //4.getTheOffsetOfLastDataRecord from collection and, add its offset to the
            //previous field of new DataRecord
            
            //4.add the offset as the ofssetOfLastDataRecordInCollection
            //5.add the offset of collectionMetaData to the offsetOfCollection in DataRecord
            
            size_t offset = PoconoFileSystem::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024
            
            DataRecordMetaDataPtr dataRecordMetaData(new DataRecordMeataData());
            dataRecordMetaData->sizeOfValueFieldInDataRecord  = record->sizeOfValueFieldInDataRecord;
            dataRecordMetaData->offsetOfDataRecordMetaData = offset;
            insertDataRecordMetaData(collectionMetaData,dataRecordMetaData);//insert the Meta Data about the record before inserting the record
            //each collectionMetaData should have information about dataRecordMetaData
            //each dataRecordMetaData should have information about dataRecord
            
            
            size_t sizeOfClass = sizeof(class DataRecordMeataData);
            record->offsetOfDataRecord =sizeOfClass + offset; //record sits right after the meta data record

            insertDataRecord(collectionMetaData,record);//inser the real data in the data record after the inserting the meta data
            
            
        }
        std::shared_ptr<std::list<DataRecordPtr>> getAllData(CollectionMetaDataPtr collectionArg)
        {
            std::shared_ptr<std::list<DataRecordPtr>> allData(new std::list<DataRecordPtr>());
            //1.get the most updated version of collectionMetaData
           CollectionMetaDataPtr collection =  fileReader->readCollectionMetaDataFromFile(collectionArg->offsetOfCollectionMetaDataInFile);
            
            //2. get the first data record and append to the list
            DataRecordMetaDataPtr firstDataMetaDataPtr = fileReader->readDataRecordMetaDataFromFile(collection->offsetOfFirstDataRecordMetaData);
            
            DataRecordPtr firstDataPtr = fileReader->readDataRecordFromFile(firstDataMetaDataPtr->offsetOfDataRecord);
            std::string valueOfDataRecord = fileReader->readTheValueOfDataRecord(firstDataPtr->offsetOfValueOfRecordInFile,firstDataPtr->sizeOfValueFieldInDataRecord);
            assert(!valueOfDataRecord.empty());
            


            allData->push_back(firstDataPtr);
            DataRecordMetaDataPtr nextDataRecordMetaDataPtr = firstDataMetaDataPtr;
            while(nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData!=-1)
            {
                DataRecordPtr dataPtr = fileReader->readDataRecordFromFile(nextDataRecordMetaDataPtr->offsetOfDataRecord);
                
                std::cout<<" pushing back data dataPtr->offsetOfNextDataRecordMetaData : "<<nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData<<std::endl;
                allData->push_back(dataPtr);
                
                nextDataRecordMetaDataPtr =fileReader->readDataRecordMetaDataFromFile(nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData);
                
                
            }
            
            return  allData;
            
            
        }
        void appendThisRecord(DataRecordPtr record)
        {
            offsetType offsetOfValueOfRecordInFile = fileWriter->writeTheValueOfRecord(record);
            record->offsetOfValueOfRecordInFile = offsetOfValueOfRecordInFile;
            record->offsetOfDataRecord = offsetOfValueOfRecordInFile;
            fileWriter->writeDataRecordAtOffset(record);
        }

        void updateThisRecord(DataRecordPtr record)
        {
            fileWriter->writeDataRecordAtOffset(record);
            
        }
        void updateThisDataRecordMetaData(DataRecordMetaDataPtr record)
        {
            
            fileWriter->writeDataRecordMetaData(record);
        }

        
        
        DataRecordPtr getFirstRecordOfCollection(CollectionMetaDataPtr collection)
        {
            DataRecordPtr nullPtr;
            if(collection->offsetOfFirstDataRecordMetaData==-1) return nullPtr;
            else
           return fileReader->readDataRecordFromFile(collection->offsetOfFirstDataRecordMetaData);
        
        }
        DataRecordPtr getLastRecordOfCollection(CollectionMetaDataPtr collection)
        {
            DataRecordPtr nullPtr;
            if(collection->offsetOfLastDataRecordMetaData==-1) return nullPtr;
            else
                return fileReader->readDataRecordFromFile(collection->offsetOfLastDataRecordMetaData);
            
        }
        
        DataRecordMetaDataPtr getLastDataRecordMetaDataOfCollection(CollectionMetaDataPtr collection)
        {
            DataRecordMetaDataPtr nullPtr;
            if(collection->offsetOfLastDataRecordMetaData==-1) return nullPtr;
            else
                return fileReader->readDataRecordMetaDataFromFile(collection->offsetOfLastDataRecordMetaData);
        }
        CollectionMetaDataPtr getCollectionMetaData (std::string nameOfCollection)
        {
         
            CollectionMetaDataPtr nullPtr;
            for(std::list<CollectionMetaDataPtr>::iterator it = allCollectionsMap.begin();
				it != allCollectionsMap.end(); ++it)
            {
                CollectionMetaDataPtr colPtr = *it;
                std::string str = colPtr->getNameOfCollectionAsString();
                
                if(str.compare(nameOfCollection)
                   ==0)
                {
                    return colPtr;
                }
            }
            std::cout<<" didn't find the collection, retuning nullptr";
            return nullPtr;//
        }
        
        void deleteCollection(std::string nameOfCollection){
        }
        
        
        
        
        
        
        
        
        
        
        void deleteData(std::string nameOfCollection,DataRecordPtr record)
        {
            
        }
        
        void updateData(std::string nameOfCollection,DataRecordPtr record)
        {
            
        }

        
        };
}

#endif
