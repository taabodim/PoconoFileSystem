/*
 * asyncReader.h
 *
 *  Created on: Mar 21, 2014
 *      Author: mahmoudtaabodi
 */

#ifndef ASYNCREADER_H_
#define ASYNCREADER_H_

#include "pico_record.h"
#include "writer_buffer_container.h" //for typedef only
#include "ConcurrentVector.h"
#include "Utils.h"

#include  "pico_logger_wrapper.h"
using namespace std;
namespace PoconoDB {
    
    class asyncReader : public pico_logger_wrapper{
        
    public:
        
        asyncReader() {
            mylogger<<"asyncReader is being constructed....\n";
        }
        virtual ~asyncReader() {
            mylogger<<("asyncReader is being destructed....\n");
        }
        //
        //	msgPtr getReadBuffer() {
        //		msgPtr bufferPtr(new pico_recorded_message());
        //		readerBufferList.push(bufferPtr);
        //		return bufferPtr;
        //	}
        std::shared_ptr<pico_record> getOneBuffer() {
            
            std::shared_ptr<pico_record> buf (new pico_record());
            singleBufferList.push_back(buf);
            return buf;
        }
        
        pico_record* getOneBufferPtr() {
            
            pico_record* buf  =new pico_record();
            
            return buf;
        }
        void returnTheBuffer(std::shared_ptr<pico_record> currentBuffer) {
            singleBufferList.remove(currentBuffer);
            currentBuffer.reset();
        }
        void returnTheBuffer(pico_record* currentBuffer) {
            delete currentBuffer;
        }
        //	ConcurrentVector<msgPtr> readerBufferList;
        ConcurrentVector<std::shared_ptr<pico_record>,VectorTraits<pico_record>  > singleBufferList;
        
    private:
        //none as of now
        
    };
}

#endif /* ASYNCREADER_H_ */
