//
//  FileSystem.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#include "FileSystem.h"


namespace PoconoDB {
   FileSystem::FileSystem(std::string fileName)
    {

                this->filename = PoconoDB::getFullCollectionName(filename);
                openFileIfItDoesntExist(filename);
                FileReaderPtr tmpReader (new FileReader(filename));
                FileWriterPtr tmpWriter (new FileWriter(filename));
                this->fileReader = tmpReader;
                this->fileWriter = tmpWriter;

                //read all the collectionOffsets from the file , and load the collectionMap

                loadAllCollectionMap();
    }

FileSystem::FileSystem()
{

            this->filename = PoconoDB::getFullCollectionName("test");
            openFileIfItDoesntExist(filename);
            FileReaderPtr tmpReader (new FileReader(filename));
            FileWriterPtr tmpWriter (new FileWriter(filename));
            this->fileReader = tmpReader;
            this->fileWriter = tmpWriter;

            //read all the collectionOffsets from the file , and load the collectionMap

            loadAllCollectionMap();
}

        CollectionMetaDataPtr FileSystem::openCollectionFromFile(std::string nameOfCollection) {
                offsetType offsetToGetMetaDataFrom = STARTING_OFFSET_OF_COLLECITON_INDEXS;
            while(offsetToGetMetaDataFrom<ENDING_OFFSET_OF_COLLECITON_INDEXS)
            {
                //                dbLogger->log(toStr(boost::format("\n loadAllCollectionMap : offset %1%") % offsetToGetMetaDataFrom));
                std::shared_ptr<CollectionMetaData> collMetaData ( new CollectionMetaData());
                fileReader->readCollectionMetaDataFromFile(collMetaData,offsetToGetMetaDataFrom);
                if(collMetaData!=NULL)
                {

                    if(collMetaData->getNameOfCollectionAsString().compare(nameOfCollection)==0)
                    {
                       if(collMetaData->isCollectionDeleted!=DELETED)
                       {
                           std::cout<<"loaded this collection from file : "<<collMetaData->toString()<<std::endl;
                            return collMetaData;
                       }
                    }

                    offsetToGetMetaDataFrom +=(sizeof(class  CollectionMetaData));
                }
            }
          std::cout<<"tried to load a collection from file but failed: "<<nameOfCollection<<std::endl;
         assert(false);
         return NULL;
        }
        CollectionMetaDataPtr FileSystem::openCollection(std::string nameOfCollection)
        {

            CollectionMetaDataPtr collMetaData = getCollectionMetaData(nameOfCollection);
            if(collMetaData!=NULL)
            {
                return collMetaData;
            }
            else {

                std::shared_ptr<CollectionMetaData> collectionMetada (new CollectionMetaData(nameOfCollection));

                offsetType offsetOfCollectionMetaDataInFile =STARTING_OFFSET_OF_COLLECITON_INDEXS + ((allCollectionsMap.size()) * sizeof(class CollectionMetaData));

                assert(offsetOfCollectionMetaDataInFile>=0);

                collectionMetada->offsetOfCollectionMetaDataInFile = offsetOfCollectionMetaDataInFile;
//                std::cout<<"openCollection: going to write this collection in File"<<collectionMetada->toString()<<std::endl;
                allCollectionsMap.push_back(collectionMetada);

                fileWriter->writeCollectionMetaData(collectionMetada,offsetOfCollectionMetaDataInFile);
                if(debug) {
                    std::shared_ptr<CollectionMetaData> verifyObject (new CollectionMetaData());
                    fileReader->readCollectionMetaDataFromFile(verifyObject,offsetOfCollectionMetaDataInFile);
                    assert(verifyObject==collectionMetada);
                }
                return collectionMetada;
            }
        }
        void FileSystem::loadAllCollectionMap()
        {
            offsetType offsetToGetMetaDataFrom = STARTING_OFFSET_OF_COLLECITON_INDEXS;
            while(offsetToGetMetaDataFrom<ENDING_OFFSET_OF_COLLECITON_INDEXS)
            {
                //                dbLogger->log(toStr(boost::format("\n loadAllCollectionMap : offset %1%") % offsetToGetMetaDataFrom));
                CollectionMetaDataPtr collMetaData(new CollectionMetaData());
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
        std::string FileSystem::createCollection(std::string nameOfCollection){
            //1. Check if collection doesnt exits,if it exists , return false
            //2.if not,create a record in collectionMap , and append the record in file later change these to vars ,
            std::string res;
            if(!collectionExists(nameOfCollection))
            {
                size_t offset = PoconoDB::getEndOfFileOffset(filename);
                CollectionMetaDataPtr colIndex(new CollectionMetaData(nameOfCollection));

                size_t offsetOfCollectionIndex =STARTING_OFFSET_OF_COLLECITON_INDEXS+ allCollectionsMap.size()*sizeof(class CollectionMetaData);
                std::cout<< " offsetOfCollectionIndex : "<<offsetOfCollectionIndex<<std::endl;

                fileWriter->writeCollectionMetaData(colIndex,offsetOfCollectionIndex);
                allCollectionsMap.push_back(colIndex);
                res.append("collection was added");

//            if(debug) {
//                    std::shared_ptr<CollectionMetaData> verifyObject (new CollectionMetaData());
//                    fileReader->readCollectionMetaDataFromFile(verifyObject,offsetOfCollectionMetaDataInFile);
//                    assert(verifyObject==colindex);
//                }

                return res;
            }
            res.append("collection already exists");
            return res;
        }
        bool FileSystem::collectionExists(std::string nameOfCollection) {
            return false;//for now
        }
        void FileSystem::linkTheLastMetaDataAndNewOneAndUpdateTheLastMetaData( CollectionMetaDataPtr collectionMetaData,DataRecordMetaDataPtr dataRecordMetaDataPtr)
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


        void FileSystem::insertData( CollectionMetaDataPtr collectionMetaData,DataRecordPtr record)
        {


            offsetType offsetOfDataRecordMetaData = PoconoDB::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024 , //getting a proper offset
            offsetType offsetOfDataRecord =offsetOfDataRecordMetaData + sizeof(struct DataRecordMetaDataStruct) * 2;
            offsetType offsetOfValueField = offsetOfDataRecord + sizeof(struct DataRecordStruct)* 2;


            //prepare DataRecord meta data
            std::shared_ptr<DataRecordMeataData> dataRecordMetaData (new DataRecordMeataData());

            dataRecordMetaData->offsetOfDataRecord = offsetOfDataRecord;
            dataRecordMetaData->offsetOfDataRecordMetaData=offsetOfDataRecordMetaData;
            dataRecordMetaData->offsetOfNextDataRecordMetaData=-1;
            dataRecordMetaData->offsetOfPreviousDataRecordMetaData=-1;
            dataRecordMetaData->offsetOfCollection=collectionMetaData->offsetOfCollectionMetaDataInFile;

            dataRecordMetaData->offsetOfValueOfRecordInFile = offsetOfValueField;
            dataRecordMetaData->lengthOfValueField =record->sizeOfValueFieldInDataRecord;


            //prepare DataRecord
            record->offsetOfCollection = collectionMetaData->offsetOfCollectionMetaDataInFile;
            record->sizeOfValueFieldInDataRecord = record->sizeOfValueFieldInDataRecord;
            record->dataRecordRemovedFlag = false;
            record->offsetOfValueOfRecordInFile = offsetOfValueField;
            record->offsetOfDataRecord = offsetOfDataRecord;

            //prepare CollectionMetaData
            if(collectionMetaData->offsetOfFirstDataRecordMetaData==-1)
            {
                collectionMetaData->offsetOfFirstDataRecordMetaData = offsetOfDataRecordMetaData;
            }


            /* writing the value field in file */

            this->fileWriter->writeTheValueOfRecord(record,offsetOfValueField);
            bool debug = true;
            if(debug)
            {
                string valueRead = fileReader->readTheValueOfDataRecord(offsetOfValueField,record->sizeOfValueFieldInDataRecord);
                std::string valueExpected = record->getValueAsString();

                std::cout<< " valueRead is : "<<valueRead<<std::endl<<
                "valueExpected is : "<<valueExpected<<std::endl;
                assert(valueRead.compare(valueExpected)==0);

            }
            /*********************************/


            fileWriter->writeDataRecordAtOffset(record,offsetOfDataRecord);


            linkTheLastMetaDataAndNewOneAndUpdateTheLastMetaData(collectionMetaData,dataRecordMetaData);//insert the Meta Data about the record after inserting the record

            fileWriter->writeDataRecordMetaData(dataRecordMetaData);
            fileWriter->writeCollectionMetaData(collectionMetaData,collectionMetaData->offsetOfCollectionMetaDataInFile);
            updateTheCollectionMetaDataInMemory(collectionMetaData);

            if(debug) {
                    std::shared_ptr<CollectionMetaData> verifyObject (new CollectionMetaData());
                    fileReader->readCollectionMetaDataFromFile(verifyObject,collectionMetaData->offsetOfCollectionMetaDataInFile);
                    assert(verifyObject==collectionMetaData);
                }
        }
        void FileSystem::updateTheCollectionMetaDataInMemory(CollectionMetaDataPtr collectionMetaData) {
         CollectionMetaDataPtr oldCollMetaData = getCollectionMetaData(collectionMetaData->getNameOfCollectionAsString() );
         if(oldCollMetaData!=NULL)
            {
                oldCollMetaData->offsetOfFirstDataRecordMetaData = collectionMetaData->offsetOfFirstDataRecordMetaData;
                oldCollMetaData->offsetOfLastDataRecordMetaData = collectionMetaData->offsetOfLastDataRecordMetaData;
            }
        }

        std::list<DataRecordPtr> FileSystem::getAllData(CollectionMetaDataPtr collectionArg)
        {
            //1.get the most updated version of collectionMetaData
            ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
            DataRecordMetaDataPtr firstDataMetaDataPtr(new DataRecordMeataData());

            std::shared_ptr<CollectionMetaData> collection (new CollectionMetaData());

            fileReader->readCollectionMetaDataFromFile(collection,collectionArg->offsetOfCollectionMetaDataInFile);
            assert(collection->offsetOfCollectionMetaDataInFile>=0);
            assert(collection->offsetOfFirstDataRecordMetaData>=0);
            //DataRecordMetaDataPtr firstDataMetaDataPtr = getARecordMetaDataOnHeap();

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
                dataPtr->setValue(valueOfDataRecord,nextDataRecordMetaDataPtr->lengthOfValueField);

                std::cout<<" pushing back data dataPtr->offsetOfNextDataRecordMetaData : "<<nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData<<",dataPtr->toString() "<<dataPtr->toString()<<std::endl;


                allData.push_back(dataPtr);

                fileReader->readDataRecordMetaDataFromFile(nextDataRecordMetaDataPtr,nextDataRecordMetaDataPtr->offsetOfNextDataRecordMetaData);


            }
            return allData;
        }

        DataRecordMetaDataPtr FileSystem::getLastDataRecordMetaDataOfCollection(CollectionMetaDataPtr collection)
        {
           std::shared_ptr<DataRecordMeataData> record (new DataRecordMeataData());
            if(collection->offsetOfLastDataRecordMetaData==-1) return NULL;
            else
            {
                 fileReader->readDataRecordMetaDataFromFile(record,collection->offsetOfLastDataRecordMetaData);
                return record;
            }
        }
        CollectionMetaDataPtr FileSystem::getCollectionMetaData (std::string nameOfCollection)
        {

           typedef std::list<CollectionMetaDataPtr>::iterator iterType;

            for(iterType it = allCollectionsMap.begin();
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
            std::cout<<" didn't find the collection " <<nameOfCollection << " retuning nullptr";
            return NULL;//
        }

        void FileSystem::deleteCollection(std::string nameOfCollection){
            //mark the collection as deleted....
            CollectionMetaDataPtr collecitonPtr = getCollectionMetaData (nameOfCollection);
            if(collecitonPtr==NULL) {
                //collecitonPtr  = getNewInstanceFromHeap(nameOfCollection);
                return ;
            }

            collecitonPtr->isCollectionDeleted = DELETED;//88  means its deleted
            fileWriter->writeCollectionMetaData(collecitonPtr,collecitonPtr->offsetOfCollectionMetaDataInFile);

            typedef std::list<CollectionMetaDataPtr>::iterator iterType;

            for(iterType it = allCollectionsMap.begin();
				it != allCollectionsMap.end(); ++it)
            {
                CollectionMetaDataPtr colPtr = *it;
                std::string str = colPtr->getNameOfCollectionAsString();

                if(str.compare(nameOfCollection)==0)
                {
                    allCollectionsMap.remove(colPtr);
                    break;
                }
            }


        }

          list<DataRecordPtr> FileSystem::findByValue(std::string nameOfCollection,std::string value) {
             list<DataRecordPtr> response;
            CollectionMetaDataPtr collectionPtr = openCollection(nameOfCollection);
            ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
            getAllData(collectionPtr);
            for(std::list<DataRecordPtr>::iterator it = allData.begin();
                it!=allData.end();++it)
            {
                if((*it)->valueIsEqualTo(value))
                {
                    std::cout<<"find:  Data Record found for value "<<value<<" : "<<(*it)->toString()<<std::endl;
                      //DataRecordPtr ptr(new DataRecord(*it));
                      response.push_back(*it);
                 }
            }

            return response;
        }


        DataRecordPtr FileSystem::find(std::string nameOfCollection,std::string key) {
            return findByKey(nameOfCollection, key);
        }

         DataRecordPtr FileSystem::findByKey(std::string nameOfCollection,std::string key) {
            DataRecordPtr response;
            CollectionMetaDataPtr collectionPtr = openCollectionFromFile(nameOfCollection);
            ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
            getAllData(collectionPtr);
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


        std::string FileSystem::deleteData(std::string nameOfCollection,std::string key)
        {
            DataRecordPtr recordPtr =  find(nameOfCollection,key);
            if(recordPtr==NULL) {
                string resp("There is no data associated with this key : ");
                resp.append(key);
                return resp;
            }
            else
            {
               std::shared_ptr<DataRecordMeataData> currentRecordMetaData (new DataRecordMeataData());
               std::shared_ptr<DataRecordMeataData> previouseRecordMetaData (new DataRecordMeataData());
               std::shared_ptr<DataRecordMeataData> nextRecordMetaData (new DataRecordMeataData());

                fileReader->readDataRecordMetaDataFromFile(currentRecordMetaData,recordPtr->offsetOfDataRecordMetaData);
                fileReader->readDataRecordMetaDataFromFile(previouseRecordMetaData,currentRecordMetaData->offsetOfPreviousDataRecordMetaData);
                fileReader->readDataRecordMetaDataFromFile(nextRecordMetaData,currentRecordMetaData->offsetOfNextDataRecordMetaData);


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

        void FileSystem::addOffsetToFreeListOfCurrentMetaDataOffsets(offsetType freeOffsetOfDataRecordMetaData){

            //add this offset to a list and use this list for inserting new datarecordmetadata
        }

        std::string FileSystem::updateData(std::string nameOfCollection,std::string key,std::string valueToBeOverwritten)
        {
            string resp;
            DataRecordPtr record =  find(nameOfCollection,key);
            if(record==NULL) {
                resp.append("There is no data associated with this key : ");
                resp.append(key);
                return resp;
            }
            else
            {
             CollectionMetaDataPtr collecitonPtr = getCollectionMetaData (nameOfCollection);
            record->setValue(valueToBeOverwritten,valueToBeOverwritten.size());
            offsetType offsetOfnewValue = PoconoDB::getEndOfFileDataBlockOffsetAsMultipleOfBlock(filename, BLOCK_SIZE);//this should be 0 or 1024 , //getting a proper offset
            std::shared_ptr<DataRecordMeataData> dataRecordMetaData (new DataRecordMeataData());
            fileReader->readDataRecordMetaDataFromFile(dataRecordMetaData,record->offsetOfDataRecordMetaData);



            dataRecordMetaData->offsetOfValueOfRecordInFile = offsetOfnewValue;
            dataRecordMetaData->lengthOfValueField =record->getValueAsString().size();


            //update the  DataRecord
            record->offsetOfCollection = collecitonPtr->offsetOfCollectionMetaDataInFile;
            record->sizeOfValueFieldInDataRecord = record->getValueAsString().size();
            record->dataRecordRemovedFlag = false;
            record->offsetOfValueOfRecordInFile = offsetOfnewValue;





            /* writing the new value field in file */

            fileWriter->writeTheValueOfRecord(record,offsetOfnewValue);
            std::string valueExpected = record->getValueAsString();

            assert(fileReader->readTheValueOfDataRecord(offsetOfnewValue,record->sizeOfValueFieldInDataRecord).compare(valueExpected)==0);
            /*********************************/

            fileWriter->writeDataRecordAtOffset(record,record->offsetOfDataRecord);
            fileWriter->writeDataRecordMetaData(dataRecordMetaData);
            }

            resp.append("Data was updated , associated with this key : ");
            resp.append(key);

            return resp;

        }
        std::list<std::string> FileSystem::showAllCollections() {
            //this loads the collection without caching it to make sure that
            //the delete collection and other possible functionality
            //works fine
            loadAllCollectionMap();
            list<std::string> allCollectionNames;
            for(list<CollectionMetaDataPtr>::iterator iter = allCollectionsMap.begin();
                iter!=allCollectionsMap.end();++iter) {
                allCollectionNames.push_back((*iter)->getNameOfCollectionAsString());
            }

            return allCollectionNames;
         }


}

