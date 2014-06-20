//
//  FileSystemAPI.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/14/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_FileSystemAPI_h
#define PoconoFileSystem_FileSystemAPI_h

#include "DataRecord.h"
#include "FileSystem.h"
namespace PoconoFileSystem {
    class FileSystemAPI {
        private :
        FileSystem fs_;
        public :
        FileSystemAPI():fs_("test"){
            
            
        }
        CollectionMetaDataPtr openCollection(std::string nameOfCollection)
        {
            return fs_.openCollection(nameOfCollection);
            
        }
        void createCollection(std::string nameOfCollection){
            
        }
        
        void deleteCollection(std::string nameOfCollection){
            fs_.deleteCollection(nameOfCollection);

        }
        
        void insertData(CollectionMetaDataPtr collection,DataRecordPtr record)
        {
            fs_.insertData(collection,record);
        }
        void  getAllData(std::list<DataRecordPtr> allData, CollectionMetaDataPtr collection)
        {
           fs_.getAllData(allData,collection);
        }
        
        std::string deleteData(std::string nameOfCollection,std::string key)
        {
            return fs_.deleteData(nameOfCollection,key);
        }
        
        std::string updateData(std::string nameOfCollection,std::string key,std::string valueToBeOverwritten)
            {
             return fs_.updateData(nameOfCollection,key,valueToBeOverwritten);
        }
        list<std::string> showAllCollections() {
            return fs_.showAllCollections();
        }
        DataRecordPtr find(std::string nameOfCollection,std::string key) {
             return fs_.find(nameOfCollection,key);
        }
        
        
    };
}


#endif
