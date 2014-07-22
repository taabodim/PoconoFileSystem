#ifndef ASYNC_WRITER_H
#define ASYNC_WRITER_H
#include "pico_record.h"
#include "Utils.h"
#include "ConcurrentVector.h"
#include "pico_logger_wrapper.h"
using namespace std;
namespace PoconoDB {
    class writer_buffer_container : public pico_logger_wrapper
    {
    
    public:
        //this has to be a list of pointer to buffers because we want to keep the buffers in the heap until
        // all the buffer is fully written to the other side...also the list should be kept in heap because
        // we cant copy a list to another list in c++, so we copy the pointers to it.
        std::shared_ptr<ConcurrentVector<std::shared_ptr<pico_record>,VectorTraits<pico_record>>>writerMessageList;
        
        
        
        
        
        
        
        
        
        writer_buffer_container():   writerMessageList ( new ConcurrentVector<std::shared_ptr<pico_record>,VectorTraits<pico_record>>())
        {
            mylogger<<("empty writer_buffer_container being constructed....");
           }
        writer_buffer_container(std::shared_ptr<ConcurrentVector<std::shared_ptr<pico_record> ,VectorTraits<pico_record>>> list)
        
        {
            mylogger<<("writer_buffer_container being constructed....");
             writerMessageList = list;
        }
        ~writer_buffer_container(){
            //std::cout<<("writer_buffer_container being destructed....");
        }
        void addToAllBuffers(std::shared_ptr<pico_record> msg)
        {
            writerMessageList->push_back(msg);
        }
        
        
    };
}
#endif
