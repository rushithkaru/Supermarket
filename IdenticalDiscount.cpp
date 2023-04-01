#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
class IdenticalDiscount : public IDiscount
{
public:
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> cartData) override {
        cout << "test " << endl;
        for (auto cartItem : cartData){
            
            if (get<0>(cartItem) == "apple") {
                get<1>(cartItem) = 0;
            }
            cout << get<0>(cartItem) << " " << get<1>(cartItem) << endl;
        }
    }
};



