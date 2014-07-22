//
//  CRUDTests.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/19/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_CRUDTests_h
#define PoconoDB_CRUDTests_h
namespace PoconoDB {
    std::string smallKey="smallKey";
    std::string smallValue="smallValue";
    std::string bigValue0="bigValue0**********************THISISABIGVALUE**THISISABIGVALUE**THISISABIGVALUE**THISISABIGVALUE*******THISISABIGVALUE*****************THISISABIGVALUE***********************************EhdOfValue";
    void testWritingAndReadingSmallValuesInOneCollection()
    {
        FileSystem testFS;
        CollectionMetaDataPtr collection =testFS.openCollection("testCollection");

        int num = 10;
        for(int i=0;i<num;i++)
        {
            DataRecordPtr record(new DataRecord(smallKey,smallValue));
            testFS.insertData(collection, record);
        }
            DataRecordPtr foundRecord = testFS.find("testCollection", smallKey);
           // assert(foundRecord==record);
            assert(foundRecord->getKeyAsString().compare(smallKey)==0);
            assert(foundRecord->getValueAsString().compare(smallValue)==0);


//        ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
//        testFS.getAllData(collection);
//        for(std::list<DataRecord>::iterator it = allData.begin();
//            it!=allData.end();++it)
//        {
//            std::cout<<"Data Record is "<<(it)->toString()<<std::endl;
//        }
    }
    void testWritingAndReadingOneSmallValueInOneCollection() {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");
        CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);


        for(int i=0;i<1;i++)
        {
            DataRecordPtr record(new DataRecord(smallKey,smallValue));
            testFS.insertData(collection, record);
//            DataRecordPtr foundRecord = testFS.find("testCollection", smallKey);
//            assert(foundRecord==record);
//            assert(foundRecord->getKeyAsString().compare(smallKey)==0);
//            assert(foundRecord->getValueAsString().compare(smallValue)==0);

        }

        ListOfDataRecordPtr allData  = getAListOfDataRecordOnHeap();
        testFS.getAllData(collection);
        for(std::list<DataRecordPtr>::iterator it = allData.begin();
            it!=allData.end();++it)
        {
            std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
        }

    }
    void testWritingAndReadingSmall1000ValuesInThreeCollections() {


        FileSystem testFS;
        int numOfCollectionsInTest =3;

        std::string colName1 ("testCollection1");
        std::string colName2 ("testCollection2");
        std::string colName3 ("testCollection3");


        CollectionMetaDataPtr collection1 =testFS.openCollection(colName1);
        CollectionMetaDataPtr collection2 =testFS.openCollection(colName2);
        CollectionMetaDataPtr collection3 =testFS.openCollection(colName3);
        CollectionMetaDataPtr allCollections [3]={collection1,collection2,collection3};

        int num = 100;
        for(int j=0;j<3;j++)
        {
            for(int i=0;i<num;i++)
            {
                std::string key("smallKey");
                key.append(toStr(i));
                key.append(allCollections[j]->getNameOfCollectionAsString());

                std::string value("");
                value.append(toStr(i));
                value.append(allCollections[j]->getNameOfCollectionAsString());

                DataRecordPtr record(new DataRecord(key,value));
                testFS.insertData(allCollections[j], record);
//                DataRecordPtr foundRecord = testFS.find(allCollections[j]->getNameOfCollectionAsString(), "smallKey");
//                assert(foundRecord==record);
//                assert(foundRecord->getKeyAsString().compare(record->getKeyAsString())==0);
//                assert(foundRecord->getValueAsString().compare(record->getValueAsString())==0);
            }
        }

        for(int j=0;j<3;j++)
        {
            std::cout<<"data in the next collections *************"<<std::endl;

            ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
            allData = testFS.getAllData(allCollections[j]);
            for(std::list<DataRecordPtr>::iterator it = allData.begin();
                it!=allData.end();++it)
            {
                std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
            }

        }

    }
    void testWritingAndReadingSmallValuesInThreeCollections()
    {
        FileSystem testFS;
        int numOfCollectionsInTest =3;

        //    for(int col = 0 ; col<numOfCollectionsInTest;col++)
        //    {
        std::string colName1 ("testCollection1");
        std::string colName2 ("testCollection2");
        std::string colName3 ("testCollection3");


        CollectionMetaDataPtr collection1 =testFS.openCollection(colName1);
        CollectionMetaDataPtr collection2 =testFS.openCollection(colName2);
        CollectionMetaDataPtr collection3 =testFS.openCollection(colName3);
        CollectionMetaDataPtr allCollections [3]={collection1,collection2,collection3};

        int num = 10;
        for(int j=0;j<3;j++)
        {
            for(int i=0;i<num;i++)
            {
                std::string key(smallKey);
                key.append(toStr(i));
                key.append(allCollections[j]->getNameOfCollectionAsString());

                std::string value(bigValue0);
                value.append(toStr(i));
                value.append(allCollections[j]->getNameOfCollectionAsString());

                DataRecordPtr record(new DataRecord(key,value));
                testFS.insertData(allCollections[j], record);
            }
        }

        for(int j=0;j<3;j++)
        {
            std::cout<<"data in the next collections *************"<<std::endl;

            ListOfDataRecordPtr allData = getAListOfDataRecordOnHeap();
            allData = testFS.getAllData(allCollections[j]);
            for(std::list<DataRecordPtr>::iterator it = allData.begin();
                it!=allData.end();++it)
            {
                std::cout<<"Data Record is "<<(*it)->toString()<<std::endl;
            }

        }

        //    }
    }

    void testWritingTenDataRecordAndFindingOne() {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");
        CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);


        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
            testFS.insertData(collection, record);
        }
        //find the one with key : smallKey8
        std::string key("smallKey8");
        DataRecordPtr foundRecord = testFS.find(nameOfCollection,key);
        assert(foundRecord!=NULL);
        assert(foundRecord->getKeyAsString().compare(key)==0);
        std::cout<<"foundRecord Data Record is "<<(foundRecord)->toString()<<std::endl;


    }
    void testDeletingFourCollections()
    {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");
        std::string nameOfCollection1("testCollection1");
        std::string nameOfCollection2("testCollection2");
        std::string nameOfCollection3("testCollection3");

        testFS.openCollection(nameOfCollection);
        testFS.openCollection(nameOfCollection1);
        testFS.openCollection(nameOfCollection2);
        testFS.openCollection(nameOfCollection3);

        testFS.deleteCollection(nameOfCollection);
        testFS.deleteCollection(nameOfCollection1);
        testFS.deleteCollection(nameOfCollection2);
        testFS.deleteCollection(nameOfCollection3);

        list<std::string> allCollections= testFS.showAllCollections();
        for(list<std::string>::iterator iter=allCollections.begin();
            iter!=allCollections.end();++iter)
        {
            std::cout<<"collection name is : "<<*iter<<std::endl;
        }

        assert(allCollections.size()==0);

    }

    //incomplete , finish this test , debug this test and make sure it works
    void testWriting10DataRecordAndDeletingTheOneWithSmallKey8() {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");

        testFS.deleteCollection(nameOfCollection);

        CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);


        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
            testFS.insertData(collection, record);
        }
        //find the one with key : smallKey8
        std::string key("smallKey8");
        std::string response = testFS.deleteData(nameOfCollection,key);
        string respExpected("record was deleted with this key : ");
        respExpected.append(key);
        assert(respExpected.compare(response)==0);
        // assert(foundRecord->getKeyAsString().compare(key)==0);
        std::cout<<"response for deleting the Data Record is "<<response<<std::endl;


    }

    //incomplete , finish this test , debug this test and make sure it works
    void testWriting10DataRecordAndDeletingAllOfThem() {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");

        testFS.deleteCollection(nameOfCollection);

        CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);


        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
            testFS.insertData(collection, record);
        }

        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            std::string updateValue("smallValue***************************************************************************************EhdOfValue");
            updateValue.append(toStr(i));

            testFS.deleteData(nameOfCollection, key);
            DataRecordPtr foundRecord = testFS.find(nameOfCollection,key);
            assert(foundRecord==NULL);

        }

    }

    //incomplete , finish this test , debug this test and make sure it works
    void testWriting10DataRecordAndUpdatingAllOfThem() {
        FileSystem testFS;
        std::string nameOfCollection("testCollection");

        testFS.deleteCollection(nameOfCollection);

        CollectionMetaDataPtr collection =testFS.openCollection(nameOfCollection);


        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            DataRecordPtr record(new DataRecord(key,"smallValue***************************************************************************************EhdOfValue"));
            testFS.insertData(collection, record);
        }


        for(int i=0;i<10;i++)
        {
            std::string key("smallKey");
            key.append(toStr(i));

            std::string updateValue("smallValue***************************************************************************************EhdOfValue");
            updateValue.append(toStr(i));

            testFS.updateData(nameOfCollection, key, updateValue);
            DataRecordPtr foundRecord = testFS.find(nameOfCollection,key);
            assert(foundRecord!=NULL);
            assert(foundRecord->getValueAsString().compare(updateValue)==0);

        }



    }
    void allOfTests() {

       // testWritingAndReadingSmallValuesInOneCollection();//fail
        //testWritingAndReadingSmallValuesInThreeCollections();//pass
        testDeletingFourCollections();
////
//        testWritingTenDataRecordAndFindingOne();
//        testWriting10DataRecordAndDeletingTheOneWithSmallKey8();

        //testWriting10DataRecordAndDeletingAllOfThem(); //its buggy  fix it

        //these tests pass
            testWritingAndReadingOneSmallValueInOneCollection();//stage 1
        testWritingAndReadingSmall1000ValuesInThreeCollections();

    }
}

#endif
