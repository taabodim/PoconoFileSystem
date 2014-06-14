//
//  DataRecord.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_DataRecord_h
#define PoconoFileSystem_DataRecord_h
#include "Utils.h"
#include <string.h>

namespace PoconoFileSystem {
    const static size_t MAX_KEY_SIZE = 32;
    
    struct DataRecordStruct{
        offsetType  offsetOfCollection;
        offsetType offsetOfValueOfRecordInFile; //this points to where the the value field of
        bool dataRecordRemovedFlag;//this flag is set to true when data is deleted;
        offsetType sizeOfValueFieldInDataRecord;
        char  key [MAX_KEY_SIZE];
        DataRecordStruct()
        {
            for(size_t i=0;i<MAX_KEY_SIZE;i++)
            {
                key[i] = '\0';
            }
            sizeOfValueFieldInDataRecord = -1;
            
            dataRecordRemovedFlag = false;
            offsetOfValueOfRecordInFile = -1;
            offsetOfCollection = -1;
        }
    };
    class DataRecord {
        private :
        // const static size_t MAX_VALUE_SIZE = 32;
        
        public :
        
       
        offsetType  offsetOfCollection; //this points to the collection this
        //data record belongs to.
        // offsetType offsetOfNextDataRecordMetaData;//this points to the next DataRecordMetaData that points to the next DataRecord
        // offsetType offsetOfPreviousDataRecordMetaData;//this points to the prev DataRecordMetaData that points to the previous DataRecord
        //offsetType offsetOfDataRecord;//this points to the offset of this data record
        offsetType offsetOfValueOfRecordInFile; //this points to where the the value field of record is .
        //we need this , because the value part of the data is variable sized.
        bool dataRecordRemovedFlag;//this flag is set to true when data is deleted;

        offsetType sizeOfValueFieldInDataRecord;
        char  key [MAX_KEY_SIZE];
        char*  value;
        
        DataRecord()
        {//for reading the data into array, init the arrays later
            // offsetOfNextDataRecordMetaData = -1;
            // offsetOfPreviousDataRecordMetaData = -1;
           // offsetOfDataRecord = -1;
            //sizeOfValueFieldInDataRecord = -1;
            offsetOfCollection = -1;
            
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
            
            setValue(valueStr,valueStr.size());
            //offsetOfDataRecord = -1;
            offsetOfCollection = -1;

            
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
            value[length+1]='\0';
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
            std::cout<<"keyStr : "<<keyStr<<std::endl;
            }
            return keyStr;
        }
        
        
        std::string getValueAsString()
        {
            char valueArrayCopy[sizeOfValueFieldInDataRecord];
            
            memcpy ( valueArrayCopy, value, sizeOfValueFieldInDataRecord );
            std::string val (valueArrayCopy,sizeOfValueFieldInDataRecord);
            std::cout<<"getValueAsString : "<<val<<std::endl;
            assert(value!=NULL);
            return val;

        }
        std::string toString()
        {
            std::string recordStr(key);
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
//            recordStr.append(PoconoFileSystem::toStr(offsetOfNextDataRecordMetaData));
//            
//            recordStr.append("offsetOfPreviousDataRecordMetaData : ");
//            recordStr.append(PoconoFileSystem::toStr(offsetOfPreviousDataRecordMetaData));
            
           // recordStr.append(" ,offsetOfDataRecord : ");
           // recordStr.append(PoconoFileSystem::toStr(offsetOfDataRecord));
            
            recordStr.append(" ,sizeOfValueFieldInDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(sizeOfValueFieldInDataRecord));
           
            recordStr.append(" ,offsetOfValueOfRecordInFile : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfValueOfRecordInFile));
//           
           

            return recordStr;
        }
    };
   
    typedef std::shared_ptr<DataRecord> DataRecordPtr;
    
}

#endif
