//
//  Utils.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_Utils_h
#define PoconoFileSystem_Utils_h
#include <boost/lexical_cast.hpp>

#include "Configs.h"
#include <stdio.h>      // for sprintf()
#include <fstream>
#include <iostream>     // for console output
#include <string>       // for std::string
#include <cassert>
#include <stdlib.h>  
namespace PoconoFileSystem{
    typedef signed long offsetType ;
    const static size_t BLOCK_SIZE = 64;
    offsetType STARTING_OFFSET_OF_COLLECITON_INDEXS = 0;//2048
    offsetType ENDING_OFFSET_OF_COLLECITON_INDEXS = 4096;
    offsetType STARTING_OFFSET_OF_DATA_BLOCK = 4096+(BLOCK_SIZE);//2048

    
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
            
            f.close();
            return false;
        }
    }
    void truncateTheFile(const std::string& name) {
        FILE * pFile;
        pFile = fopen(name.c_str(), "w");
        char str[] = "fileContent was deleted!";
        
        fwrite(str , 1 , sizeof(str) , pFile );
        fclose(pFile);
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
        if (sz<PoconoFileSystem::STARTING_OFFSET_OF_DATA_BLOCK){
            sz = PoconoFileSystem::STARTING_OFFSET_OF_DATA_BLOCK;
        }
        if(sz%blockSize)
        {
            diff = sz%blockSize;
            sz+=(blockSize-diff);
        }
        std::cout<<" blockSize: "<<blockSize<<std::endl
        <<" diff : "<<diff<<std::endl<<" sz : "<<sz<<std::endl;
        
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
}

#endif
