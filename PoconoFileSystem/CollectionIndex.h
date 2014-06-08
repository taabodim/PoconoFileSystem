//
//  CollectionIndex.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_CollectionIndex_h
#define PoconoFileSystem_CollectionIndex_h
#include "Utils.h"
namespace PoconoFileSystem {
    class CollectionMetaData {
        private :
        
        char nameOfCollection [32];
        const static int LIMIT_OF_COLLECTION_SIZE = 128;
        public :
        offsetType offsetOfCollectionMetaDataInFile;
        //size_t offsetOfAllDataRecords[LIMIT_OF_COLLECTION_SIZE];
        offsetType offsetOfFirstDataRecord;//this points to the first data record of the collection
        offsetType offsetOfLastDataRecord;//this points to the last data record of the collection

        CollectionMetaData() //for reading the CollectionMetaData
        //info from file
        {
            this->offsetOfFirstDataRecord=-1;
            this->offsetOfLastDataRecord=-1;
            this->offsetOfCollectionMetaDataInFile=-1;
            
        }
        CollectionMetaData(std::string name)
       
        {
            
            const char* namePtr = name.c_str();
            
            for(int i=0;i<name.size();i++)
                
            {
                nameOfCollection [i] = *(namePtr);
                ++namePtr;
            }
            for(size_t i=name.size();i<LIMIT_OF_COLLECTION_SIZE;i++)
                
            {
                nameOfCollection [i] = '\0';
            }
            
            this->offsetOfFirstDataRecord= -1;
            this->offsetOfLastDataRecord = -1;
            this->offsetOfCollectionMetaDataInFile=-1;
            

        }
        std::string  getNameOfCollectionAsString()
        {
            std::string  nameOfCollectionStr(nameOfCollection);
            assert(nameOfCollectionStr.size()>0);
            return nameOfCollectionStr;
            
        
        }
        void addTheOffsetToListOfDataRecords(size_t offsetOfNewDataRecord)
        {
//            for(int i=0;i<LIMIT_OF_COLLECTION_SIZE;i++)
//            {
//                if(offsetOfAllDataRecords[i]==-1)
//                {
//                    offsetOfAllDataRecords[i] = offsetOfNewDataRecord;
//                    break;
//                }
//            }
        }
        
        std::string toString()
        {
            std::string recordStr(getNameOfCollectionAsString());
            recordStr.append(";");
            recordStr.append("offsetOfFirstDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfFirstDataRecord));
            
            recordStr.append("offsetOfLastDataRecord : ");
            recordStr.append(PoconoFileSystem::toStr(offsetOfLastDataRecord));
            
            return recordStr;
            //recordStr
        }

        
    };
    
    typedef std::shared_ptr<CollectionMetaData> CollectionMetaDataPtr;
    
}

#endif
