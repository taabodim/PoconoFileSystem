//
//  CollectionIndex.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/6/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_CollectionIndex_h
#define PoconoFileSystem_CollectionIndex_h

namespace PoconoFileSystem {
    class CollectionIndex {
        private :
        
        char[32] collectionName;
        char[32] collectionOffset;
        
        public :
        
        
    };
    
    typedef std::shared_ptr<CollectionIndex> CollectionIndexPtr;
    
}

#endif
