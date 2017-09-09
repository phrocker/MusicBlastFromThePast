#ifndef HASH_H
#define HASH_H

// iH represents input Hash


enum TYPES{ EMPTY=0, DELETE, FULL,INTIALIZED };

template <class K, class D>
class Hash
{
   //     TEMPLATE_DEFINES
    public:
        Hash();//K,D);
        Hash(K,D); //,K,D);
        K &GetKey(){ return Key; }
        D &GetData(){ return Data; }
        int GetStatus(){ return status;}
        void SetStatus(TYPES stat){ status=stat;}
        bool operator=(Hash<K,D>*);
    	bool operator=(D in);

        bool operator==(const Hash<K,D> &) const;
//    	bool operator==( in);


    
	    void SetData(D &iData){ Data=iData; }
        void SetKey(K &iKey){ Key=iKey; }
    
        int GetHeight(){ return height; }
	    int SetHeight(int ih){ return (height=ih); }

    	int GetBalanceFactor(){ return balanceFactor; }
    	int SetBalanceFactor(int ih){ return (balanceFactor=ih); }
	
        void IncrementCount(){ count++; }
    	int GetCount(){ return count; }
    protected:
    	int count;
    	int height;
    	int balanceFactor;
    	D Data;
    	K Key;
        TYPES status;
};

template <class K, class D>
Hash<K,D>::Hash() //K,D
{
    status = EMPTY;
}

template <class K, class D>
Hash<K,D>::Hash(K ikey,D idata)//;,K nullkey,D nulldata)
{
    Key = ikey;
    Data= idata;
    status = INTIALIZED;
}


template <class K, class D>
bool
Hash<K,D>::operator=(Hash<K,D>* iH)
{
    this->Key = iH->Key;
    this->Data = iH->Data;
    this->status = FULL;
    return true;
}

template <class K, class D>
bool
Hash<K,D>::operator=(D in)
{
    
    this->Data = in;
    this->status = FULL;
    return true;
    
}

template <class K,class D>
bool 
Hash<K,D>::operator==(const Hash<K,D> &inHash) const
{
    return ( inHash.Data == this->Data ? true : false); 
}
/*
TEMPLATE_DEFINES
void 
Hash<K,D>::operator delete( void *,Hash<K,D> &iH)
{
    iH.deleteStatus = false;
}
*/
#endif
