//
//  FileWriter.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_FileWriter_h
#define PoconoFileSystem_FileWriter_h
#include "DataRecord.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "CollectionIndex.h"
namespace PoconoFileSystem {
    class FileWriter {
        private :
        std::string filename;
        public :
        FileWriter(std::string fileName) 
        {
            filename=PoconoFileSystem::getFullCollectionName(fileName);
            if(!PoconoFileSystem::checkFileExist(filename))
            {
                PoconoFileSystem::openFileIfItDoesntExist(filename);
            }
        }
        void writeDataRecordAtOffset(std::shared_ptr<DataRecord> record, size_t record_offset);
        void writeCollectionMetaData(CollectionMetaDataPtr colIndex,size_t offsetOfCollectionIndex);
        void writeCollectionMetaData(CollectionMetaDataPtr colIndex);

     };
    typedef std::shared_ptr<FileWriter> FileWriterPtr;
    
    
    void FileWriter::writeCollectionMetaData(CollectionMetaDataPtr colIndex,size_t offsetOfCollectionIndex)
    {
        FILE *ptr_myfile;
        //        struct recordInDatabase my_record;
        
        //     std::unique_lock < std::mutex > writeLock(writeMutex);
        
        ptr_myfile=fopen(filename.c_str(),"r+b");
        if (!ptr_myfile)
        {
            //            mylogger<<"Unable to open file!";
            
        }
        
        assert(ptr_myfile);
        
        fseek ( ptr_myfile , offsetOfCollectionIndex , SEEK_SET );
        fwrite(&(*colIndex), sizeof(class CollectionMetaData), 1, ptr_myfile);
        fflush(ptr_myfile);
        fclose(ptr_myfile);
    
    
    }
    void FileWriter::writeCollectionMetaData(CollectionMetaDataPtr colIndex)
    {
        FILE *ptr_myfile;
        //        struct recordInDatabase my_record;
        
        //     std::unique_lock < std::mutex > writeLock(writeMutex);
        
        ptr_myfile=fopen(filename.c_str(),"r+b");
        if (!ptr_myfile)
        {
            //            mylogger<<"Unable to open file!";
            
        }
        
        assert(ptr_myfile);
        std::cout<<"writing collectionMetaData at offset "<<colIndex->offsetOfCollectionMetaDataInFile<<" "<<colIndex->toString()<<std::endl;
        fseek ( ptr_myfile , colIndex->offsetOfCollectionMetaDataInFile , SEEK_SET );
        fwrite(&(*colIndex), sizeof(class CollectionMetaData), 1, ptr_myfile);
        fflush(ptr_myfile);
        fclose(ptr_myfile);
        
        
    }
    
    
    void  FileWriter::writeDataRecordAtOffset (std::shared_ptr<DataRecord> record, size_t record_offset) {
        
        FILE *ptr_myfile;
//        struct recordInDatabase my_record;
        
   //     std::unique_lock < std::mutex > writeLock(writeMutex);
        
        ptr_myfile=fopen(filename.c_str(),"r+b");
        if (!ptr_myfile)
        {
//            mylogger<<"Unable to open file!";
            
        }
        
        assert(ptr_myfile);
        
        fseek ( ptr_myfile , record_offset , SEEK_SET );
        fwrite(&(*record), sizeof(class DataRecord), 1, ptr_myfile);
        fflush(ptr_myfile);
        fclose(ptr_myfile);
        std::cout<<" wrote this DataRecord to file at offset "<<record_offset<<" , "<<record->toString()<<std::endl;
    }
}

#endif
