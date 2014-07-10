//
//  FileSystemAPI.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/14/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_FileSystemAPI_h
#define PoconoDB_FileSystemAPI_h

#include "DataRecord.h"
#include "FileSystem.h"
namespace PoconoDB {
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
        ListOfDataRecordPtr  getAllData(CollectionMetaDataPtr collection)
        {
           return  fs_.getAllData(collection);
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
