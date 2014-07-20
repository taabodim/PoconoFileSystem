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

//    class FileReader : public LoggerWrapper {
        std::string filename;
        public :
        FileReader(std::string fileName) : filename(fileName)
        {
            this->filename=fileName;

        }
        DataRecord readDataRecordFromFile(offsetType offset);
        void readCollectionMetaDataFromFile(CollectionMetaData* collectionPtr,offsetType offset);
        void readDataRecordMetaDataFromFile(DataRecordMetaDataPtr record,offsetType offset);
        std::string readTheValueOfDataRecord(offsetType offset,offsetType sizeOfValueField);

        };


        typedef std::shared_ptr<FileReader> FileReaderPtr;
    void FileReader::readCollectionMetaDataFromFile(CollectionMetaData* collectionPtr,offsetType offset) {

        FILE *ptr_myfile;
//        std::cout<<"FileReader : filename : "<<filename<<std::endl;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");

        }
        assert(ptr_myfile);
        fseek ( ptr_myfile , offset , SEEK_SET );
        CollectionMetaDataStruct colStruct;

        fread(&colStruct,sizeof(struct CollectionMetaDataStruct),1,ptr_myfile);

        for(int i=0;i<32;i++)
        {
            collectionPtr->nameOfCollection[i] = colStruct.nameOfCollection[i];
        }
        collectionPtr->offsetOfLastDataRecordMetaData = colStruct.offsetOfLastDataRecordMetaData;
        collectionPtr->offsetOfCollectionMetaDataInFile =colStruct.offsetOfCollectionMetaDataInFile;
        collectionPtr->offsetOfFirstDataRecordMetaData= colStruct.offsetOfFirstDataRecordMetaData;
        collectionPtr->isCollectionDeleted = colStruct.isCollectionDeleted;



//
//        std::cout<<"At offset "<<offset<<" with size of "<<sizeof(class CollectionMetaData)<<",this collMetadata was read from file "<<collMetadata->toString()<<std::endl;

//         dbLogger->log(toStr(boost::format("\n loadAllCollectionMap : At offset  %1% with size of %2% ,this collMetadata was read from file %3% \n") %offset
//             %sizeOfClass %collMetadata->toString()));

        fclose(ptr_myfile);

    }
//    DataRecordPtr FileReader::readDataRecordFromFileFake(offsetType offset) {
//        std::shared_ptr<DataRecord> record (new DataRecord());
//        return record;
//    }
    DataRecord FileReader::readDataRecordFromFile(offsetType offset) {



//        DataRecordPtr record  = getARecordDataOnHeap();
//        DataRecordPtr record(new DataRecord());
        //DataRecordStruct* dataRecordStruct (new DataRecordStruct());

        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");

        }
        assert(ptr_myfile);



        fseek ( ptr_myfile , offset , SEEK_SET );
//        std::shared_ptr<DataRecordStruct> dataRecordStruct(new DataRecordStruct());
        DataRecordStruct dataRecordStruct;
        fread(&dataRecordStruct,sizeof(struct DataRecordStruct),1,ptr_myfile);

        //std::shared_ptr<DataRecord> record (new DataRecord());
        DataRecord record;

        for(offsetType i=0;i<MAX_KEY_SIZE;i++)
        {
            record.key[i] = dataRecordStruct.key[i];
        }
        record.sizeOfValueFieldInDataRecord = dataRecordStruct.sizeOfValueFieldInDataRecord;

        record.dataRecordRemovedFlag = dataRecordStruct.dataRecordRemovedFlag;
        record.offsetOfValueOfRecordInFile = dataRecordStruct.offsetOfValueOfRecordInFile;
        record.offsetOfCollection = dataRecordStruct.offsetOfCollection;
        record.offsetOfDataRecordMetaData =   dataRecordStruct.offsetOfDataRecordMetaData ;
        record.offsetOfDataRecord =  dataRecordStruct.offsetOfDataRecord;

//         std::cout<<"At offset "<<offset<<" with size of "<<sizeof(struct DataRecordStruct)<<",this record was read from file "<<record->toString()<<std::endl;

        fclose(ptr_myfile);

        return record;
    }
    void FileReader::readDataRecordMetaDataFromFile(DataRecordMetaDataPtr record,offsetType offset) {

        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");

        }
        assert(ptr_myfile);



        assert(offset>-1);
        fseek ( ptr_myfile , offset , SEEK_SET );
        DataRecordMetaDataStruct dataRecordMetaDataStruct;

        fread(&dataRecordMetaDataStruct,sizeof(struct DataRecordMetaDataStruct),1,ptr_myfile);





        record->lengthOfValueField =dataRecordMetaDataStruct.lengthOfValueField;
        record->offsetOfDataRecord = dataRecordMetaDataStruct.offsetOfDataRecord;
        record->offsetOfValueOfRecordInFile = dataRecordMetaDataStruct.offsetOfValueOfRecordInFile;
        record->offsetOfDataRecordMetaData=dataRecordMetaDataStruct.offsetOfDataRecordMetaData;
        record->offsetOfNextDataRecordMetaData=dataRecordMetaDataStruct.offsetOfNextDataRecordMetaData;
        record->offsetOfPreviousDataRecordMetaData=dataRecordMetaDataStruct.offsetOfPreviousDataRecordMetaData;
        record->offsetOfCollection=dataRecordMetaDataStruct.offsetOfCollection;


//        std::cout<<"At offset "<<offset<<" with size of "<<sizeof(class DataRecordMeataData)<<",this record meta data was read from file "<<record->toString()<<std::endl;







//        assert(record->offsetOfDataRecord!=-1);
//        assert(record->offsetOfDataRecordMetaData!= -1);

        fclose(ptr_myfile);

    }
    std::string FileReader::readTheValueOfDataRecord(offsetType offset,offsetType sizeOfValueFieldInDataRecord ) {
        FILE *ptr_myfile;
        ptr_myfile=fopen(filename.c_str(),"rb");
        if (!ptr_myfile)
        {
            printf("Unable to open file!");

        }
        assert(ptr_myfile);
        fseek ( ptr_myfile , offset , SEEK_SET );
        char valueReadFromFile [sizeOfValueFieldInDataRecord];

        for(int i=0;i<sizeOfValueFieldInDataRecord;i++)
        {
            valueReadFromFile[sizeOfValueFieldInDataRecord]='\0';
        }
        fread(valueReadFromFile,sizeOfValueFieldInDataRecord,1,ptr_myfile);

        std::cout<<"valueReadFromFile array : "<<std::endl;
        std::string valueReadFromFileStr;
        valueReadFromFileStr.reserve(1000);
        for(int i=0;i<sizeOfValueFieldInDataRecord;i++) {
            valueReadFromFileStr.push_back(valueReadFromFile[i]);
            std::cout<<valueReadFromFile[i];

        }

        std::cout<<"\nAt offset "<<offset<<" with size of "<<sizeOfValueFieldInDataRecord<<",this value was read from file "<<valueReadFromFile<<std::endl;

        assert(!valueReadFromFileStr.empty());
        fclose(ptr_myfile);

        return valueReadFromFileStr;

    }


    }

#endif
