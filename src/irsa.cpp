#include "irsa.hh"
#include <cstdlib>


bool isrp(int a, int b)
{
	if(a==1||b==1)  return true;
	while(1)
    {
		int t = a%b;
		if(t == 0) 
        {
            break;
        }
		else
        {
			a = b;
			b = t;
		}
	}
	if(b>1)	return false;
	else return true;
}

void IRSA::init(uint64_t p, uint64_t q)
{
    this->p = p;
    this->q = q;
    n = p*q;
    auto eual = (p-1)*(q-1);
    while(true) {
        auto num = random() % eual;
        for(;num < eual; num++) {
            if(isrp(num, eual))
                break;
        }
        if(num != eual) {
            e = num;
            break;
        }
    }
    for(int i = 0; ;i++) {
        if((eual * i + 1) % e == 0) {
            d = (eual * i + 1) / e;
            break;
        }
            
    }
}

uint64_t IRSA::encryptByPublic(const uint64_t &m)
{
    uint64_t res = 1;
    for(uint64_t i = 0; i < e; i++) {
        res = res * m % n;
    }
    return res;
}

uint64_t IRSA::decryptByPrivate(const uint64_t &c)
{
    uint64_t res = 1;
    for(uint64_t i = 0; i < d; i++) {
        res = res * c % n;
    }
    return res;
}

uint64_t IRSA::encryptByPrivate(const uint64_t &m)
{
    uint64_t res = 1;
    for(uint64_t i = 0; i < d; i++) {
        res = res * m % n;
    }
    return res;
}

uint64_t IRSA::decryptByPublic(const uint64_t &c)
{
    uint64_t res = 1;
    for(uint64_t i = 0; i < e; i++) {
        res = res * c % n;
    }
    return res;
}