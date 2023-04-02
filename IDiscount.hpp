#ifndef IDISCOUNT_H
#define IDISCOUNT_H
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
class IDiscount
{

public:
    virtual void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) = 0;
};

#endif