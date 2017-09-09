#ifndef DBASE_QUERY
#define DBASE_QUERY

#include <iostream>
#include <string>
#include <sstream>
using namespace std;
class Query
{
    template <class D>
    friend string &operator<<(string &,D data);
    friend ostream &operator<<(ostream &,Query &);
    public:
        Query(){ clear(); }
        void clear(){ my_query=""; }
        string &str(){ return my_query; }
    protected:
        string my_query;        
};

template <class D>
Query &operator<<(Query &me, D data)
{
    stringstream st;
    st << data;
    me.str()+=st.str();
    return me;
}



ostream 
&operator<<(ostream &outstream,Query &d)
{
    outstream << d.my_query;
}

#endif

