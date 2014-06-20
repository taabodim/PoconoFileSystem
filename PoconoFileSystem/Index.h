//
//  Index.h
//  PoconoFileSystem
//
//  Created by Mahmoud Taabodi on 6/19/14.
//  Copyright (c) 2014 Mahmoud Taabodi. All rights reserved.
//

#ifndef PoconoFileSystem_Index_h
#define PoconoFileSystem_Index_h


#include <list>
#include <memory>
using namespace std;
namespace pico {
    class pico_record_node;
    typedef pico_record_node* nodeType;
    
    //requirements of std::string
    class pico_record_node
    //: public std::enable_shared_from_this<pico_record_node>{ //this causes problem when we are not using a normal pointer
    {
    public:
        //
        offsetType key; //this key is calculated based on the hash of the first  pico record of a pico message
        offsetType offset; //this is the offset  of the first record of a pico message  in the file
        
        nodeType left; //left node in the tree
        nodeType right; //right node in the tree
        
        pico_record_node() {
            this->key = -1;
            this->offset = -1;
            this->left = nullptr;
            this->right = nullptr;
            
        }
        offsetType minKey() {
            if (left == nullptr)
                return key;
            else
                return left->minKey();
        }
        
        //share_from_this throws bad weak ptr because there is no std::shared_ptr already to create
        //another shared ptr, probably because delete thread is out of scope , so I am going to use a normal pointer
        // pico_record_node*
        //  nodeType
        pico_record_node* deleteNode(offsetType key, nodeType parent) {
            if (key < this->key) {
                if (this->left != nullptr)
                    return left->deleteNode(key, this);
                else
                    return nullptr;
            } else if (key > this->key) {
                if (this->right != nullptr)
                    return right->deleteNode(key, this);
                else
                    return nullptr;
            } else {
                if (left != nullptr && right != nullptr) {
                    this->key = right->minKey();
                    return right->deleteNode(this->key, this);
                } else if (parent->left == this) {
                    
                    parent->left = (left != nullptr) ? left : right;
                    return this; //
                } else if (parent->right == this) {
                    
                    parent->right = (left != nullptr) ? left : right;
                    // nodeType shareThis(this);
                    return this;
                }
            }
            
            return nullptr;
        }
        
        //        void printNode()
        //        {
        //
        //            mylogger<<"printing a node : \n";
        //            if(left==nullptr)
        //                mylogger<<"left : null ==> ";
        //            else
        //                mylogger<<"left : "<< left->key<<" ==> ";
        //
        //            mylogger<<" node : "<<key;
        //
        //            if(right==nullptr)
        //                mylogger<<" <== right : null";
        //            else
        //                mylogger<<" <== right : "<<right->key;
        //
        //        }
    };
    
    class pico_binary_index_tree: public pico_logger_wrapper { //this tree saves all the
    public:
        
        nodeType root;
        long numberOfNodesInTree;
        pico_binary_index_tree()
        //:root (new pico_record_node()){
        {
            
            root = new pico_record_node();
        }
        ~pico_binary_index_tree() {
            destroy_tree();
            
        }
        void destroy_tree(nodeType leaf) {
            if (leaf != nullptr) {
                
                if (leaf->left != nullptr)
                    destroy_tree(leaf->left);
                if (leaf->right != nullptr)
                    destroy_tree(leaf->right);
                delete leaf;
                // leaf->reset();//to not point to the object anymore
                //leaf=nullptr;
            }
            
        }
        
        bool remove(pico_record_node record) {
            offsetType key = record.key;
            bool retResult;
            if (root == nullptr)
                retResult = false;
            else {
                if (root->key == key) {
                    nodeType auxRoot(new pico_record_node());
                    auxRoot->key = 0;
                    auxRoot->left = root;
                    pico_record_node* removedNode = root->deleteNode(key, auxRoot);
                    root = auxRoot->left;
                    if (removedNode != nullptr) {
                        // delete removedNode;
                        removedNode = nullptr;
                        //removedNode.reset();
                        retResult = true;
                    } else
                        retResult = false;
                } else {
                    pico_record_node* removedNode = root->deleteNode(key, nullptr);
                    if (removedNode != nullptr) {
                        //delete removedNode;
                        
                        //  removedNode.reset();
                        removedNode = nullptr;
                        retResult = true;
                    } else
                        retResult = false;
                }
            }
            
            if (retResult == true)
                numberOfNodesInTree--;
            return retResult;
        }
        //
        //
        //        nodeType getSuccessor(nodeType node) {
        //            while (node->right->key!=-1)
        //                node = node->right;
        //            return node;
        //        }
        
        void insert(std::shared_ptr<DBRecord> record, nodeType leaf) {
            
            nodeType node = convert_pico_record_to_index_node(record);
            insert(node, leaf);
        }
        void insert(nodeType nodeToBeInserted, nodeType leaf) {
            offsetType key = nodeToBeInserted->key;
            offsetType offset = nodeToBeInserted->offset;
            mylogger << "pico_index : insert : nodeToBeInserted->key =key = "
            << nodeToBeInserted->key << "\n";
            mylogger << "pico_index : insert : leaf->key = " << leaf->key << "\n";
            
            if (key < leaf->key) {
                if (leaf->left != nullptr) {
                    mylogger
                    << "pico_index : insert : key is smaller checking the left node";
                    
                    insert(nodeToBeInserted, leaf->left);
                } else {
                    mylogger
                    << ("pico_index : insert : key is smaller adding a left node to this node");
                    
                    leaf->left = nodeToBeInserted;
                    leaf->left->left = nullptr; //Sets the left child of the child std::string to nullptr
                    leaf->left->right = nullptr; //Sets the right child of the child std::string to nullptr
                }
            } else if (key >= leaf->key) {
                if (leaf->right != nullptr) {
                    mylogger
                    << ("pico_index : insert : key is larger checking the right node ");
                    
                    insert(nodeToBeInserted, leaf->right);
                } else {
                    mylogger
                    << ("pico_index : insert : leaf doesnt have right child node.");
                    
                    leaf->right = nodeToBeInserted;
                    leaf->right->left = nullptr; //Sets the left child of the child std::string to nullptr
                    leaf->right->right = nullptr; //Sets the right child of the child std::string to nullptr
                }
            }
        }
        nodeType search(nodeType node, nodeType leaf) {
            offsetType key = node->key;
            mylogger
            << " pico_index : printing the  tree before search  \n";
            print_tree();
            mylogger
            << " pico_index : the record that we want to find in tree has this key  "
            << node->key << "\n";
            if (leaf != nullptr) {
                mylogger
                << " pico_index : the record that exists in tree has this key  "
                << leaf->key << "\n";
                if (key == leaf->key) {
                    mylogger
                    << " pico_index : the record was found in index with key  "
                    << leaf->key << "\n";
                    return leaf;
                } else if (key < leaf->key) {
                    
                    mylogger
                    << " pico_index : searching the left child node for the key  "
                    << key << "\n";
                    return search(node, leaf->left);
                } else {
                    mylogger
                    << " pico_index : searching the right child node for the key  "
                    << key << "\n";
                    return search(node, leaf->right);
                    
                }
            }
            
            else {
                mylogger
                << " pico_index : end of searching for node, no key found in tree  "
                << key << "\n";
                
                return nullptr;
            }
        }
        
        void insert(nodeType node) {
            insert(node, root);
        }
        nodeType search(std::shared_ptr<DBRecord> record) {
            nodeType node = search(convert_pico_record_to_index_node(record), root);
            if (node == nullptr) {
                record->offset_of_record = -1;
            } else {
                record->offset_of_record = node->offset;
            }
            return node;
        }
        void destroy_tree() {
            destroy_tree(root);
        }
        
        nodeType convert_pico_record_to_index_node(std::shared_ptr<DBRecord>  record) {
            nodeType node(new pico_record_node());
            std::hash < std::string > hash_fn;
            std::size_t key_hash = hash_fn(record->getKeyAsString()); //record key will be used to make the node key
            
            node->key = key_hash;
            // node->key = offset;//for testing the tree, just use the offset for now
            
            node->offset = record->offset_of_record; //this is the record offset that will be saved in the index
            node->left = nullptr;
            node->right = nullptr;
            return node;
        }
        
        //        void print_tree()
        //        {
        //            mylogger<<"this is the tree \n";
        //            list<nodeType> level;
        //            level.push_front(root);
        //            while(!level.empty()){
        //                nodeType node = level.front();
        //                level.pop_front();
        //
        ////                if(node->left!= nullptr && node->right!= nullptr)
        ////                {//to avoid duplicate printing of a node , only if its really a parent , it will print
        //                    mylogger<<"\nthis is the middle node being printed ";
        //                    node->printNode();
        ////                }
        //                if(node->left!= nullptr)
        //                {
        //                    mylogger<<"\nthis is left Node of the above parent being printed \n";
        //
        //                    node->left->printNode();
        //                    level.push_front(node->left);
        //                }
        //                //  mylogger<<(node->offset);
        //                if(node->right!= nullptr)
        //                {
        //                    mylogger<<"\nthis is right Node of the above parent being printed \n";
        //
        //                    node->right->printNode();
        //                    level.push_front(node->right);
        //                }
        //
        //            }
        //
        //        }
        void print_tree() {
            print_tree(root);
        }
        void print_tree(nodeType topNode)        //preorderPrint
        {
            if (topNode != nullptr) {
                mylogger << topNode->key << " ";
                print_tree(topNode->left);
                print_tree(topNode->right);
            }
        }
        void add_to_tree(std::shared_ptr<DBRecord> it) { //this method creates a tree structure
            //based on the pico records that it reads from a collection
            mylogger << "pico_index: adding this record to index " << it->toString();
            assert(it->offset_of_record >= 0);
            nodeType node = convert_pico_record_to_index_node(it);
            insert(node);
            numberOfNodesInTree++;
            mylogger << "pico_index : This is the index being printed...\n";
            print_tree();
            
        }
        /* Computes the number of nodes in a tree. */
        int size() {
            return size(root);
            
        }
        int size(pico_record_node* node) {
            if (node == NULL)
                return 0;
            else
                return (size(node->left) + 1 + size(node->right));
        }
        void build_tree(list<DBRecord> all_pico_records) { //this method creates a tree structure
            //based on the pico records that it gets reads from a collection
            mylogger << "\ntree is going to be build by " << all_pico_records.size()
            << " elements\n";
            
            for (list<DBRecord>::iterator it = all_pico_records.begin();
                 it != all_pico_records.end(); ++it) {
                
                mylogger << "\nbuild_tree : offset is " << it->offset_of_record
                << "\n";
                assert(it->offset_of_record >= 0);
                nodeType node = convert_pico_record_to_index_node(it);
                insert(node);
                numberOfNodesInTree++;
            }
            
        }
        
        void test_tree() {
            const int num = 20;
            nodeType allNodesInsertedInTree[num];
            std::shared_ptr<DBRecord> allRecordsInsertedInTree[num];
            
            string key("mahmoudkey");
            for (int i = 0; i < num; i++) {
                int offset = random_number<int>(3);
                std::shared_ptr<DBRecord>  record (new DBRecord());
                record->offset_of_record = offset;
                //record.setKey(random_string(key,5));
                allNodesInsertedInTree[i] = convert_pico_record_to_index_node(
                                                                              record);
                allRecordsInsertedInTree[i] = record;
                insert(allNodesInsertedInTree[i]);
            }
            
            //  print_tree();
            //remove(allNodesInsertedInTree[1]->key);
            //mylogger<<"this is the tree after deletion of a node \n";
            mylogger << "this is wehere the second record  is"
            << search(*allRecordsInsertedInTree[1]);
            // print_tree();
            
        }
        
    };
}

#endif
