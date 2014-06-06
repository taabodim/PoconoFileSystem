//
//  FileSystem.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_FileSystem_h
#define PoconoFileSystem_FileSystem_h

namespace PoconoFileSystem {
    class FileSystem {
        private :
        FileReaderPtr fileReader;
        FileWriterPtr fileWriter;
        
        public :
        
        void createCollection(string nameOfCollection){
            
        }
        
        void deleteCollection(string nameOfCollection){
        }
        
        void insertData(string nameOfCollection,DataRecordPtr record)
        {
            
        }
        
        void deleteData(string nameOfCollection,DataRecordPtr record)
        {
            
        }
        
        void updateData(string nameOfCollection,DataRecordPtr record)
        {
            
        }

        
    };
}

#endif
