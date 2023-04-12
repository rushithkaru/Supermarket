#ifndef IDISCOUNT_H
#define IDISCOUNT_H
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>

/* Interface for implementing discounts.
* All discounts need to implement applydiscount() in order to 
* make sure it is compatible with the rest of the system. 
*/
class IDiscount
{
public:
    virtual void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) = 0;
};

#endif