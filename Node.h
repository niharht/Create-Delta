//
//  Node.h
//  Project 4
//
//  Created by Nihar Tamhankar on 5/29/16.
//  Copyright © 2016 Nihar Tamhankar. All rights reserved.
//

#ifndef Node_h
#define Node_h
#include <string>
#include <vector>
using namespace std;

struct Node{
    Node();    //default constructor
    int index;    //index of substring in file
    string token;   //substring value
    Node* next;     //points to next pointer in linked list in hash table value
};
class NodeHashTable{
public:
    NodeHashTable(int size);
    void insertNode(Node* node);
    Node* find( string pattern);
private:
    int m_size;
    vector<Node*> hashtable;   ///have a hashtable implemented as a vector of Node*
    
};


#endif /* Node_h */
