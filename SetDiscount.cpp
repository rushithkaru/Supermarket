#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
#include <queue>

/*Class to implement discount type 2
*/
class SetDiscount : public IDiscount
{
private:
    //Total discount cost for this type of discount
    float discountVal = 0;

public:
    
    void updateCart(vector<Item>, vector<tuple<string, bool>> &cartData);
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override;
    float getDiscountVal();

};

/* Method takes a vector of all items in the same 
* set and adds the discount to the cheapest item.
*
* @param items (vector<Item>) all eligible items in the same set (category)
* @param cartData (vector of the items in the cart)
*/
void SetDiscount::updateCart(vector<Item> items, vector<tuple<string, bool>> &cartData){
    if (items.size()){
        string currentItem = items.front().name;
        float currentMin = items.front().price;
        int counter = 1;
        int minIndex = 0;
        int temp = 0;
        for (int i = 1; i < items.size(); i++) {
                /* find if there are three items in the 
                * same set and apply discount to the 
                * cheapest item.*/
                if (items[i].name != currentItem){
                    currentItem = items[i].name;
                    counter++;
                    if (currentMin >  items[i].price){
                        currentMin = items[i].price;
                        minIndex = items[i].index;
                        //not needed
                        temp = i;
                    }
                }
                if (counter == 3){
                    get<1>(cartData[minIndex]) = false;
                    this->discountVal += items[i].price;
                    //not needed
                    items[temp].price = 0;
                    break;
                }
        }
    }
}

/*
* applyDiscount() implementation
*
* Method updates the cartData vector and sets saves the items that will be free
* based on this discount type.
*
* @param inventory (map of inventory data)
* @param cartData (vector of the items in the cart)
*/
void SetDiscount::applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData)  {

        for (int category = 0 ; category < NUM_CATEGORIES ; category++){
            vector<Item> items;
            int index = 0;
            for (const auto&  cartItem : cartData){

                Product product = inventory.find(get<0>(cartItem))->second;
                //Create vector of items in the same category
                if (product.getCategory() == category && get<1>(cartItem)){
                    Item newItem = {product.getItem() , product.getPrice(),index};
                    items.push_back(newItem);
                }
                index++;
            }

        updateCart(items,cartData);
        }       
}

float SetDiscount::getDiscountVal(){
    return discountVal;
}