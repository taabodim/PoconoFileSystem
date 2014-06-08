//
//  DataBlock.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_DataBlock_h
#define PoconoFileSystem_DataBlock_h

namespace PoconoFileSystem {
    class DataBlock {
        private :
        const static size_t DATA_BLOCK_SIZE = 32;
        const static size_t DATA_BLOCK_OFFSET_SIZE = 32;
         
        char[DATA_BLOCK_SIZE] collectionName;
        char[DATA_BLOCK_OFFSET_SIZE] nextDataBlockPtr;
        char[DATA_BLOCK_OFFSET_SIZE] previousDataBlockPtr;
        
        public :
        
        
    };
    
    typedef std::shared_ptr<DataBlock> DataBlockPtr;
    
}

#endif
