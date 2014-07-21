//
//  FileSystem.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_FileSystem_h
#define PoconoDB_FileSystem_h
#include "FileWriter.h"
#include "FileReader.h"
#include "CollectionMetaData.h"
#include <list>
#include "SuperBlock.h"
#include "Utils.h"
#include "DataRecordMetaData.h"
#include "pico_logger_wrapper.h"
namespace PoconoDB {
    class FileSystem : public pico_logger_wrapper {
        private :
            const static int DELETED = 88;
            std::string filename;
            FileReaderPtr fileReader;
            FileWriterPtr fileWriter;
            std::list<CollectionMetaDataPtr> allCollectionsMap;

        public :


            FileSystem(std::string fileName);

            CollectionMetaDataPtr openCollectionFromFile(std::string nameOfCollection);
            CollectionMetaDataPtr openCollection(std::string nameOfCollection);
            void loadAllCollectionMap();
            std::string createCollection(std::string nameOfCollection);
            bool collectionExists(std::string nameOfCollection);
            void linkTheLastMetaDataAndNewOneAndUpdateTheLastMetaData( CollectionMetaDataPtr collectionMetaData,DataRecordMetaDataPtr dataRecordMetaDataPtr);
            void insertData( CollectionMetaDataPtr collectionMetaData,DataRecordPtr record);
            DataRecordMetaDataPtr getLastDataRecordMetaDataOfCollection(CollectionMetaDataPtr collection);
            CollectionMetaDataPtr getCollectionMetaData (std::string nameOfCollection);
            void deleteCollection(std::string nameOfCollection);
            list<DataRecordPtr> findByValue(std::string nameOfCollection,std::string value);
            DataRecordPtr find(std::string nameOfCollection,std::string key);
            DataRecordPtr findByKey(std::string nameOfCollection,std::string key);
            std::string deleteData(std::string nameOfCollection,std::string key);
            void addOffsetToFreeListOfCurrentMetaDataOffsets(offsetType freeOffsetOfDataRecordMetaData);
            std::string updateData(std::string nameOfCollection,std::string key,std::string valueToBeOverwritten);
            std::list<std::string> showAllCollections();

    };
}

#endif
