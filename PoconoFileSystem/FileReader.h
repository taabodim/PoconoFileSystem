//
//  FileReader.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_FileReader_h
#define PoconoFileSystem_FileReader_h
#include "DataRecord.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "PoconoConfig.h"
#include "DataRecordMetaData.h"
namespace PoconoFileSystem {
    class FileReader {
        std::string filename;
        public :
        FileReader(std::string fileName) : filename(fileName)
        {
            filename=PoconoFileSystem::getFullCollectionName(fileName);
            
        }
        DataRecordPtr readDataRecordFromFile(size_t offset);
        CollectionMetaDataPtr readCollectionMetaDataFromFile(size_t offset);
        DataRecordMetaDataPtr readDataRecordMetaDataFromFile(size_t offset);
            
            
        };
        
        
        typedef std::shared_ptr<FileReader> FileReaderPtr;
    CollectionMetaDataPtr FileReader::readCollectionMetaDataFromFile(size_t offset) {
        
        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");
            
        }
        assert(ptr_myfile);
        fseek ( ptr_myfile , offset , SEEK_SET );
        CollectionMetaDataPtr collMetadata ( new CollectionMetaData());
        fread(&(*collMetadata),sizeof(class CollectionMetaData),1,ptr_myfile);
        
        fclose(ptr_myfile);
        return collMetadata;
    }
    
    DataRecordPtr FileReader::readDataRecordFromFile(size_t offset) {
        
        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");
            
        }
        assert(ptr_myfile);
        fseek ( ptr_myfile , offset , SEEK_SET );
        DataRecordPtr record ( new DataRecord());
        fread(&(*record),sizeof(class DataRecord),1,ptr_myfile);
        
        fclose(ptr_myfile);
        return record;
    }
    DataRecordMetaDataPtr FileReader::readDataRecordMetaDataFromFile(size_t offset) {
        
        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");
            
        }
        assert(ptr_myfile);
        fseek ( ptr_myfile , offset , SEEK_SET );
        DataRecordMetaDataPtr record ( new DataRecordMeataData());
        fread(&(*record),sizeof(class DataRecordMeataData),1,ptr_myfile);
        fclose(ptr_myfile);
        return record;
    }
    
    
    
    }

#endif
