//
//  PoconoConfig.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_PoconoConfig_h
#define PoconoFileSystem_PoconoConfig_h

namespace PoconoFileSystem{
    
    class PoconoConfig {
    public:
        static std::string dataDir;
        static long defaultTimeoutInSec;
        
        static std::string logDir;
        static std::string logFileName;
    };
    
}


#endif
