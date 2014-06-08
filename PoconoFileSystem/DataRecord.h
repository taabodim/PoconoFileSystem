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
namespace PoconoFileSystem {
    class DataRecord {
        private :
        const static size_t MAX_KEY_SIZE = 32;
        const static size_t MAX_VALUE_SIZE = 32;
        
        public :
        
        char  key[MAX_KEY_SIZE];
        char  value[MAX_VALUE_SIZE];
        offsetType  offsetOfCollection; //this points to the collection this
        //data record belongs to.
        offsetType offsetOfNextDataRecord;//this points to the next data record
        offsetType offsetOfPreviousDataRecord;//this points to the prev data record
        offsetType offsetOfDataRecord;//this points to the offset of this data record
        bool dataRecordRemovedFlag;
        
        
        DataRecord()
        {//for reading the data into array, init the arrays later
            offsetOfNextDataRecord = -1;
            offsetOfPreviousDataRecord = -1;
            offsetOfDataRecord = -1;
        }
        DataRecord(std::string keyStr,std::string valueStr)
        {
            const char* keyPtr = keyStr.c_str();
            const char* valuePtr = valueStr.c_str();
            
            for(int i=0;i<keyStr.size();i++)
                
            {
                key[i] = *keyPtr;
                ++keyPtr;
            }
            for(size_t i=keyStr.size();i<MAX_KEY_SIZE;i++)
            {
                key[i] = '\0';
            }
            
            for(int i=0;i<valueStr.size();i++)
                
            {
                value[i] = *valuePtr;
                ++valuePtr;
            }
            for(size_t i=valueStr.size();i<MAX_VALUE_SIZE;i++)
            {
                value[i] = '\0';
            }
            
            offsetOfNextDataRecord = -1;
            offsetOfPreviousDataRecord = -1;
            offsetOfDataRecord = -1;
        
        }
        std::string toString()
        {
            std::string recordStr(key);
            recordStr.append(";");
            recordStr.append(value);
            recordStr.append("offsetOfNextDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfNextDataRecord));
            
            recordStr.append("offsetOfPreviousDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfPreviousDataRecord));
            
            recordStr.append("offsetOfDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfDataRecord));
            
            return recordStr;
            //recordStr
        }
    };
    
    typedef std::shared_ptr<DataRecord> DataRecordPtr;
    
}

#endif
