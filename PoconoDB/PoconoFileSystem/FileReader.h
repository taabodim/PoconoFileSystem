//
//  FileReader.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_FileReader_h
#define PoconoDB_FileReader_h
#include "DataRecord.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "PoconoConfig.h"
#include "DataRecordMetaData.h"
#include "LoggerWrapper.h"
#include "pico_logger_wrapper.h"
namespace PoconoDB {


    class FileReader : public pico_logger_wrapper {
        private :

            std::string filename;
        public :
        FileReader(std::string fileName);
        DataRecordPtr readDataRecordFromFile(offsetType offset);
        void readCollectionMetaDataFromFile(std::shared_ptr<CollectionMetaData> collectionPtr,offsetType offset);
        void readDataRecordMetaDataFromFile(DataRecordMetaDataPtr record,offsetType offset);
        std::string readTheValueOfDataRecord(offsetType offset,offsetType sizeOfValueField);

        };


        typedef std::shared_ptr<FileReader> FileReaderPtr;


    }

#endif
