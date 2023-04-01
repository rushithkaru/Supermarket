#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
class IdenticalDiscount : public IDiscount
{
public:
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override {
        
        string current = get<0>(cartData[0]);
        int repeat_counter = 0;

        for (auto &cartItem : cartData){
            if (inventory.count(get<0>(cartItem)) > 0){
                if (get<0>(cartItem) != current){
                    current = get<0>(cartItem);
                    repeat_counter = 1;
                }
                else {
                    repeat_counter++;
                }
                if (repeat_counter > 0 && repeat_counter % 3 == 0){
                    get<1>(cartItem) = 0;
                }
            }
            else {
                cout << "Not available:  " << endl;
            }  
        }
    }
};



