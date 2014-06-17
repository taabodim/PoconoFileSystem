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
#include "CollectionMetaData.h"
#include <list>
#include "SuperBlock.h"
#include "Utils.h"
#include "DataRecordMetaData.h"
namespace PoconoFileSystem {
    class FileSystem : public loggerWrapper {
        private :
        const static int DELETED = 88;
        std::string filename;
        FileReaderPtr fileReader;
        FileWriterPtr fileWriter;
        //        std::list<CollectionMetaDataPtr> allCollectionsMap; //change this to map later
        std::list<CollectionMetaDataRawPtr> allCollectionsMap;
        
        
//        SuperBlock superBlock;
        public :
        
        
        FileSystem(std::string fileName):filename(fileName),fileReader(new FileReader(filename)),
        fileWriter(new FileWriter(filename))
        {
            filename = PoconoFileSystem::getFullCollectionName(filename);
            openFileIfItDoesntExist(filename);
            //read all the collectionOffsets from the file , and load the collectionMap
            
            loadAllCollectionMap();
            
        }
        CollectionMetaDataPtr openCollection(std::string nameOfCollection)
        {
            
            CollectionMetaDataPtr collMetaData = getCollectionMetaData(nameOfCollection);
            if(collMetaData!=NULL)
            {
                return collMetaData;
            }
            else {
                
                //                CollectionMetaDataPtr colIndex(new CollectionMetaData(nameOfCollection));
                CollectionMetaDataRawPtr colIndex = new CollectionMetaData(nameOfCollection);
                
                offsetType offsetOfCollectionMetaDataInFile =STARTING_OFFSET_OF_COLLECITON_INDEXS + ((allCollectionsMap.size()-1) * sizeof(class CollectionMetaData));
                colIndex->offsetOfCollectionMetaDataInFile = offsetOfCollectionMetaDataInFile;
                
                allCollectionsMap.push_back(colIndex);
                
                fileWriter->writeCollectionMetaData(colIndex,offsetOfCollectionMetaDataInFile);
                return colIndex;
            }
        }
        void loadAllCollectionMap()
        {
            offsetType offsetToGetMetaDataFrom = STARTING_OFFSET_OF_COLLECITON_INDEXS;
            while(offsetToGetMetaDataFrom<ENDING_OFFSET_OF_COLLECITON_INDEXS)
            {
                //                dbLogger->log(toStr(boost::format("\n loadAllCollectionMap : offset %1%") % offsetToGetMetaDataFrom));
                CollectionMetaDataRawPtr collMetaData = new CollectionMetaData();
                fileReader->readCollectionMetaDataFromFile(collMetaData,offsetToGetMetaDataFrom);
                if(collMetaData!=NULL)
                {
                    
                    if(!collMetaData->getNameOfCollectionAsString().empty())
                    {
                       if(collMetaData->isCollectionDeleted!=DELETED)
                       {
                           allCollectionsMap.push_back(collMetaData);
                       }
                    }
                    
                    offsetToGetMetaDataFrom +=(sizeof(class  CollectionMetaData));
                }
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
                //                CollectionMetaDataPtr colIndex(new CollectionMetaData(nameOfCollection));
                CollectionMetaDataRawPtr colIndex=new CollectionMetaData(nameOfCollection);
                
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
        void linkTheLastMetaDataAndNewOneAndUpdateTheLastMetaData( CollectionMetaDataPtr collectionMetaData,DataRecordMetaDataPtr dataRecordMetaDataPtr)
        {
            
            //get the last data record meta data that collection meta data points to
            DataRecordMetaDataPtr lastdataRecordMetaDataOfCollection = getLastDataRecordMetaDataOfCollection(collectionMetaData);
            
            if(lastdataRecordMetaDataOfCollection!=NULL)
                
            {
              
                //update the collectionMetaData to point to the last data record meta data
                
                lastdataRecordMetaDataOfCollection->offsetOfNextDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData; //make the last
                //dataRecordMetaData of collection points to the new data record
                
                fileWriter->writeDataRecordMetaData(lastdataRecordMetaDataOfCollection);
                //updates the last data record meta data
                //that points to the data record that we are about to save right after it.
                
                
                dataRecordMetaDataPtr->offsetOfPreviousDataRecordMetaData = lastdataRecordMetaDataOfCollection->offsetOfDataRecordMetaData;
                
                collectionMetaData->offsetOfLastDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                
               
            }
            else {
                //this is the case that we want to write the first data row in colleciton, the collection
                //doesnt point to anything
                dataRecordMetaDataPtr->offsetOfPreviousDataRecordMetaData = -1;
                collectionMetaData->offsetOfFirstDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                collectionMetaData->offsetOfLastDataRecordMetaData = dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
                
            }
            
            dataRecordMetaDataPtr->offsetOfNextDataRecordMetaData = -1;
            dataRecordMetaDataPtr->offsetOfCollection = collectionMetaData->offsetOfCollectionMetaDataInFile;
           
        }
       
        
        void insertData( CollectionMetaDataPtr collectionMetaData,DataRecordPtr record)
        {
            
            
            offsetType offsetOfDataRecordMetaData = PoconoFileSystem::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024 , //getting a proper offset
            offsetType offsetOfDataRecord =offsetOfDataRecordMetaData + sizeof(struct DataRecordMetaDataStruct) * 2;
            offsetType offsetOfValueField = offsetOfDataRecord + sizeof(struct DataRecordStruct)* 2;
            
            
            //prepare DataRecord meta data
            DataRecordMetaDataPtr dataRecordMetaData(new DataRecordMeataData());
            
            dataRecordMetaData->offsetOfDataRecord = offsetOfDataRecord;
            dataRecordMetaData->offsetOfValueOfRecordInFile = offsetOfValueField;
            dataRecordMetaData->offsetOfDataRecordMetaData=offsetOfDataRecordMetaData;
            dataRecordMetaData->offsetOfNextDataRecordMetaData=-1;
            dataRecordMetaData->offsetOfPreviousDataRecordMetaData=-1;
            dataRecordMetaData->offsetOfCollection=collectionMetaData->offsetOfCollectionMetaDataInFile;
            dataRecordMetaData->lengthOfValueField =record->getValueAsString().size();
            
            
            //prepare DataRecord
            record->offsetOfCollection = collectionMetaData->offsetOfCollectionMetaDataInFile;
            record->sizeOfValueFieldInDataRecord = record->getValueAsString().size();
            record->dataRecordRemovedFlag = false;
            record->offsetOfValueOfRecordInFile = offsetOfValueField;
            
            
            
            
            
            /* writing the value field in file */
            
            fileWriter->writeTheValueOfRecord(record,offsetOfValueField);
            std::string valueExpected = record->getValueAsString();
            
            assert(fileReader->readTheValueOfDataRecord(offsetOfValueField,record->sizeOfValueFieldInDataRecord).compare(valueExpected)==0);
            /*********************************/
            
            
            fileWriter->writeDataRecordAtOffset(record,offsetOfDataRecord);

            
            linkTheLastMetaDataAndNewOneAndUpdateTheLastMetaData(collectionMetaData,dataRecordMetaData);//insert the Meta Data about the record after inserting the record
            
            fileWriter->writeDataRecordMetaData(dataRecordMetaData);

           
            
             fileWriter->writeCollectionMetaData(collectionMetaData,collectionMetaData->offsetOfCollectionMetaDataInFile);
            
        }
        void getAllData(std::list<DataRecordPtr>& allData, CollectionMetaDataPtr collectionArg)
        {
            //1.get the most updated version of collectionMetaData
            CollectionMetaDataRawPtr collection = new CollectionMetaData();
            
            fileReader->readCollectionMetaDataFromFile(collection,collectionArg->offsetOfCollectionMetaDataInFile);
            
            
            
            DataRecordMetaDataPtr firstDataMetaDataPtr = new DataRecordMeataData();
            
            //2. get the first data record and append to the list
            fileReader->readDataRecordMetaDataFromFile(firstDataMetaDataPtr,collection->offsetOfFirstDataRecordMetaData);
            
            //read the data record that is pointed by the first pointer
            DataRecordPtr firstDataPtr = fileReader->readDataRecordFromFile(firstDataMetaDataPtr->offsetOfDataRecord);
            
            //read the value that is pointed by the first pointer
            std::string valueOfDataRecord = fileReader->readTheValueOfDataRecord(firstDataPtr->offsetOfValueOfRecordInFile,firstDataPtr->sizeOfValueFieldInDataRecord);
            
            firstDataPtr->setValue(valueOfDataRecord,firstDataMetaDataPtr->lengthOfValueField);
            
            
            assert(!valueOfDataRecord.empty());
            allData.push_back(firstDataPtr);
    
            
            
            DataRecordMetaDataPtr nextDataRecordMetaDataPtr = firstDataMetaDataPtr;
            
            while(nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData!=-1)
            {
                DataRecordPtr dataPtr = fileReader->readDataRecordFromFile(nextDataRecordMetaDataPtr->offsetOfDataRecord);
                
                std::string valueOfDataRecord = fileReader->readTheValueOfDataRecord(dataPtr->offsetOfValueOfRecordInFile,dataPtr->sizeOfValueFieldInDataRecord);
                
                std::cout<<" pushing back data dataPtr->offsetOfNextDataRecordMetaData : "<<nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData<<std::endl;
                
                dataPtr->setValue(valueOfDataRecord,nextDataRecordMetaDataPtr->lengthOfValueField);
                
                allData.push_back(dataPtr);
                
                fileReader->readDataRecordMetaDataFromFile(nextDataRecordMetaDataPtr,nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData);
                
                
            }
            
        }
        
        DataRecordMetaDataPtr getLastDataRecordMetaDataOfCollection(CollectionMetaDataPtr collection)
        {
            DataRecordMetaDataPtr record = new DataRecordMeataData();
            if(collection->offsetOfLastDataRecordMetaData==-1) return NULL;
            else
            {
                 fileReader->readDataRecordMetaDataFromFile(record,collection->offsetOfLastDataRecordMetaData);
                return record;
            }
        }
        CollectionMetaDataPtr getCollectionMetaData (std::string nameOfCollection)
        {
            
        //            typedef std::list<CollectionMetaDataPtr>::iterator iterType;
            typedef std::list<CollectionMetaDataRawPtr>::iterator iterType;
            
            for(iterType it = allCollectionsMap.begin();
				it != allCollectionsMap.end(); ++it)
            {
                CollectionMetaDataRawPtr colPtr = *it;
                std::string str = colPtr->getNameOfCollectionAsString();
                
                if(str.compare(nameOfCollection)
                   ==0)
                {
                    return colPtr;
                }
            }
            std::cout<<" didn't find the collection, retuning nullptr";
            return NULL;//
        }
        
        void deleteCollection(std::string nameOfCollection){
            //mark the collection as deleted....
            CollectionMetaDataPtr collecitonPtr = getCollectionMetaData (nameOfCollection);
            collecitonPtr->isCollectionDeleted = DELETED;//88  means its deleted
            fileWriter->writeCollectionMetaData(collecitonPtr,collecitonPtr->offsetOfCollectionMetaDataInFile);
            
            typedef std::list<CollectionMetaDataRawPtr>::iterator iterType;
            
            for(iterType it = allCollectionsMap.begin();
				it != allCollectionsMap.end(); ++it)
            {
                CollectionMetaDataRawPtr colPtr = *it;
                std::string str = colPtr->getNameOfCollectionAsString();
                
                if(str.compare(nameOfCollection)==0)
                {
                    allCollectionsMap.remove(colPtr);
                    break;
                }
            }
            
        }
        
        
        
        DataRecordPtr find(std::string nameOfCollection,std::string key) {
            CollectionMetaDataPtr collectionPtr = openCollection(nameOfCollection);
            std::list<DataRecordPtr> allData;
            getAllData(allData,collectionPtr);
            for(std::list<DataRecordPtr>::iterator it = allData.begin();
                it!=allData.end();++it)
            {
                if((*it)->keyIsEqualTo(key))
                {
                    std::cout<<"find:  Data Record found for key "<<key<<" : "<<(*it)->toString()<<std::endl;
                    
                    return (*it);
                 }
            }

            
            return NULL;
        }
        
        
        std::string deleteData(std::string nameOfCollection,std::string key)
        {
            DataRecordPtr recordPtr =  find(nameOfCollection,key);
            if(recordPtr==NULL) {
                string resp("There is no data associated with this key : ");
                resp.append(key);
                return resp;
            }
            else
            {
                DataRecordMetaDataPtr previouseRecordMetaData = getRecordMetaData(recordPtr->offsetOfPreviousDataRecordMetaData);
                DataRecordMetaDataPtr nextRecordMetaData = getRecordMetaData(recordPtr->offsetOfNextDataRecordMetaData);
                DataRecordMetaDataPtr currentRecordMetaData = getRecordMetaData(recordPtr->offsetOfCurrentDataRecordMetaData);

                //linking the previous to the next and the next to the previous
                previouseRecordMetaData->offsetOfNextDataRecordMetaData = nextRecordMetaData->offsetOfDataRecordMetaData;
                nextRecordMetaData->offsetOfPreviousDataRecordMetaData  = previouseRecordMetaData->offsetOfDataRecordMetaData;
                currentRecordMetaData->isDataRecordMetaDeleted = DELETED;

                addOffsetToFreeListOfCurrentMetaDataOffsets(currentRecordMetaData->offsetOfDataRecordMetaData);
                //we can write the empty charactest instead of value later....TODO

                fileWriter->writeDataRecordMetaData(currentRecordMetaData);
                fileWriter->writeDataRecordMetaData(previouseRecordMetaData);
                fileWriter->writeDataRecordMetaData(nextRecordMetaData);

                string resp("record was deleted with this key : ");
                resp.append(key);
                return resp;
            }
        }  
        void addOffsetToFreeListOfCurrentMetaDataOffsets(offsetType freeOffsetOfDataRecordMetaData){

            //add this offset to a list and use this list for inserting new datarecordmetadata 
        }
                
        std::string updateData(std::string nameOfCollection,std::string key,std::string valueToBeOverwritten)
        {
            DataRecordPtr recordPtr =  find(nameOfCollection,key);
            if(recordPtr==NULL) {
                string resp("There is no data associated with this key : ");
                resp.append(key);
                return resp;
            }
            else
            {
                offsetType offsetOfnewValue = PoconoFileSystem::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024 , //getting a proper offset
           

                DataRecordMetaDataPtr currentRecordMetaData = getRecordMetaData(recordPtr->offsetOfCurrentDataRecordMetaData);
                recordPtr->sizeOfValueFieldInDataRecord = valueToBeOverwritten.size();
                recordPtr->offsetOfValueField = offsetOfnewValue;
                fileWriter->writeTheValueOfRecord(recordPtr,offsetOfnewValue);

                std::string valueExpected = recordPtr->getValueAsString();
                
                fileWriter->writeDataRecordMetaData(currentRecordMetaData);

                fileWriter->

                assert(fileReader->readTheValueOfDataRecord(offsetOfValueField,recordPtr->sizeOfValueFieldInDataRecord).compare(valueExpected)==0);
            }
        }
        std::list<std::string> showAllCollections() {
            //this loads the collection without caching it to make sure that 
            //the delete collection and other possible functionality
            //works fine
            loadAllCollectionMap();
            list<std::string> allCollectionNames;
            for(list<CollectionMetaDataRawPtr>::iterator iter = allCollectionsMap.begin();
                iter!=allCollectionsMap.end();++iter)
            {
                allCollectionNames.push_back((*iter)->getNameOfCollectionAsString());

            }
            
            return allCollectionNames;
         }
        
    };
}

#endif
