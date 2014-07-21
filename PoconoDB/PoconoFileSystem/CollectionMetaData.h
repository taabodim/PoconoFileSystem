//
//  CollectionMetaData.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/14/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_CollectionMetaData_h
#define PoconoDB_CollectionMetaData_h

#include "Utils.h"
#include <memory>
#include <iostream>
namespace PoconoDB {
    class CollectionMetaData;
    typedef std::shared_ptr<CollectionMetaData> CollectionMetaDataPtr;

    typedef CollectionMetaData* CollectionMetaDataPtr;

    struct CollectionMetaDataStruct{
        char nameOfCollection [32];
        offsetType offsetOfLastDataRecordMetaData;//this points to the last data
        offsetType offsetOfCollectionMetaDataInFile;//needed
        offsetType offsetOfFirstDataRecordMetaData;
        offsetType isCollectionDeleted;
        CollectionMetaDataStruct(){
            for(int i=0;i<32;i++)
            {
                nameOfCollection[i]='\0';

            }
            offsetOfLastDataRecordMetaData=-1;
            offsetOfCollectionMetaDataInFile=-1;
            offsetOfFirstDataRecordMetaData=-1;
            isCollectionDeleted=-1;

        }
    };

    class CollectionMetaData : std::enable_shared_from_this<CollectionMetaData>{


        private :

        const static int LIMIT_OF_COLLECTION_SIZE = 128;
        public :

        offsetType offsetOfFirstDataRecordMetaData;//this points to the first data record meta data of the collection
        offsetType offsetOfCollectionMetaDataInFile;//needed
        offsetType isCollectionDeleted;
        offsetType offsetOfLastDataRecordMetaData;//this points to the last data record meta data of the collection
        char nameOfCollection [32];

        CollectionMetaData() //for reading the CollectionMetaData
        //info from file
        {
//            for(int i=0;i<LIMIT_OF_COLLECTION_SIZE-1;i++)
//
//            {
//                nameOfCollection [i] = '\0';//memory bug in this for loop
//            }
            this->offsetOfFirstDataRecordMetaData= -1;
            this->offsetOfLastDataRecordMetaData = -1;
            this->isCollectionDeleted = -1;
        }
        CollectionMetaData(std::string name)

        {

            const char* namePtr = name.c_str();

            for(int i=0;i<name.size()-1;i++)

            {
                nameOfCollection [i] = *(namePtr);
                ++namePtr;
            }
            for(size_t i=name.size();i<LIMIT_OF_COLLECTION_SIZE-1;i++)

            {
               // nameOfCollection [i] = '\0';memory bug in this loop
            }

            this->offsetOfFirstDataRecordMetaData= -1;
            this->offsetOfLastDataRecordMetaData = -1;
            this->isCollectionDeleted = -1;

        }
        std::string  getNameOfCollectionAsString()
        {
            std::string  nameOfCollectionStr;

            for(int i=0;i<32;i++)
            {
                if(nameOfCollection [i]!='\0')
                {
                    nameOfCollectionStr.push_back(nameOfCollection [i]);
                }
            }
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
            std::string recordStr;
            recordStr.append(getNameOfCollectionAsString());
            recordStr.append(";");
            recordStr.append("offsetOfFirstDataRecord : ");
            recordStr.append(PoconoDB::toStr(offsetOfFirstDataRecordMetaData));

            recordStr.append("offsetOfLastDataRecord : ");
            recordStr.append(PoconoDB::toStr(offsetOfLastDataRecordMetaData));


            recordStr.append("isCollectionDeleted : ");
            recordStr.append(PoconoDB::toStr(isCollectionDeleted));

            recordStr.append("offsetOfCollectionMetaDataInFile : ");
            recordStr.append(PoconoDB::toStr(offsetOfCollectionMetaDataInFile));


            return recordStr;
         }
        std::shared_ptr<CollectionMetaData> getSharedPtrFromThis()
        {
            return shared_from_this();
        }
        CollectionMetaDataPtr getNewInstanceOnHeap(std::string nameOfCollection)
        {
            return new CollectionMetaData(nameOfCollection);
        }
        virtual  ~CollectionMetaData() {

        }
        bool operator == (CollectionMetaData& other) {
                for(int i=0;i<LIMIT_OF_COLLECTION_SIZE-1;i++)
            {
               if(this->nameOfCollection [i] != other.nameOfCollection[i]) return false;
           }
               if(this->offsetOfFirstDataRecordMetaData [i] != other.offsetOfFirstDataRecordMetaData[i]) return false;
               if(this->offsetOfLastDataRecordMetaData [i] != other.offsetOfLastDataRecordMetaData[i]) return false;
               if(this->isCollectionDeleted [i] != other.isCollectionDeleted[i]) return false;
           
            return true;
        }
    };


}


#endif
