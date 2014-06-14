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
        }
        
        void insertData(CollectionMetaDataPtr collection,DataRecordPtr record)
        {
            fs_.insertData(collection,record);
        }
        std::shared_ptr<std::list<DataRecordPtr>> getAllData(CollectionMetaDataPtr collection)
        {
            return fs_.getAllData(collection);
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
