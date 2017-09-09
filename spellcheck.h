#ifndef SPELLCHECK_H
#define SPELLCHECK_H
#include <string>
//#include "/windows/g/Programming/spell/stdafx.h" // metaphone data
#include "MetaPhone.h" // metaphone data
#include "avl/AVLTree.h"
#include "avl/AVLTreeStruct.h"
//#include "/windows/g/Programming/spell/avl/avltree.h"
//#include "/windows/g/Programming/spell/avl/AVLTreeStruct.h"

using namespace std;

#define LOW_FIND .20
#define HIGH_FIND .60
#define SIZE_TO_LIST 1000
#define MAX_RESULTS 10
#define PATH  "/windows/g/Programming/spell/" //"G:\\Programming\\spell\\"
// #define DEBUG 1
// offers debugging techniques
string lc(CString one)
{
    for (int i =0; i < one.data.length(); i++)
        if (one.data[i] >=65 && one.data[i] <=90)
            one.data[i]+=32;
    return one.data;
}    

string lcs(string one)
{
    for (int i =0; i < one.length(); i++)
        if (one[i] >=65 && one[i] <=90)
            one[i]+=32;
    return one;
}    
class
spellChecker
{
    public:
    string singleSuggest;
    spellChecker(){ singleSuggest=""; }
    bool check(string);
    AVLTree< AVLTreeStruct<string,string> > &suggestions(){ return spellList; }

    private:
        double estimate(string,string);
        AVLTree< AVLTreeStruct<string,string> > spellList;
};    

bool
spellChecker::check(string searchString)
{
    
    CString one,two;
    MString ss(searchString.c_str());
    ss.DoubleMetaphone(one,two);
    if (one.data[0]=='0')
        one=two;
    one = lc(one);
    string filepath="";
    filepath=PATH;
    filepath+=one.data[0];
    filepath+="/";
    if (one.data.length() >= 2)
    {
    filepath+=one.data.substr(0,2);
    filepath+="/";
    }
    filepath+=one.data;
    filepath+=".dat";
    int perm=1;
    char tw[3];

    
    ifstream spellFile(filepath.c_str());
    bool findp=false;
    if (!spellFile )
    {
        // try all permutations
        if (one.data.length() >= 2)
        {
                string six=filepath.substr(0,6);
                            six=PATH;
                six+=one.data[0];
                six+="/";

            six+=one.data.substr(0,2);
            six+="/";
        
            six+=one.data.substr(0,6);
        six+="~";
        sprintf(tw,"%i",perm);
        six+=tw;
            six+=".dat";

        spellFile.clear();
        spellFile.open(six.c_str());

        }    
        if (!spellFile)
            return false;

    }

    string fileReader="";
    bool found=false;
    int numfound=0;
    double checkAmnt=0;
    if (searchString.length() > 4)
        checkAmnt=HIGH_FIND;
    else
        checkAmnt=LOW_FIND;
    while(spellFile)
    {             
    while(spellFile >> fileReader)
    {
        if (fileReader.length() > 0)
        {
            if (lcs(searchString)==fileReader)
            {
                found=true;
                break;
            }
            AVLTreeStruct<string,string> temp;
        
            if (estimate(searchString,fileReader) >= checkAmnt && numfound!=MAX_RESULTS)
            {
                if (numfound == 0)
                    singleSuggest=fileReader;
                numfound++;
                temp.SetData(fileReader);
                temp.SetKey(fileReader);
                spellList.Insert(temp);
            }
        }
    }
}   
//    spellList.PrintKeys();
    spellFile.close();
    return found;
}    

double
spellChecker::estimate(string comp1,string comp2)
{
    int total = comp1.length();
    int crc=0;
    if (comp1.length() > 2 && comp2.length() > 1)
    {
        if (comp1[0]=='p' && comp1[1]=='h' && comp2[0]=='f')
            crc++;
    }
    if (comp1[0]=='p' && comp1[1]=='h' && comp2[0]=='f')
    {
        for (int i =2,j=1; j < total,i <comp2.length(); i++,j++)
        {
               if (comp1[i]==comp2[j])
                   crc++;
                 
        }
            
    }
    else
    {
        for (int i =total-1; i >=0; i--)
        {
            if (i < comp2.length())
                if (comp1[i]==comp2[i])
                    crc++;
                
        }
    }
    return (double(crc)/double(total));
}

#endif
