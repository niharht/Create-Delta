//
//  delta.cpp
//  Project 4
//
//  Created by Nihar Tamhankar on 5/27/16.
//  Copyright © 2016 Nihar Tamhankar. All rights reserved.
//

///create get next item
/// add items to hash table
//take new file and find item in hash table
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "Node.h"
using namespace std;


void createDelta(istream& oldf, istream& newf, ostream& deltaf){
    string Oldfilestring;
    string line;
    int N=16;
    while(getline(oldf,line)){  ///as long as not end of file, adding contents of file into a string and adding newline as necessary
        Oldfilestring+=line;
        if(!oldf.eof()){
            Oldfilestring+='\n';
        }
        
    }
    
    string newFilestring;
    while(getline(newf, line)){///as long as not end of file, adding contents of file into a string and adding newline as necessary
        newFilestring+=line;
        if(!newf.eof()){
            newFilestring+='\n';
            
        }
        
    }
    
    NodeHashTable oldstringTokens(2000);
    int currentindex=0;
    while(currentindex<Oldfilestring.size()){   ///until end of string substringing and adding to hashtable
        Node* s= new Node;
        s->token=Oldfilestring.substr(currentindex,N);
        s->index=currentindex;
        oldstringTokens.insertNode(s);
        currentindex=currentindex+N;
    }
    
    
    currentindex=0;
    int startunmatchedindex=0;
    while(currentindex<newFilestring.length()){
        string token=newFilestring.substr(currentindex,N);
        Node* matchnode=oldstringTokens.find(token);
        if(matchnode!=nullptr){
            
            if(currentindex > startunmatchedindex){ //for chracters behind matches, creating add instruction
                string delta=newFilestring.substr(startunmatchedindex,currentindex-startunmatchedindex);
                deltaf<<"A"<<(currentindex-startunmatchedindex)<<":"<<delta;
                
                
            }
            string oldtoken=matchnode->token;
            int oldtokenIndex=(matchnode->index)+(int)(oldtoken.size());
            currentindex+=(int)oldtoken.size();
            while(oldtokenIndex<Oldfilestring.size() && currentindex<newFilestring.size()){
                //once match has been found looking beyond to see if more match
                
                if (Oldfilestring[oldtokenIndex] != newFilestring[currentindex]) {
                    break;
                }
                oldtoken += oldtoken[oldtokenIndex];
                ++oldtokenIndex;
                ++currentindex;
                
            }
            // make copy to delta file with copy instruction matchedNode->index, oldToken string (actual characters and size)
            deltaf<<"C"<<oldtoken.length()<<","<<matchnode->index;
            startunmatchedindex=currentindex;
        }
        
        else { ///no match found
            currentindex += 1;
        }
    }
    //copy last unmatched string
    // copy all the unmatched characters
    if(currentindex > startunmatchedindex){
        string delta=newFilestring.substr(startunmatchedindex,currentindex-startunmatchedindex);
        deltaf<<"A"<<(currentindex-startunmatchedindex)<<":"<<delta;
    }
    
}
bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch)  ||  !isascii(ch)  ||  !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
    if (!inf.get(cmd)  ||  (cmd == '\n'  &&  !inf.get(cmd)) )
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    char ch;
    switch (cmd)
    {
        case 'A':
            return getInt(inf, length) && inf.get(ch) && ch == ':';
        case 'C':
            return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);
    }
    return false;
}

bool applyDelta(istream& oldf, istream& deltaf, ostream& newf){
    string Oldfilestring;
    string line;
    while(getline(oldf,line)){  ///as long as not end of file, adding contents of file into a string and adding newline as necessary
        Oldfilestring+=line;
        if(!oldf.eof()){
            Oldfilestring+='\n';
        }
    }
    int length;
    int offset;
    char cmd;
    while(getCommand(deltaf, cmd, length, offset)==true){   //checkinggetCommand is valid
        bool leaveloop=false; ///for checking if at end of the file
        switch (cmd) {
            case 'x':{
                leaveloop=true; ///at end of the file
                break;
            }
            case 'A':{
                char* addstring=new char[length+1];   //using C++ read function from where character is and for how long then adding string
                deltaf.read(addstring, length);
                addstring[length]='\0';
                newf<<addstring;
                delete[] addstring;
                break;
            }
            case 'C':
                string copy=Oldfilestring.substr(offset,length); //copying from old file with index and length
                newf<<copy;
                break;
                
        }
        if(leaveloop==true){ ///have reached end of file
            return true;
        }
    }
    return false;
}


int main(){
    ifstream oldf("/Users/nihartamhankar/Downloads/p4test/greeneggs1.txt");
    ifstream newf("/Users/nihartamhankar/Downloads/p4test/greeneggs2.txt");
    ofstream deltaf("/Users/nihartamhankar/Downloads/p4test/greeneggsdelta16.txt");
    ofstream changef("/Users/nihartamhankar/Downloads/p4test/greeneggschanges.txt");
    createDelta(oldf, newf, deltaf);
    oldf.close();
    newf.close();
    deltaf.close();
    ifstream old2f("/Users/nihartamhankar/Downloads/p4test/greeneggs1.txt");
    ifstream delta2f("/Users/nihartamhankar/Downloads/p4test/greeneggsdelta16.txt");
    applyDelta(old2f, delta2f, changef);
    cout<<"Passed all tests"<<endl;
}



