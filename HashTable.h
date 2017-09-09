#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>

#include "hash.h"
#include "vpointer.h"
#include "isprime.h"

#include "avl/AVLTree.h"
//#include "AVL/AVLTreeStruct.h"

using namespace std;

#define TEMPLATE_DEFINE template <class K, class D>

TEMPLATE_DEFINE
class HashTable : private PrimeFunctions
{
    public:
//        HashTable(unsigned int(*act)(K&),int);
            HashTable();//{ initsize=10; cout << " error could be here?" << endl; }
        HashTable(unsigned int(*act)(K&),unsigned long,D);
        void Set(unsigned int(*act)(K&),unsigned long,D);
	   unsigned long AllowableSize(){ return initSize; }
//        ~HashTable(){ EmptyNodes(HashLookup); }

        Hash<K,D> &operator[](K); // get K type and return reference to D of vector

  //      bool AddHash(K,D); // success or failure

//	bool DeleteHash(K);

//	D &GetHash(K); // 
    protected:

//        void EmptyNodes(vector< VPOINTER< Hash<K,D> > > &); // beginning nodes

        unsigned int (*action)( K& );
	
	vector< VPOINTER< Hash<K,D> > > HashLookup; 
        
	Hash<K,D>  &SearchKey(K,vector< VPOINTER< Hash<K,D>  > > &);
	Hash<K,D>  &SearchKey(K);
	
	// unsigned int because values will never be negative
        unsigned long initSize;
        D emptyData;
        unsigned int numResizes;
        unsigned int tableExist;
        unsigned long tableSize;
     
};

//#include "HashTable.cpp"


TEMPLATE_DEFINE
HashTable<K,D>::HashTable()
{
//    action=act;
//    emptyData = 0;
    initSize = NearestPrime(10);
    HashLookup.resize(initSize);
    
}
TEMPLATE_DEFINE
HashTable<K,D>::HashTable(unsigned int(*act)(K&),unsigned long size,D id)
{
    action=act;
    emptyData = id;
    initSize = NearestPrime(size);
    HashLookup.resize(initSize);
    
    
}
TEMPLATE_DEFINE
void
HashTable<K,D>::Set(unsigned int(*act)(K&),unsigned long size,D id)
{
    action=act;
    emptyData = id;
    initSize = NearestPrime(size+10);
    HashLookup.resize(initSize);
    
    
}


TEMPLATE_DEFINE
Hash<K,D> 
&HashTable<K,D>::operator[](K inKey)
{
	return SearchKey(inKey,HashLookup);
}


TEMPLATE_DEFINE
Hash<K,D>
&HashTable<K,D>::SearchKey(K mySearchKey,vector< VPOINTER< Hash<K,D>  > > &beginFrom)
{
    
    unsigned long keyLocation = action(mySearchKey) % initSize; // try to find location 
    if ( beginFrom[keyLocation].GetStruct().GetKey() == mySearchKey && beginFrom[keyLocation].GetStruct().GetStatus() != EMPTY)
        return beginFrom[keyLocation].GetStruct();
    else if ( beginFrom[keyLocation].GetStruct().GetStatus() == EMPTY)
    {
	beginFrom[keyLocation].GetStruct().SetKey(mySearchKey);
	beginFrom[keyLocation].GetStruct().SetData(emptyData);
        return beginFrom[keyLocation].GetStruct();
	}
    else
    {
    	Hash<K,D> searchtemp;
	searchtemp.SetKey(mySearchKey);
       if (beginFrom[keyLocation].linkTree.SearchTree(searchtemp))
       	{
        return beginFrom[keyLocation].linkTree.FoundData();
	}
       else
        {
		    Hash<K,D> *temp = new Hash<K,D>;
            temp->SetKey(mySearchKey);
	    temp->SetData(emptyData);
            beginFrom[keyLocation].linkTree.Insert( *temp );
	    beginFrom[keyLocation].linkTree.SearchTree(searchtemp);
	    return beginFrom[keyLocation].linkTree.FoundData();
        }
    }
}



TEMPLATE_DEFINE
Hash<K,D>
&HashTable<K,D>::SearchKey(K mySearchKey)
{
    return SearchKey(mySearchKey,HashLookup);
}

unsigned int
stoi(string& input)
{
    int total=0;
    for (int i =0; i < input.length(); i++)
        total+=(int)input.at(i);
    return total;

}
#endif
// iS represents input Size
// PFv represents PrimeFunction variable
// DHT represents Data Hash Table
// KHT represents Key Hash Table
// K represents KEY TYPE, D represents DATA TYPE
// rT represents return Value
