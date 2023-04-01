#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>

class SetDiscount : public IDiscount
{
public:
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool> > cartData) override {
        cout << "dsjc" << endl;
    }
};


