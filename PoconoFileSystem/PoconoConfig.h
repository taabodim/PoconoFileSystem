//
//  PoconoConfig.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_PoconoConfig_h
#define PoconoDB_PoconoConfig_h

namespace PoconoDB {
    
    class PoconoConfig {
    public:
        static std::string dataDir;
        static long defaultTimeoutInSec;
        
        static std::string logDir;
        static std::string logFileName;
    };
    
}


#endif
