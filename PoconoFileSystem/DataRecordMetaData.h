//
//  DataRecordMetaData.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/9/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_DataRecordMetaData_h
#define PoconoFileSystem_DataRecordMetaData_h
#include "Utils.h"

namespace PoconoFileSystem {
    
	/*this class is a fixed size class that 
	contains sits at an offset multiple of BLOCK_SIZE
	and has information about the data record that follows
	such as where that data record starts and the sizeof class
	that contains the data record, the rest of information resides 
	in DataRecord

     
	*/
    
    struct DataRecordMetaDataStruct{
        offsetType offsetOfDataRecord; //this is the offset of data record that this meta data points to
        offsetType offsetOfValueOfRecordInFile;
        offsetType offsetOfDataRecordMetaData;//this is where this class sits in filesystem
 		offsetType offsetOfNextDataRecordMetaData;
        offsetType offsetOfPreviousDataRecordMetaData;
        offsetType offsetOfCollection;
        offsetType lengthOfValueField;
        offsetType isDataRecordMetaDeleted;
        
        DataRecordMetaDataStruct(){
            offsetOfDataRecord = -1;
            offsetOfValueOfRecordInFile = -1;
            offsetOfDataRecordMetaData=-1;
            offsetOfNextDataRecordMetaData=-1;
            offsetOfPreviousDataRecordMetaData=-1;
            offsetOfCollection=-1;
            lengthOfValueField=-1;
            isDataRecordMetaDeleted=-1;
        }
        
        
    };
    class DataRecordMeataData {
 	

 		private: 

 		
 		public : 
 		offsetType offsetOfDataRecord; //this is the offset of data record that this meta data points to
        offsetType offsetOfValueOfRecordInFile;
        offsetType lengthOfValueField;
 		offsetType offsetOfDataRecordMetaData;//this is where this class sits in filesystem
 		offsetType offsetOfNextDataRecordMetaData;
        offsetType offsetOfPreviousDataRecordMetaData;
        offsetType offsetOfCollection;
        offsetType isDataRecordMetaDeleted;
       
        DataRecordMeataData()
 		{
 			offsetOfDataRecordMetaData = -1;
 			offsetOfDataRecord = -1;
 			offsetOfCollection = -1;
            offsetOfNextDataRecordMetaData = -1;
            offsetOfPreviousDataRecordMetaData = -1;
            offsetOfValueOfRecordInFile = -1;
            lengthOfValueField=-1;
            isDataRecordMetaDeleted = -1;
       

 		}
 		 std::string toString()
        {
            std::string recordStr("RecordMetaData is {");
            

            recordStr.append("offsetOfDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfDataRecord));
            

            recordStr.append("offsetOfDataRecordMetaData : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfDataRecordMetaData));
            

            recordStr.append("offsetOfNextDataRecordMetaData : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfNextDataRecordMetaData));
            
            recordStr.append("offsetOfPreviousDataRecordMetaData : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfPreviousDataRecordMetaData));
            
            recordStr.append(" ,offsetOfDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfDataRecord));
            
           
           recordStr.append(" ,offsetOfCollection : ");
           recordStr.append(PoconoFileSystem::toStr(offsetOfCollection));
            
            recordStr.append(" ,offsetOfValueOfRecordInFile : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfValueOfRecordInFile));
            
            recordStr.append(" ,lengthOfValueField : ");
            recordStr.append(PoconoFileSystem::toStr(lengthOfValueField));
            
            recordStr.append(" ,isDataRecordMetaDeleted : ");
            recordStr.append(PoconoFileSystem::toStr(isDataRecordMetaDeleted));
            
            
           recordStr.append("}");
           
            return recordStr;
        }
 	};

// 	typedef std::shared_ptr<DataRecordMeataData> DataRecordMetaDataPtr;
    typedef DataRecordMeataData* DataRecordMetaDataPtr;

}

#endif
