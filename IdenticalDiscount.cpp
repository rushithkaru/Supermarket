#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>

//Class to implement discount type 1.
class IdenticalDiscount : public IDiscount
{
private:
    //Total discount cost for this type of discount
    float discountTotal = 0.0;
public:
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override;
    float getDiscountVal();
};

/* applyDiscount() implementation
* For every 3 identical purchases the third is free
* @param inventory (map of inventory data)
* @param cartData (vector of the items in the cart)
*/
void IdenticalDiscount::applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) {
        
        string current = get<0>(cartData[0]);
        int repeat_counter = 0;

        for (auto &cartItem : cartData){
            string item = get<0>(cartItem);
            //Check if item is in the inventory
            if (inventory.count(item) > 0){
                //check for repeats 
                if (item != current){
                    current = item;
                    repeat_counter = 1;
                }
                else {
                    repeat_counter++;
                }
                //Check if item is eligible for discount
                if (repeat_counter > 0 && repeat_counter % 3 == 0){
                    get<1>(cartItem) = false;
                    Product product = inventory.find(item)->second;
                    discountTotal +=  product.getPrice();
                }
            }
            else {
                cout << "Not available:  " << endl;
            }  
        }
}

//Getter for discount value.
float IdenticalDiscount::getDiscountVal(){
    return discountTotal;
}