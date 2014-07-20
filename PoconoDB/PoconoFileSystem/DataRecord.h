//
//  DataRecord.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_DataRecord_h
#define PoconoDB_DataRecord_h
#include "Utils.h"
#include <string.h>
#include <list>

namespace PoconoDB {
    const static size_t MAX_KEY_SIZE = 32;
    
    struct DataRecordStruct{
        offsetType  offsetOfCollection;
        offsetType offsetOfValueOfRecordInFile; //this points to where the the value field of
        bool dataRecordRemovedFlag;//this flag is set to true when data is deleted;
        offsetType sizeOfValueFieldInDataRecord;
        offsetType offsetOfDataRecordMetaData; //this is the offset of metadata record that points to this guy , we need it for update functionality
        offsetType offsetOfDataRecord;//this points to where this datarecord sits in file,this is needed for update functionality
        char  key [MAX_KEY_SIZE];
        DataRecordStruct()
        {
//            for(size_t i=0;i<MAX_KEY_SIZE;i++)
//            {
//                key[i] = '\0';
//            }
//            sizeOfValueFieldInDataRecord = -1;
//            
//            dataRecordRemovedFlag = false;
//            offsetOfValueOfRecordInFile = -1;
//            offsetOfCollection = -1;
//            offsetOfDataRecordMetaData =-1;
//            offsetOfDataRecord = -1;
        }
    };
    class DataRecord {
        private :
        // const static size_t MAX_VALUE_SIZE = 32;
        
        public :
        
       
        offsetType offsetOfCollection; //this points to the collection this data record belongs to.
        offsetType offsetOfValueOfRecordInFile; //this points to where the the value field of record is .
        //we need this , because the value part of the data is variable sized.
        bool dataRecordRemovedFlag;//this flag is set to true when data is deleted;
        offsetType sizeOfValueFieldInDataRecord;
        offsetType offsetOfDataRecordMetaData; //this is the offset of metadata record that points to this guy , we need it for update functionality
        offsetType offsetOfDataRecord;//this points to where this datarecord sits in file,this is needed for update functionality
        char  key [MAX_KEY_SIZE];
        char*  value;
        
        DataRecord() {
            //for reading the data into array, init the arrays later
            // offsetOfNextDataRecordMetaData = -1;
            // offsetOfPreviousDataRecordMetaData = -1;
           // offsetOfDataRecord = -1;
            //sizeOfValueFieldInDataRecord = -1;
//            offsetOfCollection = -1;
//            offsetOfDataRecord =-1;
//            
//            for(int i=0;i<MAX_KEY_SIZE;i++)
//                
//            {
//                key[i] = '\0';
//                
//            }
        }
        DataRecord(std::string keyStr,std::string valueStr)
        {
            const char* keyPtr = keyStr.c_str();
           
            for(int i=0;i<keyStr.size();i++)
                
            {
                key[i] = *keyPtr;
                ++keyPtr;
            }
            for(size_t i=keyStr.size();i<MAX_KEY_SIZE;i++)
            {
                key[i] = '\0';
            }
            
            setValue(valueStr,valueStr.size()+1);
            //offsetOfDataRecord = -1;
            offsetOfCollection = -1;
            offsetOfDataRecord =-1;
            
           // assert(offsetOfDataRecord==-1);

        }
        void setValue(std::string valueStr,offsetType length)
        {
            const char* valuePtr = valueStr.c_str();
            value = new char[length];
            for(int i=0;i<length;i++)
                
            {
                value[i] = *valuePtr;
                ++valuePtr;
            }
            value[length]='\0';
            sizeOfValueFieldInDataRecord = length;
            assert(sizeOfValueFieldInDataRecord== length);

        }
        bool keyIsEqualTo(std::string keyGiven)
        {
           if(this->getKeyAsString().compare(keyGiven)==0)
               return true;
            
            return false;
        }
        std::string keyStr;
        std::string getKeyAsString()
        {
        
            if(keyStr.empty())
            {
            char keyArrayCopy[MAX_KEY_SIZE];
            
            memcpy ( keyArrayCopy, key, MAX_KEY_SIZE );
            std::string keyStrNew (key);
            keyStr.append(keyStrNew);
            //std::cout<<"keyStr : "<<keyStr<<std::endl;
            }
            return keyStr;
        }
        
        
        std::string getValueAsString()
        {
            char valueArrayCopy[sizeOfValueFieldInDataRecord];
            
            memcpy ( valueArrayCopy, value, sizeOfValueFieldInDataRecord );
            std::string val (valueArrayCopy,sizeOfValueFieldInDataRecord);
            //std::cout<<"getValueAsString : "<<val<<std::endl;
            assert(value!=NULL);
            return val;

        }
        std::string toString()
        {
            std::string recordStr;
            recordStr.reserve(1000);
            recordStr.append(key);
            recordStr.append(";");
            if(value==NULL)
            {
                recordStr.append("NULL");
            }
            else
            {
                recordStr.append(value);
            }
//            recordStr.append("offsetOfNextDataRecordMetaData : ");
//            recordStr.append(PoconoDB::toStr(offsetOfNextDataRecordMetaData));
//            
//            recordStr.append("offsetOfPreviousDataRecordMetaData : ");
//            recordStr.append(PoconoDB::toStr(offsetOfPreviousDataRecordMetaData));
            
           // recordStr.append(" ,offsetOfDataRecord : ");
           // recordStr.append(PoconoDB::toStr(offsetOfDataRecord));
            
            recordStr.append(" ,sizeOfValueFieldInDataRecord : ");
            recordStr.append(PoconoDB::toStr(sizeOfValueFieldInDataRecord));
           
            recordStr.append(" ,offsetOfValueOfRecordInFile : ");
            recordStr.append(PoconoDB::toStr(offsetOfValueOfRecordInFile));
//           
           

            return recordStr;
        }
    };
   
    typedef std::shared_ptr<DataRecord> DataRecordPtr;
//    typedef DataRecord* DataRecordPtr;
//    typedef std::shared_ptr<std::list<DataRecord>> ListOfDataRecordPtr;
    typedef std::list<DataRecord> ListOfDataRecordPtr;
    
    DataRecordPtr getARecordDataOnHeap() {
        //later count how many objects are created
        std::shared_ptr<DataRecord> ptr(new DataRecord());
//        DataRecordPtr ptr = new DataRecord();
        return ptr;
    }
    
     ListOfDataRecordPtr getAListOfDataRecordOnHeap() {
        std::list<DataRecord> allData;
         
        return allData;
   }
    std::list<DataRecordPtr> getAListOfDataRecordOnHeapModified() {
        //        std::shared_ptr<std::list<DataRecordPtr>> allData(new std::list<DataRecordPtr>());
        
        std::list<DataRecordPtr> allData;
        
        return allData;
    }

    
    
}

#endif
