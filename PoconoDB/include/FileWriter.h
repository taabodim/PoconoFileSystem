//
//  FileWriter.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_FileWriter_h
#define PoconoDB_FileWriter_h
#include "DataRecord.h"
#include "DataRecordMetaData.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cassert>
#include "CollectionMetaData.h"
namespace PoconoDB {
    class FileWriter {
        private :
        std::string filename;
        public :
        FileWriter(std::string fileName)
        {
            this->filename=fileName;
        }
        void writeDataRecordAtOffset(DataRecordPtr record,offsetType offsetOfDataRecord);
        void writeCollectionMetaData(CollectionMetaDataPtr collectionMetaDataPtr,offsetType offsetOfCollectionMetaData);

        void writeDataRecordMetaData(DataRecordMetaDataPtr dataRecordMetaDataPtr);
        void writeTheValueOfRecord(DataRecordPtr record,offsetType offsetOfValueOfRecordInFile);

    };


    typedef std::shared_ptr<FileWriter> FileWriterPtr;

}

#endif
