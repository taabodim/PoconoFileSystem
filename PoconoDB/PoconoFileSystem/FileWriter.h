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
        void writeCollectionMetaData(CollectionMetaDataRawPtr collectionMetaDataPtr,offsetType offsetOfCollectionMetaData);

        void writeDataRecordMetaData(DataRecordMetaDataPtr dataRecordMetaDataPtr);
        void writeTheValueOfRecord(DataRecordPtr record,offsetType offsetOfValueOfRecordInFile);

    };


    typedef std::shared_ptr<FileWriter> FileWriterPtr;

    void FileWriter::writeDataRecordMetaData(DataRecordMetaDataPtr dataRecordMetaDataPtr)
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
        assert(dataRecordMetaDataPtr->offsetOfDataRecordMetaData>-1);


        DataRecordMetaDataStruct dataRecordStruct;

        dataRecordStruct.offsetOfDataRecord =dataRecordMetaDataPtr->offsetOfDataRecord;
        dataRecordStruct.offsetOfValueOfRecordInFile = dataRecordMetaDataPtr->offsetOfValueOfRecordInFile;
        dataRecordStruct.offsetOfDataRecordMetaData=dataRecordMetaDataPtr->offsetOfDataRecordMetaData;
        dataRecordStruct.offsetOfNextDataRecordMetaData=dataRecordMetaDataPtr->offsetOfNextDataRecordMetaData;
        dataRecordStruct.offsetOfPreviousDataRecordMetaData=dataRecordMetaDataPtr->offsetOfPreviousDataRecordMetaData;
        dataRecordStruct.offsetOfCollection=dataRecordMetaDataPtr->offsetOfCollection;
        dataRecordStruct.lengthOfValueField=dataRecordMetaDataPtr->lengthOfValueField;


        fseek ( ptr_myfile , dataRecordMetaDataPtr->offsetOfDataRecordMetaData , SEEK_SET );
        fwrite(&dataRecordStruct,sizeof(struct DataRecordMetaDataStruct),1,ptr_myfile);

        fflush(ptr_myfile);
        fclose(ptr_myfile);

         assert(dataRecordMetaDataPtr->offsetOfDataRecordMetaData>-1);


    }

    void FileWriter::writeCollectionMetaData(CollectionMetaDataRawPtr colIndex,offsetType offsetOfCollectionIndex)
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
        CollectionMetaDataStruct colStructPtr;

        for(int i=0;i<32;i++)
        {
            colStructPtr.nameOfCollection[i]=colIndex->nameOfCollection[i];
        }

        colStructPtr.offsetOfLastDataRecordMetaData=colIndex->offsetOfLastDataRecordMetaData;
        colStructPtr.offsetOfCollectionMetaDataInFile=colIndex->offsetOfCollectionMetaDataInFile;
        colStructPtr.offsetOfFirstDataRecordMetaData=colIndex->offsetOfFirstDataRecordMetaData;
        colStructPtr.isCollectionDeleted = colIndex->isCollectionDeleted;


        fseek ( ptr_myfile , offsetOfCollectionIndex , SEEK_SET );
        fwrite(&colStructPtr, sizeof(struct CollectionMetaDataStruct), 1, ptr_myfile);
        fflush(ptr_myfile);
        fclose(ptr_myfile);


    }
      void FileWriter::writeTheValueOfRecord(DataRecordPtr record,offsetType offsetOfValueOfRecordInFile){
        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"r+b");
        if (!ptr_myfile)
        {
//            mylogger<<"Unable to open file!";

        }

        assert(ptr_myfile);
        //assert(record->offsetOfDataRecord>-1);
        fseek ( ptr_myfile , offsetOfValueOfRecordInFile, SEEK_SET );
        std::string valueAsStr = record->getValueAsString();

        fwrite(valueAsStr.c_str(), valueAsStr.size(), 1, ptr_myfile);
        fflush(ptr_myfile);

        fclose(ptr_myfile);
        std::cout<<" wrote this value of data record at offset "<<offsetOfValueOfRecordInFile<<std::endl;

    }

    void  FileWriter::writeDataRecordAtOffset (DataRecordPtr record,offsetType offsetOfDataRecord) {

        FILE *ptr_myfile;
//        struct recordInDatabase my_record;

   //     std::unique_lock < std::mutex > writeLock(writeMutex);

        ptr_myfile=fopen(filename.c_str(),"r+b");
        if (!ptr_myfile)
        {
//            mylogger<<"Unable to open file!";

        }

        assert(ptr_myfile);
        //assert(record->offsetOfDataRecord>-1);

        DataRecordStruct dataRecordStruct;

        for(int i=0;i<32;i++)
        {
            dataRecordStruct.key[i]=record->key[i];
        }

        dataRecordStruct.sizeOfValueFieldInDataRecord=record->sizeOfValueFieldInDataRecord;
        dataRecordStruct.dataRecordRemovedFlag=record->dataRecordRemovedFlag;
        dataRecordStruct.offsetOfValueOfRecordInFile = record->offsetOfValueOfRecordInFile;
        dataRecordStruct.offsetOfCollection = record->offsetOfCollection;
        dataRecordStruct.offsetOfDataRecordMetaData = record->offsetOfDataRecordMetaData;
        dataRecordStruct.offsetOfDataRecord =record->offsetOfDataRecord;

        fseek ( ptr_myfile , offsetOfDataRecord , SEEK_SET );
        fwrite(&dataRecordStruct, sizeof(struct DataRecordStruct), 1, ptr_myfile);
        fflush(ptr_myfile);
        fclose(ptr_myfile);
        std::cout<<" wrote this DataRecord to file at offset "<<offsetOfDataRecord<<" , "<<record->toString()<<std::endl;
    }


}

#endif
