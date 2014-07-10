////
////  RecordInserter.h
////  PicoDB
////
////  Created by Mahmoud Taabodi on 5/11/14.
////  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
////
//
//#ifndef PicoDB_RecordInserter_h
//#define PicoDB_RecordInserter_h
//#include "pico_message.h"
//#include "Utils.h"
//#include <collection_manager.h>
//#include "pico_logger_wrapper.h"
//
//namespace PoconoDB {
//
//class RecordInserter: public pico_logger_wrapper {
//private:
//    std::shared_ptr<collection_manager> collectionManager;
//public:
//    RecordInserter()
//    {
//        
//    }
//    void setCollectionManager(std::shared_ptr<collection_manager> collectionManagerArg){
//        collectionManager =collectionManagerArg;
//    }
//	msgPtr insertOneMessage(msgPtr picoMsg, offsetType offset) //insert at a specific offset
//			{
//		
////		std::shared_ptr<pico_collection> optionCollection =
////				collectionManager->getTheCollection(picoMsg->collection);
////
////                pico_buffered_message<pico_record> msg_in_buffers;
////                =
////				picoMsg->getKeyValueOfMessageInRecords();
////                int initialSize = msg_in_buffers.size();
////		pico_record firstrecord = msg_in_buffers.peek();
////
////		offsetType whereToWriteThisRecord = offset;
////		if (collectionManager->getTheCollection(picoMsg->collection)->ifRecordExists(
////				firstrecord)) {
////
////			if (mylogger.isTraceEnabled()) {
////				mylogger
////						<< "request_processr: record already exists,returning error! ";
////			}
//
////			string result("record already exists in database ");
////			msgPtr msg = pico_message::build_message_from_string(result,
////					picoMsg->messageId);
////			return msg;
////
////		}
//
////		int i = 0;
////		do {
////			pico_record record = msg_in_buffers.pop();
////
////			mylogger
////					<< "\nrequest_processor : record that is going to be saved is this : "
////					<< record.toString();
////			i++;
////			optionCollection->overwrite(record, whereToWriteThisRecord);
////			whereToWriteThisRecord += max_database_record_size;
////
////		} while (!msg_in_buffers.empty());
////		assert(i == initialSize);
//		string result("one message was added to database in ");
//    	result.append("1 record");
//
//		msgPtr msg = pico_message::build_message_from_string(result,
//				picoMsg->messageId);
//		return msg;
//
//	}
//
//	msgPtr insertOneMessage(msgPtr picoMsg) {
//
//		std::shared_ptr<pico_collection> optionCollection =
//				collectionManager->getTheCollection(picoMsg->collection);
//
//		std::shared_ptr<DBRecord>  recordTOBeInDB= picoMsg->getDBrecordOfMessage();
//
//
//		if (collectionManager->getTheCollection(picoMsg->collection)->ifRecordExists(
//				recordTOBeInDB)) {
//
//			if (mylogger.isTraceEnabled()) {
//				mylogger
//						<< "request_processr: record already exists,returning error! ";
//			}
//
//			string result("record already exists in database ");
//			msgPtr msg = pico_message::build_message_from_string(result,
//					picoMsg->messageId);
//			return msg;
//
//		}
//
//	    mylogger<< "\nrequest_processor : record that is going to be saved at this offset "<< record.toString();
//        
//        optionCollection->append(recordTOBeInDB); //append the
//		
//		string result("one message was added to database in ");
////		result.append(convertToString(i));
//		result.append(" seperate records");
//
//		msgPtr msg = pico_message::build_message_from_string(result,
//				picoMsg->messageId);
//        
//		return msg;
//
//	}
//};
//}
//
//#endif
