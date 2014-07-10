//
//  ClientServerExample.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 7/10/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_ClientServerExample_h
#define PoconoDB_ClientServerExample_h
#include <cstdlib>
#include <list>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <memory>
//#include <boost/date_time/gregorian/gregorian.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/ptr_container/ptr_list.hpp>
//#include <boost/foreach.hpp>
//#include <array>
//#include <cstring>
//#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <boost/thread.hpp>
#include "PonocoRunnable.h"
#include <memory>
#include <utility>
#include <array>
//#include <boost/archive/text_oarchive.hpp> //these libs are made for 32 bit but my laptop is 64 bit , solve it later
//#include <boost/archive/text_iarchive.hpp>
#include "OffsetManager.h"
#include "ObjectPool.h"
#include "Runnable.h"
#include "ThreadWorker.h"
#include "pico_logger_wrapper.h"
#include <iostream>
#include "PonocoClient.h"
#include <string>
#include <vector>
#include "logger.h"

#include "ThreadPool.h"
#include "pico_test.h"
#include "pico_server.h"
#include "Utils.h"
#include "pico_record.h"
#include "pico_collection.h"
#include "pico_client.h"
#include "pico_index.h"
#include "ConcurrentVector.h"
#include <type_traits>
#include <memory>
#include <cstdio>

namespace PoconoDB {
    
    std::string pico_session::logFileName ("session");
    std::string request_processor::logFileName ("session");
    std::string SimpleRunnable::logFileName ("gicapods");
    //std::string DeleteTaskRunnable::logFileName ("gicapods");
    
    string pico_test::smallKey0 {"smallKey0"};
    string pico_test::smallKey1  ("smallKey1");
    string pico_test::smallKey2  ("smallKey2");
    string pico_test::smallKey3  ("smallKey3");
    
    
    string  pico_test::smallValue0  ("smallValue0");
    string  pico_test::smallValue1 ("smallValue1");
    string  pico_test::smallValue2 ("smallValue2");
    string  pico_test::smallValue3 ("smallValue3");
    
    std::string request_processor::insertCommand("insert");
    std::string request_processor::deleteCommand("delete");

    std::string request_processor::updateCommand("update");
    std::string request_processor::findCommand("find");
    std::string request_processor::getCommand("get");
    std::string request_processor::addUserToDBCommand("adduser");
    std::string request_processor::deleteUserToDBCommand("deleteuser");
    std::string request_processor::deleteCollectionCommand("deleteCollection");
    std::string request_processor::createCollectionCommand("createCollection");
    //std::string PicoConfig::dataDir("/Users/mahmoudtaabodi/Documents/");
    //std::string PicoConfig::logDir("/Users/mahmoudtaabodi/Documents/");
    
    std::string PicoConfig::dataDir("/Users/mtaabodi/Documents/pico_data/");
    std::string PicoConfig::logDir("/Users/mtaabodi/Documents/pico_logs/");
    std::string PicoConfig::logFileName("");
    long PicoConfig::defaultTimeoutInSec(10);
    //std::string pico_record::BEGKEY {"BEGKEY"};
    //std::string pico_record::CONKEY {"CONKEY"};
    std::atomic<offsetType> OffsetManager::offset(-1);
    PonocoDriver* PonocoClient::driverPtr{0};
    string  pico_test::bigValue0("Families skepticalFamilies of the 239 people who were aboard when the plane disappeared from radar screens early March 8 met Friday with Malaysia Airlines and government officials. They came away unpersuaded that progress was being made.Today, all they said was that they were confident, family representative Steve Wang said. But that really doesn't mean that they have confirmed it. They didn't use the word 'confirm.' So it could be that it's a real lead, but it could also not be. I think that, at the moment, everyone needs to wait for final, confirmed information.That view was echoed by Sarah Bajc, whose partner, Philip Wood, was among the passengers.Every time some official gives one of those absolute statements of 'We're sure it's the pings from the black boxes' or 'We're sure it's in the ocean,' we all crash, she told CNNs New Day.Our feet get knocked out from underneath us. But then it always ends up reversing itself, and they step back from it.She expressed skepticism about the way the investigation has been handled. The fox is very much in charge of the henhouse here, she told New Day. We've got a country leading the investigation who also has the primary liability in the case, and it makes us question every step that's taken.\" More cluesA senior Malaysian government official and another source involved in the investigation divulged new details about the flight to CNN on Thursday, including information about what radar detected, the last words from the cockpit and how high the plane was flying after it went off the grid.Malaysia Airlines Flight 370 disappeared from military radar for about 120 nautical miles after it crossed back over the Malay Peninsula, sources said. Based on available data, this means the plane must have dipped in altitude to between 4,000 and 5,000 feet, sources said.The dip could have been programmed into the computers controlling the plane as an emergency maneuver, said aviation expert David Soucie.The real issue here is it looks like -- more and more -- somebody in the cockpit was directing this plane and directing it away from land,said Peter Goelz, a CNN aviation analyst and former National Transportation Safety Board managing director.And it looks as though they were doing it to avoid any kind of detection.But former U.S. Department of Transportation Inspector General Mary Schiavo was not convinced. She said the reported dip could have occurred in response to a loss of pressure, to reach a level where pressurization was not needed and those aboard the plane would have been able to breathe without oxygen, or to get out of the way of commercial traffic123456endOfMessage");
    
    string  pico_test::bigValue1("Families skepticalFamilies of the 239 people who were aboard when the plane disappeared from ;radar screens early March 8 met Friday with Malaysia Airlines and government officials. They came away unpersuaded that progress was being made.Today, all they said was that they were confident, family representative Steve Wang said. But that really doesn't mean that they have confirmed it.endOfMessage");
    
    namespace PoconoServer{
        
        void printStackTraceHandler(int sig) {
            void *array[10];
            size_t size;
            
            // get void*'s for all entries on the stack
            size = backtrace(array, 10);
            
            // print out all the frames to stderr
            fprintf(stderr, "Error: signal %d:\n", sig);
            backtrace_symbols_fd(array, size, STDERR_FILENO);
            
            
            pico_logger_wrapper::myloggerPtr->log("this is the stack trace :\n ");
            //    for(int i=0;i<10;i++)
            //    {
            //    std::string *sp = static_cast<std::string*>(array[i]);
            //    // You could use 'sp' directly, or this, which does a copy.
            //    std::string s = *sp;
            //    // Don't forget to destroy the memory that you've allocated.
            //  //  delete sp;
            //
            //    pico_logger_wrapper::myloggerPtr->log(s);
            //    }
            //realname = abi::__cxa_demangle(e.what(), 0, 0, &status);
            //  std::cout << ti.name() << "\t=> " << realname << "\t: " << status << '\n';
            
            //exit(1);
        }

        void registerPrintStackHandlerForSignals() {
            
            signal(SIGSEGV, printStackTraceHandler);   // install our handler
            
            signal(SIGHUP, printStackTraceHandler);
            signal(SIGINT, printStackTraceHandler);
            signal(SIGQUIT, printStackTraceHandler);
            signal(SIGILL, printStackTraceHandler);
            signal(SIGTRAP, printStackTraceHandler);
            signal(SIGABRT, printStackTraceHandler);
            signal(SIGFPE, printStackTraceHandler);
            signal(SIGKILL, printStackTraceHandler);
            signal(SIGSEGV, printStackTraceHandler);
            signal(SIGSYS, printStackTraceHandler);
            signal(SIGTERM, printStackTraceHandler);
            
        }
        
        //std::unique_ptr<ThreadPool>  pico_collection::delete_thread_pool (new ThreadPool(numberOfDeletionThreads));
        
        //
        void clientServerExample() {
            try {
                
                std::map <string, string> ThreadNamesMap;
                
                using namespace PoconoDB;
                
                //     std::shared_ptr<PonocoDriverHelper> sharedSyncHelper  (new PonocoDriverHelper);
                PonocoDriverHelper* sharedSyncHelper  =new PonocoDriverHelper();
                std::shared_ptr<DriverType> ptr(new DriverType());
                //         DriverType* ptr = new DriverType(sharedSyncHelper);
                boost::thread serverThread(runServer);
                // sleepViaBoost(1);
                //        thread::native_handle_type hnd=serverThread.native_handle();
                //        pthread_setname_np
                //        string threadIdStr = convertToString(serverThread.get_id());
                //        string serverName("serverThread");
                //
                //        ThreadNamesMap[threadIdStr] =serverName;
                //        printElementsOfMap(ThreadNamesMap);
                
                
                // boost::bind(runPonocoDriver,_1, _2)(*ptr,sharedSyncHelper);
                //auto func = std::bind(runPonocoDriver,_1, _2,ptr,sharedSyncHelper);
                
                PonocoRunnable driverThreadRunnable(ptr.get(),sharedSyncHelper);
                
                // boost::thread picoDriverThread(boost::bind(runPonocoDriver,_1, _2)(ptr,sharedSyncHelper));
                boost::thread  poncoDriverThread(boost::bind(&PonocoRunnable::runPonocoDriver,driverThreadRunnable));
                // sleepViaBoost(1);
                
                //bind(f, _2, _1)(x, y);
                
                // auto func1= std::bind(runPoncoClientProgram,_1, _2,ptr,sharedSyncHelper);
                PonocoRunnable clientThreadRunnable(ptr.get(),sharedSyncHelper);
                boost::thread poncoClientThread(boost::bind(&PonocoRunnable::runPoncoClientProgram,clientThreadRunnable));
                
                
                //        poncoClientThread.detach();
                //        picoDriverThread.detach();
                //        serverThread.detach();
                
                //a thread that is waiting on a condition variable, cannot be joined
                
                if(serverThread.joinable()) {serverThread.join();}
                //        if(poncoClientThread.joinable())
                //        {poncoClientThread.join();}
                //        if(picoDriverThread.joinable())
                //        {picoDriverThread.join();}
                
            } catch (std::exception& e) {
                std::cerr << "Exception: " << e.what() << "\n";
                raise(SIGABRT);
                
            } catch (...) {
                std::cerr << "Exception: unknown thrown" << "\n";
                raise(SIGABRT);
                
            }
        }
        

        int main(int argc, char** argv) {
            bool homeSetting = false;
            
            if(homeSetting)
            {
                PicoConfig::logDir.clear();
                PicoConfig::logDir.append("/Users/mahmoudtaabodi/Documents/pico_logs/");
                PicoConfig::dataDir.clear();
                PicoConfig::dataDir.append("/Users/mahmoudtaabodi/Documents/pico_data/");
                
                
            }
            
            
            try {
                
                string names[3] = {"session.log","client.log","gicapods.log"};
                for (int i = 0; i<3; ++i){
                    
                    
                    string removeMe = PicoConfig::logDir;
                    removeMe.append(names[i]);
                    truncateTheFile(removeMe);
                    //             if( remove( removeMe ) != 0 )
                    //        {
                    //            perror( "Error deleting file" );
                    //        }
                    //        else{
                    //            assert(false);
                    //        
                    //        }
                }
                
                
                pico_logger_wrapper logger;//just to initialize the static pointer
                registerPrintStackHandlerForSignals();
                std::set_unexpected(myunexpected);
                //        test_pico_index();
                //  testThreadPool();
                clientServerExample();
                
                // runChatServer();
                //		readingAndWritingRecordData();
                //		jsonCPPexample() ;
                //		readingAndWritingComplexData();
                // file_example();
                //   templateSpecialiaztionOfSharedPtrExample();
            } catch (const std::exception& e) {
                cout << " exception : " << e.what() << endl;
                raise(SIGABRT);
                
            }
            catch (...) {
                cout << "<----->unknown exception thrown.<------>";
                raise(SIGABRT);
                
            }
            return 0;
        }
        

    }
}

#endif
