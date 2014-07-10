//
//  DBTest.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 7/10/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_DBTest_h
#define PoconoDB_DBTest_h

namespace PoconoDB {
    
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
}
#endif
