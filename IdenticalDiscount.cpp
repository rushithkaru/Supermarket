#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
class IdenticalDiscount : public IDiscount
{
private:
    float discountTotal = 0.0;
public:
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override;
    float getDiscountVal();
};

void IdenticalDiscount::applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) {
        
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
                    auto it = inventory.find(get<0>(cartItem));
                    Product product = it->second;
                    discountTotal +=  product.getPrice();
                }
            }
            else {
                cout << "Not available:  " << endl;
            }  
        }
}

float IdenticalDiscount::getDiscountVal(){
    return discountTotal;
}