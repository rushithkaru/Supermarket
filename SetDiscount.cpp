#include "IDiscount.hpp"
using namespace std;
#include "Product.cpp"
#include <map>
#include <vector>
#include <tuple>
#include <queue>

struct Item {
        string name;
        float price;
        int index;
    };

class SetDiscount : public IDiscount
{
private:
    //map of cheapest item in each category (set)
    map<int,float> cheapestItem;
    float discountVal = 0;

public:
    
    void updateCart(vector<Item>, vector<tuple<string, bool>> &cartData);
    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override;
    float getDiscountVal();

};

void SetDiscount::updateCart(vector<Item> items, vector<tuple<string, bool>> &cartData){
    if (items.size()){
        string currentItem = items.front().name;
        float currentMin = items.front().price;
        int counter = 1;
        int minIndex = 0;
        int temp = 0;
        for (int i = 1; i < items.size(); i++) {
                
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

    for (int i = 0; i < items.size(); i++) {
            std::cout << items[i].name << " (" << items[i].price << ") " << endl;
    }
}

void SetDiscount::applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData)  {

        for (int category = 0 ; category < NUM_CATEGORIES ; category++){
            vector<Item> items;
            int index = 0;
            for (const auto&  cartItem : cartData){
                
                auto it = inventory.find(get<0>(cartItem));
                Product product = it->second;

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