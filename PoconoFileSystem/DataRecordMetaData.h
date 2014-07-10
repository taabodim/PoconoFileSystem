//
//  DataRecordMetaData.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/9/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_DataRecordMetaData_h
#define PoconoDB_DataRecordMetaData_h
#include "Utils.h"

namespace PoconoDB {
    
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
        DataRecordMeataData(const DataRecordMeataData& from) {
            this->offsetOfDataRecordMetaData= from.offsetOfDataRecordMetaData;
 			this->offsetOfDataRecord= from.offsetOfDataRecord;
 			this->offsetOfCollection=from.offsetOfCollection;
            this->offsetOfNextDataRecordMetaData=from.offsetOfNextDataRecordMetaData;
            this->offsetOfPreviousDataRecordMetaData=from.offsetOfPreviousDataRecordMetaData;
            this->offsetOfValueOfRecordInFile=from.offsetOfValueOfRecordInFile;
            this->lengthOfValueField=from.lengthOfValueField;
            this->isDataRecordMetaDeleted= from.isDataRecordMetaDeleted;
            
        }
        DataRecordMeataData operator=(const DataRecordMeataData& from){
            this->offsetOfDataRecordMetaData= from.offsetOfDataRecordMetaData;
 			this->offsetOfDataRecord= from.offsetOfDataRecord;
 			this->offsetOfCollection=from.offsetOfCollection;
            this->offsetOfNextDataRecordMetaData=from.offsetOfNextDataRecordMetaData;
            this->offsetOfPreviousDataRecordMetaData=from.offsetOfPreviousDataRecordMetaData;
            this->offsetOfValueOfRecordInFile=from.offsetOfValueOfRecordInFile;
            this->lengthOfValueField=from.lengthOfValueField;
            this->isDataRecordMetaDeleted= from.isDataRecordMetaDeleted;
            return *this;
        }
 		 std::string toString()
        {
            std::string recordStr;
//            recordStr.reserve(1000);
            recordStr.append("RecordMetaData is {");

            recordStr.append("offsetOfDataRecord : ");
            recordStr.append(PoconoDB::toStr(offsetOfDataRecord));
            

            recordStr.append("offsetOfDataRecordMetaData : ");
            recordStr.append(PoconoDB::toStr(offsetOfDataRecordMetaData));
            

            recordStr.append("offsetOfNextDataRecordMetaData : ");
            recordStr.append(PoconoDB::toStr(offsetOfNextDataRecordMetaData));
            
            recordStr.append("offsetOfPreviousDataRecordMetaData : ");
            recordStr.append(PoconoDB::toStr(offsetOfPreviousDataRecordMetaData));
            
            recordStr.append(" ,offsetOfDataRecord : ");
            recordStr.append(PoconoDB::toStr(offsetOfDataRecord));
            
           
           recordStr.append(" ,offsetOfCollection : ");
           recordStr.append(PoconoDB::toStr(offsetOfCollection));
            
            recordStr.append(" ,offsetOfValueOfRecordInFile : ");
            recordStr.append(PoconoDB::toStr(offsetOfValueOfRecordInFile));
            
            recordStr.append(" ,lengthOfValueField : ");
            recordStr.append(PoconoDB::toStr(lengthOfValueField));
            
            recordStr.append(" ,isDataRecordMetaDeleted : ");
            recordStr.append(PoconoDB::toStr(isDataRecordMetaDeleted));
            
            
           recordStr.append("}");
           
            return recordStr;
        }
        
 	virtual ~DataRecordMeataData()
        {
           // std::cout<<"~DataRecordMeataData object's out"<<std::endl;
        }
    };

// 	typedef std::shared_ptr<DataRecordMeataData> DataRecordMetaDataPtr;
    typedef DataRecordMeataData* DataRecordMetaDataPtr;
    static DataRecordMetaDataPtr getARecordMetaDataOnHeap() { 
        //later count how many objects are created
        DataRecordMetaDataPtr ptr = new DataRecordMeataData();
        return ptr; 
    }
}

#endif
