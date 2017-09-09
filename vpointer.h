#ifndef V_POINTER_H
#define V_POINTER_H


#include <vector>
#include "avl/AVLTree.h"

using namespace std;

template <class D>
class VPOINTER
{

    
    public:
     //   bool resizeHash(unsigned int);
        VPOINTER(D);
        VPOINTER();
        D &GetStruct(){ return data; }
        vector< VPOINTER< D > > &GetLink(){ return link; }

        D data;
        AVLTree< D > linkTree;
    private:
//        vector< VPOINTER< D > > link;      

};

template <class D>
VPOINTER<D>::VPOINTER()
{
//    link.empty();
}

template <class D>
VPOINTER<D>::VPOINTER(D iData)
{
    data=iData;
//    link.empty();
}
/*
template<class D>
bool
VPOINTER<D>::resizeHash(unsigned int size)
{
    if (link.size() > 0)
         return false;
    else
    {
        link.resize(size);
        return true;
    }
}


*/

#endif
