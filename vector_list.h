#ifndef VECTORLIST_H
#define VECTORLIST_h

#include <list>


// many data structures will contain a key
class vector_list
{
public:
	
	vector_list();
	~vector_list();

	void insertValue(NAMETYPE &, VALUETYPE &);

	void removeValue(NAMETYPE &);
	
	VALUETYPE &getData(NAMETYPE &);

	void print_value(char* aName);
	
	void destroyList();
private:
	// there will be separate 

	template <class VALUETYPE,class NAMETYPE>
	class vecs
	{
		protected:
		vector<VALUETYPE> keyname;
		vector<NAMETYPE> keyvalue;
	};

	void sort_function();
	list<vecs> variableList;
	bool isSorted;
};

#endif

