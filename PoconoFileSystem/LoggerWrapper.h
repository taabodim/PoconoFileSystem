////
////  LoggerWrapper.h
////  PoconoDB
////
////  Created by Mahmoud Taabodi on 6/13/14.
////  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
////
//
//#ifndef PoconoDB_LoggerWrapper_h
//#define PoconoDB_LoggerWrapper_h
//#include "logger.h"
//
//namespace PoconoDB {
//    
//    template<typename T>
//    logger& operator << (logger& wrapper,const T& nonstr)
//    {
//        string str = boost::lexical_cast<std::string>(nonstr);
//        // std::cout<<str;
//        wrapper.log(str);
//        return wrapper;
//    }
//    
//    template<>
//    logger& operator << ( logger& wrapper,const std::string& str)
//    {
//        //std::cout<<str;
//        wrapper.log(str);
//        return wrapper;
//    }
//    
//    template <typename Type>
//    void log(logger& wrapper, Type str )
//    {
//        string realStr = convertToString(str);
//        wrapper.log(realStr);
//    }
//    
//    
//    void log(logger& wrapper,std::string str )
//    {
//        wrapper.log(str);
//        
//    }
//    
//    class loggerWrapper {
//        
//    public:
//        logger mylogger;
//        std::shared_ptr<logger> clientLogger;
//        std::shared_ptr<logger> sessionLogger;
//        std::shared_ptr<logger> dbLogger;
//        
//        loggerWrapper():mylogger("/gicapods")
//        ,clientLogger(new logger("client"))
//        ,sessionLogger(new logger("session"))
//        ,dbLogger(new logger("dbLogger"))
//        {
//            
//            
//            
//        }
//        
//        loggerWrapper(std::string logFileName):mylogger(logFileName)
//        ,clientLogger(new logger("client"))
//        ,sessionLogger(new logger("session"))
//        ,dbLogger(new logger("dbLogger"))
//        
//        {
//            
//        }
//        
//        void log(const std::string str)  {
//            mylogger<<str;
//        }
//        
//        
//        
//    };
//}
//
//#endif
