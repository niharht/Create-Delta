//
//  Node.cpp
//  Project 4
//
//  Created by Nihar Tamhankar on 5/30/16.
//  Copyright © 2016 Nihar Tamhankar. All rights reserved.
//

#include "Node.h"
#include <iostream>
using namespace std;

Node::Node(){
    next=nullptr;
}

NodeHashTable::NodeHashTable(int size):m_size(size), hashtable(size, nullptr){  //makes a hashtable of with size buckets all point to nullpointer  
    
}
void NodeHashTable::insertNode(Node* node){
    hash<string> hasfunc;   //using C++ hash function for string
    int index =(hasfunc(node->token)% m_size);     //hashing based on value
    Node* current= hashtable[index];
    if(current==nullptr){
        hashtable[index]=node;    //if hash value points to null, entering first value
        return;
    }
    while(current->next!=nullptr){
        current=current->next;     //adding to linked list
    }
    current->next=node;
    node->next=nullptr;
}

Node* NodeHashTable::find( string pattern){  ///using C++ hash string function to see if value is in hash table
    hash<string>hasfunc;
    int index=(hasfunc(pattern)%m_size);
    Node* current= hashtable[index];
    while(current!=nullptr){
        if(current->token==pattern){
            return current;
        }
        current=current->next;
    }
    return nullptr;///if not found return null
}
