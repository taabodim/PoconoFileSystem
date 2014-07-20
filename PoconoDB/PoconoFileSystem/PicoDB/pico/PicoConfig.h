//
//  PicoConfig.h
//  PicoDB
//
//  Created by Mahmoud Taabodi on 5/12/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PicoDB_PicoConfig_h
#define PicoDB_PicoConfig_h

namespace PoconoDB {

    class PicoConfig{
    public:
        static std::string dataDir;
        static long defaultTimeoutInSec;

        static std::string logDir;
        static std::string logFileName;
    };

}

#endif
