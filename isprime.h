#ifndef ISPRIME_H
#define ISPRIME_H
        // iV represents input Value

class PrimeFunctions
{
    public:

        unsigned long NearestPrime(unsigned long iV){ return !(iV%2) ? iV+1 : iV; }

        bool IsPrime(unsigned int);

    private:
};


bool 
PrimeFunctions::IsPrime(unsigned int iV)
{
    return ((iV%2) ? true : false);
}

#endif
