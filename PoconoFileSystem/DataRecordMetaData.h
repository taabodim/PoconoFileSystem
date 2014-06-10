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
    class DataRecordMeataData {
 	

 		private: 

 		
 		public : 
 		offsetType offsetOfDataRecord; //this is the offset of data record that this meta data points to
 		offsetType sizeOfValueFieldInDataRecord; //this is the value that 
 		//is passed to the constructor of the DataRecord class to 
 		//create the right size DataRecord object and read all the info 
 		offsetType offsetOfDataRecordMetaData;//this is where this class sits in filesystem
 		offsetType offsetOfNextDataRecordMetaData;
        offsetType offsetOfPreviousDataRecordMetaData;
        offsetType offsetOfCollection;
        DataRecordMeataData()
 		{
 			offsetOfDataRecordMetaData = -1;
 			offsetOfDataRecord = -1;
 			sizeOfValueFieldInDataRecord = -1;
 			offsetOfCollection = -1;

 		}
 	};

 	typedef std::shared_ptr<DataRecordMeataData> DataRecordMetaDataPtr;
}

#endif
