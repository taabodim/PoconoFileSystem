//
//  Configs.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_Configs_h
#define PoconoFileSystem_Configs_h


namespace PoconoFileSystem{
    
    class Configs {
    public:
        static std::string dataDir;
        static long defaultTimeoutInSec;
        
        static std::string logDir;
        static std::string logFileName;
    };
    
}
#endif
