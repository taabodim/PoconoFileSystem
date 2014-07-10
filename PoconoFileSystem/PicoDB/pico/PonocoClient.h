//
//  PonocoClient.h
//  PicoDB
//
//  Created by Mahmoud Taabodi on 4/30/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PicoDB_PonocoClient_h
#define PicoDB_PonocoClient_h
#include "pico_client.h"
#include "pico_test.h"
#include "pico_logger_wrapper.h"
namespace PoconoDB {

class PonocoClient: public pico_logger_wrapper {
private:
	static PonocoDriver* driverPtr;
public:

	PonocoClient(PonocoDriver* driverPtrArg) {
		driverPtr = driverPtrArg;

	}
	void setPonocoDriver(PonocoDriver* driverPtrArg)
	{

		if(driverPtr==NULL)
		{
			driverPtr = driverPtrArg;
		}
	}

	void updateOneBigData() {
		std::string key(pico_test::smallKey0);

		for (int i = 0; i < 1; i++) {

			driverPtr->insert(key, pico_test::bigValue0);
			std::shared_ptr<pico_message> message = driverPtr->update(key,
					pico_test::smallValue0);

		}
	}
	void deleteOneBigData() {
		std::string key(pico_test::smallKey0);
		for (int i = 0; i < 1; i++) {

			driverPtr->insert(key, pico_test::bigValue0);
			std::shared_ptr<pico_message> message = driverPtr->deleteRecord(
					key);

		}
	}
	void getOneBigData() {
		std::string key(pico_test::smallKey0);
		for (int i = 0; i < 1; i++) {

			driverPtr->insert(key, pico_test::bigValue0);
			std::shared_ptr<pico_message> message = driverPtr->get(key);

		}
	}
	void insertOneBigData() {
		std::string key(pico_test::smallKey0);
		for (int i = 0; i < 1; i++) {
			driverPtr->insert(key, pico_test::bigValue0);

		}

	}
	void insertOneBigRandomData() {
		std::string key(pico_test::smallKey0);
		for (int i = 0; i < 1000; i++) {
			string randomSmallKey = convertToString(i).append(random_string(key, 10).append(
					random_string(key, 10)));
//			if (mylogger.isTraceEnabled()) {
//				mylogger
//						<< "driverPtr->insert(randomSmallKey,pico_test::bigValue0 );";
//			}
//			driverPtr->insert(randomSmallKey, pico_test::bigValue0);
			driverPtr->insert(randomSmallKey, randomSmallKey);

		}

	}
	void deleteAndCreateCollectionTest(string collectionName) {
		driverPtr->deleteCollection(collectionName);
		driverPtr->createCollection(collectionName);

	}

	void write1000smallRandomDataUsing100Threads() {
		int numOfThreads = 1;
		PicoConfig::defaultTimeoutInSec = 10;
		vector<boost::thread*> allThreads;
		//sleepViaBoost(2); this throws exception
		for (int i = 0; i < numOfThreads; i++) {
			try {
//                boost::thread poncoDriverThread(
//					boost::bind(&PonocoClient::insertOneBigRandomData,this));

				boost::thread* poncoDriverThread(
						new boost::thread(
								boost::bind(
										&PonocoClient::insertOneBigRandomData,
										this)));
				poncoDriverThread->detach();
				allThreads.push_back(poncoDriverThread);

			} catch (std::exception& e) {
				std::cout << " Exception in threads " << e.what() << "\n";
			}
		}

//        for (int i = 0; i < numOfThreads; i++) {
//            try{
//                allThreads[i]->join();
//
//            }catch(std::exception& e)
//            {
//                std::cout<<" Exception in threads "<<e.what()<<"\n";
//            }
//		}

	}
//        void write1000smallRandomData()
//        {
//            
//            
//            for(int  i=0;i<1000;i++)
//                insert(random_string(20),random_string(20));
//            
//        }
//        void writeOneDeleteOne()
//        {
//            for(int  i=0;i<1;i++)
//                insert(pico_test::smallKey0,pico_test::smallValue0);
//            for(int  i=0;i<1;i++)
//                deleteTest(pico_test::smallKey0,pico_test::smallValue0);
//            
//        }
//        void writeThe_same_record_to_check_if_we_update_or_insert_multiple()
//        {
//            for(int  i=0;i<2;i++)
//            {
//                insert(pico_test::smallKey0,pico_test::smallValue0);
//            }
//        }
//        
//        void writeTenKEY0KEY1KEY2DeleteAllKEY2()
//        {
//            for(int  i=0;i<1;i++)
//            {
//                
//                insert(pico_test::smallKey0,pico_test::smallValue0);
//                insert(pico_test::smallKey1,pico_test::smallValue1);
//                insert(pico_test::smallKey2,pico_test::smallValue2);
//                insert(pico_test::smallKey2,pico_test::smallValue2);
//                insert(pico_test::smallKey2,pico_test::smallValue2);
//                insert(pico_test::smallKey0,pico_test::smallValue0);
//                insert(pico_test::smallKey1,pico_test::smallValue1);
//            }
//            for(int  i=0;i<1;i++)
//                deleteTest(pico_test::smallKey2,pico_test::smallKey2);
//            //
//        }
//        void write1000SmallKeysBigValues_and_deleteAll()
//        {
//            for(int  i=0;i<1000;i++)
//            {
//                std::string key(pico_test::smallKey0);
//                key.append(convertToString(i));
//                insert(key,pico_test::bigValue0);
//            }
//            
//            //            for(int  i=0;i<1000;i++)
//            //            {
//            //                std::string key(pico_test::smallKey0);
//            //                key.append(convertToString(i));
//            //
//            //                deleteTest(key,pico_test::smallKey2);
//            //            }
//            
//            
//        }
//        
//        void write1000SmallKeysValues_and_deleteAll()
//        {
//            for(int  i=0;i<1000;i++)
//            {
//                std::string key(pico_test::smallKey0);
//                key.append(convertToString(i));
//                insert(key,pico_test::smallValue0);
//            }
//            
//            for(int  i=0;i<1000;i++)
//            {
//                std::string key(pico_test::smallKey0);
//                key.append(convertToString(i));
//                
//                deleteTest(key,pico_test::smallKey2);
//            }
//            
//            
//        }

	void currentTestCase() {

		steady_clock::time_point t1 = steady_clock::now();

		string col("currencyCollection");
		PicoConfig::defaultTimeoutInSec = 1;
		//deleteAndCreateCollectionTest(col);

		assert(driverPtr!=NULL);
		write1000smallRandomDataUsing100Threads();

		//            writeOneDeleteOne();
//            insertOneBigData();
//getOneBigData();
		//         deleteOneBigData();
		//    updateOneBigData();
		//write1000SmallKeysBigValues_and_deleteAll();

		steady_clock::time_point t2 = steady_clock::now();

		duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
        
		mylogger << "****************************************\n";
		mylogger << "\nIt took me " << time_span.count() << " seconds.\n";
		std::cout << "****************************************\n";
		std::cout << "\nIt took me " << time_span.count() << " seconds.\n";
      //  sleepViaBoost(100);
	}

};

}

#endif
