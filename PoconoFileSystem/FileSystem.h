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
        void insertData( CollectionMetaDataPtr collectionMetaData,DataRecordPtr record)
        {
            //1.read the end of file offset
            //2.add the record to file to the an offset which is multiple of BLOCK_SIZE =1024
            //3.add the new offset to the next of the collection.offsetOfLastDataRecord
            //4.getTheOffsetOfLastDataRecord from collection and, add its offset to the
            //previous field of new DataRecord
            
            //4.add the offset as the ofssetOfLastDataRecordInCollection
            //
            
            size_t offset = PoconoFileSystem::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024
            
            //fix this because it should be starting from DATA BLOCK not at the end of file
            record->offsetOfDataRecord = offset;
            
            DataRecordPtr lastRecordOfCollection = getLastRecordOfColelction(collectionMetaData);
            
            if(lastRecordOfCollection)
                
            {
                lastRecordOfCollection->offsetOfNextDataRecord = record->offsetOfDataRecord;
                updateThisRecord(lastRecordOfCollection);
                record->offsetOfPreviousDataRecord = lastRecordOfCollection->offsetOfDataRecord;
                collectionMetaData->offsetOfLastDataRecord = record->offsetOfDataRecord;
                fileWriter->writeCollectionMetaData(collectionMetaData);
            }
            else {
            
                record->offsetOfPreviousDataRecord = -1;
                collectionMetaData->offsetOfFirstDataRecord = record->offsetOfDataRecord;
                collectionMetaData->offsetOfLastDataRecord = record->offsetOfDataRecord;
                
                fileWriter->writeCollectionMetaData(collectionMetaData);
            }
                record->offsetOfNextDataRecord = -1;
            //collectionMetaData->offsetOfLastDataRecord = offset;
            appendThisRecord(record, offset);
            fileWriter->writeCollectionMetaData(collectionMetaData);
            
            
        }
        std::shared_ptr<std::list<DataRecordPtr>> getAllData(CollectionMetaDataPtr collectionArg)
        {
            std::shared_ptr<std::list<DataRecordPtr>> allData(new std::list<DataRecordPtr>());
            //1.get the most updated version of collectionMetaData
           CollectionMetaDataPtr collection =  fileReader->readCollectionMetaDataFromFile(collectionArg->offsetOfCollectionMetaDataInFile);
            
            //2. get the first data record and append to the list
            DataRecordPtr firstDataPtr = fileReader->readDataRecordFromFile(collection->offsetOfFirstDataRecord);
            allData->push_back(firstDataPtr);
            DataRecordPtr nextDataPtr = firstDataPtr;
            while(nextDataPtr->offsetOfNextDataRecord!=-1)
            {
                DataRecordPtr dataPtr = fileReader->readDataRecordFromFile(nextDataPtr->offsetOfNextDataRecord);
                std::cout<<" pushing back data dataPtr->offsetOfNextDataRecord : "<<dataPtr->offsetOfNextDataRecord<<std::endl;
                allData->push_back(dataPtr);
                nextDataPtr = dataPtr;
                
            }
            
            return  allData;
            
            
        }
        void appendThisRecord(DataRecordPtr record,size_t offset)
        {
            
            assert(offset>=0);
            fileWriter->writeDataRecordAtOffset(record, offset);
            
        }
        void updateThisRecord(DataRecordPtr record)
        {
            fileWriter->writeDataRecordAtOffset(record, record->offsetOfDataRecord);
            
        }
        DataRecordPtr getFirstRecordOfColelction(CollectionMetaDataPtr collection)
        {
            DataRecordPtr nullPtr;
            if(collection->offsetOfFirstDataRecord==-1) return nullPtr;
            else
           return fileReader->readDataRecordFromFile(collection->offsetOfFirstDataRecord);
        
        }
        DataRecordPtr getLastRecordOfColelction(CollectionMetaDataPtr collection)
        {
            DataRecordPtr nullPtr;
            if(collection->offsetOfLastDataRecord==-1) return nullPtr;
            else
                return fileReader->readDataRecordFromFile(collection->offsetOfLastDataRecord);
            
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
