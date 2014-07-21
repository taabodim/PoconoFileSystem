//
//  Utils.h
//  PoconoDB
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoDB_Utils_h
#define PoconoDB_Utils_h
#include <boost/lexical_cast.hpp>

#include "Configs.h"
#include <stdio.h>      // for sprintf()
#include <fstream>
#include <iostream>     // for console output
#include <string>       // for std::string
#include <cassert>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include <assert.h>
#include <chrono>
#include <ctime>
#include <ratio>
#include <string>       // for std::string
#include <stdlib.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>
#include "PicoConfig.h"
#include <assert.h>
#include <chrono>
#include <ctime>
#include <ratio>
#include <stdio.h>      // for sprintf()

#include <iostream>     // for console output

using namespace std;
namespace PoconoDB {
    typedef signed long offsetType ;
    const static size_t BLOCK_SIZE = 64;
    offsetType STARTING_OFFSET_OF_COLLECITON_INDEXS = 0;//2048
    offsetType ENDING_OFFSET_OF_COLLECITON_INDEXS = 4096;
    offsetType STARTING_OFFSET_OF_DATA_BLOCK = 4096+(BLOCK_SIZE);//2048
    bool debug = true;



    std::string getFullCollectionName(std::string name) {
        std::string fullname = Configs::dataDir;
        std::string ext(".dat");
        fullname.append(name);
        fullname.append(ext);
        return fullname;
    }
    bool checkFileExist(const std::string& name) {
        std::fstream f;
        f.open(name, std::fstream::in); //this will open file if it doesnt exist
        if (f.good()) {
            f.close();
            //   mylogger<<"file does exist...\n";
            return true;
        } else {
            //      mylogger<<"file does NOT exist,creating one now...\n";
            return false;
        }
    }

    bool openFileIfItDoesntExist(const std::string& name) {
        std::fstream f;
        std::cout<<"filenzme is "<<name<<std::endl;
        f.open(name, std::fstream::in); //this will open file if it doesnt exist
        if (f.good()) {
            f.close();
            //   mylogger<<"file does exist...\n";
            return true;
        } else {
            //      mylogger<<"file does NOT exist,creating one now...\n";
            FILE * pFile;
            pFile = fopen(name.c_str(), "w");
           fclose(pFile);
            return false;
        }
    }
    void truncateTheFile(const std::string& name) {
        FILE * pFile;
        pFile = fopen(name.c_str(), "w");
        const char* str = "";
        assert(pFile);
       if(pFile)
        {
           // fwrite(str , 1 , sizeof(str) , pFile );
            fclose(pFile);
        }
        else {std::cout<<"couldnt open the file";}
    }
    size_t getEndOfFileOffset(std::string filename)
    {
        FILE *fp;
        assert(!filename.empty());
        fp=fopen(filename.c_str(),"r+b");
        assert(fp!=NULL);
        fseek(fp, 0L, SEEK_END);
        size_t sz = ftell(fp);
        fclose(fp);

        return sz;
    }
    offsetType getEndOfFileOffsetAsMultipleOfBlock(std::string filename,size_t blockSize)
    {
        FILE *fp;
        assert(!filename.empty());
        fp=fopen(filename.c_str(),"r+b");
        assert(fp!=NULL);
        fseek(fp, 0L, SEEK_END);
        size_t sz = ftell(fp);
        fclose(fp);
        offsetType diff;
        std::cout<<" initial sz : "<<sz<<std::endl;

        if(sz%blockSize)
        {
            diff = sz%blockSize;
            sz+=(blockSize-diff);
        }
        assert(diff<blockSize);
        std::cout<<" blockSize: "<<blockSize<<std::endl
        <<" diff : "<<diff<<std::endl<<" sz : "<<sz<<std::endl;

        return sz;
    }
    offsetType getEndOfFileDataBlockOffsetAsMultipleOfBlock(std::string filename,size_t blockSize)
    {
        FILE *fp;
        assert(!filename.empty());
        fp=fopen(filename.c_str(),"r+b");
        assert(fp!=NULL);
        fseek(fp, 0L, SEEK_END);
        size_t sz = ftell(fp);
        fclose(fp);
        offsetType diff;
        std::cout<<" initial sz : "<<sz<<std::endl;
        if (sz<PoconoDB::STARTING_OFFSET_OF_DATA_BLOCK){
            sz = PoconoDB::STARTING_OFFSET_OF_DATA_BLOCK;
        }
        if(sz%blockSize)
        {
            diff = sz%blockSize;
            sz+=(blockSize-diff);
            std::cout<<" blockSize: "<<blockSize<<std::endl
        <<" diff : "<<diff<<std::endl<<" sz : "<<sz<<std::endl;

        }else
        {
              std::cout<<" blockSize: "<<blockSize<<std::endl
              <<std::endl<<" sz : "<<sz<<std::endl;

        }

        return sz;

    }
    template<typename T>
    std::string toStr(T i) {
        std::string str = boost::lexical_cast < std::string > (i);
        return str;
    }

    template<typename T>
    T toStr(std::string sth) {
        T str = boost::lexical_cast < T > (sth);
        return str;

    }

    //-----------------------------------------------------------------------------
    // Format current time (calculated as an offset in current day) in this form:
    //
    //     "hh:mm:ss.SSS" (where "SSS" are milliseconds)
    //-----------------------------------------------------------------------------
    std::string getTimeNowAsString()
    {
        // Get current time from the clock, using microseconds resolution
        const boost::posix_time::ptime now =
        boost::posix_time::microsec_clock::local_time();

        // Get the time offset in current day
        const boost::posix_time::time_duration td = now.time_of_day();

        //
        // Extract hours, minutes, seconds and milliseconds.
        //
        // Since there is no direct accessor ".milliseconds()",
        // milliseconds are computed _by difference_ between total milliseconds
        // (for which there is an accessor), and the hours/minutes/seconds
        // values previously fetched.
        //
        const long hours        = td.hours();
        const long minutes      = td.minutes();
        const long seconds      = td.seconds();
        const long milliseconds = td.total_milliseconds() -
        ((hours * 3600 + minutes * 60 + seconds) * 1000);

        //
        // Format like this:
        //
        //      hh:mm:ss.SSS
        //
        // e.g. 02:15:40:321
        //
        //      ^          ^
        //      |          |
        //      123456789*12
        //      ---------10-     --> 12 chars + \0 --> 13 chars should suffice
        //
        //
        char buf[40];
        sprintf(buf, "%02ld:%02ld:%02ld.%03ld",
                hours, minutes, seconds, milliseconds);

        return buf;
    }


    long convertStringToLong(std::string str)
    {
        long i = atol (str.c_str());
        return i;
    }
    std::string convertLongToString(long num)
    {
        return toStr(num);
    }
//    void setTheStackSize()
//    {
//        const rlim_t kStackSize = 32L * 1024L * 1024L;   // min stack size = 64 Mb
//        struct rlimit rl;
//        int result;
//
//        result = getrlimit(RLIMIT_STACK, &rl);
//        if (result == 0)
//        {
//            if (rl.rlim_cur < kStackSize)
//            {
//                rl.rlim_cur = kStackSize;
//                result = setrlimit(RLIMIT_STACK, &rl);
//                if (result != 0)
//                {
//                    fprintf(stderr, "setrlimit returned result = %d\n", result);
//                }
//                std::cout<<"result of stack change is "<<result<<std::endl;
//                assert(result==0);
//            }
//        }
//
//        // ...
//
//
//
//    }

    typedef long offsetType;
    typedef std::string messageType;

    std::string random_string(size_t length) {
        auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        return str;
    }

    std::string random_string(std::string firstPart, size_t length) {
        auto randchar = []() -> char
        {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
        };
        std::string str(length, 0);
        std::generate_n(str.begin(), length, randchar);
        str.append(firstPart);
        return str;
    }
    template<typename T>
    T random_number(std::size_t N = 5) {
        auto randchar = []() -> char
        {
            using namespace std::chrono;

            steady_clock::time_point t1 = steady_clock::now();

            const char charset[] =
            "09182736455463728190";
            const size_t max_index = (sizeof(charset) - 1);

            steady_clock::time_point t2 = steady_clock::now();

            nanoseconds now = duration_cast<nanoseconds>(t2-t1);

            return charset[ (rand() ^ now.count() ^ now.count() ) % max_index ];
        };
        std::string retStr(N, 0);
        std::generate_n(retStr.begin(), N, randchar);

        T num = boost::lexical_cast < T > (retStr);
        // mylogger << "retStr : "<<retStr<<std::endl;
        return num;
    }
    template<typename T>
    std::string convertToString(T i) {
        std::string str = boost::lexical_cast < std::string > (i);
        return str;
    }

    template<typename T>
    T convertToSomething(std::string sth) {
        T str = boost::lexical_cast < T > (sth);
        return str;

    }


    template<typename T>
    T calc_hash_code(T obj) {

        std::string objStr = boost::lexical_cast < std::string > (obj);
        std::size_t hash_code = std::hash<std::string>()(objStr);

        return hash_code;

    }

    std::size_t calc_hash_code(std::string obj) {

        std::size_t hash_code = std::hash<std::string>()(obj);
        return hash_code;

    }


    string calc_request_id() {
        return random_number<string>(32);
    }

    void sleepViaBoost(int seconds)
    {

        //boost::this_thread::sleep(boost::posix_time::seconds(seconds));
        //this throws some weird exception

        //    std::chrono::milliseconds dura( seconds*1000 );
        //    std::this_thread::sleep_for( dura );

    }
    std::string getTimeNowAsString_Old() {
        using std::chrono::system_clock;
        system_clock::time_point today = system_clock::now();

        std::time_t tt;
        tt = system_clock::to_time_t ( today );
        std::string timeStr =toStr( ctime(&tt));
        return timeStr;

    }

    void getProperMessageAboutSize(std::string data,std::string& properMessageAboutSize)
    {
        const char* dataPtr = data.c_str();

        for(size_t i=0;i<4;i++)
        {
            properMessageAboutSize.push_back('#');
        }
        for(int i=0;i<data.size();i++,dataPtr++)
        {
            properMessageAboutSize.push_back(*dataPtr);
        }


    }

    //    //-----------------------------------------------------------------------------
    //    // Format current time (calculated as an offset in current day) in this form:
    //    //
    //    //     "hh:mm:ss.SSS" (where "SSS" are milliseconds)
    //    //-----------------------------------------------------------------------------
    //    std::string getTimeNowAsString()
    //    {
    //        // Get current time from the clock, using microseconds resolution
    //        const boost::posix_time::ptime now =
    //        boost::posix_time::microsec_clock::local_time();
    //
    //        // Get the time offset in current day
    //        const boost::posix_time::time_duration td = now.time_of_day();
    //
    //        //
    //        // Extract hours, minutes, seconds and milliseconds.
    //        //
    //        // Since there is no direct accessor ".milliseconds()",
    //        // milliseconds are computed _by difference_ between total milliseconds
    //        // (for which there is an accessor), and the hours/minutes/seconds
    //        // values previously fetched.
    //        //
    //        const long hours        = td.hours();
    //        const long minutes      = td.minutes();
    //        const long seconds      = td.seconds();
    //        const long milliseconds = td.total_milliseconds() -
    //        ((hours * 3600 + minutes * 60 + seconds) * 1000);
    //
    //        //
    //        // Format like this:
    //        //
    //        //      hh:mm:ss.SSS
    //        //
    //        // e.g. 02:15:40:321
    //        //
    //        //      ^          ^
    //        //      |          |
    //        //      123456789*12
    //        //      ---------10-     --> 12 chars + \0 --> 13 chars should suffice
    //        //
    //        //
    //        char buf[40];
    //        sprintf(buf, "%02ld:%02ld:%02ld.%03ld",
    //                hours, minutes, seconds, milliseconds);
    //
    //        return buf;
    //    }



}

#endif
