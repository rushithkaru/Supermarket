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

    

public:
    void updateCheapest(map<string,Product> inventory);

    void updateCart(vector<Item>, vector<tuple<string, bool>> &cartData);

    void applyDiscount(map<string,Product> inventory , vector<tuple<string, bool>> &cartData) override {

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
                        temp = i;
                    }
                }
                if (counter == 3){
                    get<1>(cartData[minIndex]) = false;
                    items[temp].price = 0;
                }
        }
    }

    for (int i = 1; i < items.size(); i++) {
            //std::cout << items[i].name << " (" << items[i].price << ") " << endl;

    }

}

void SetDiscount::updateCheapest(map<string,Product> inventory){
    for (const auto&  row : inventory){
        auto product = row.second;
        if (cheapestItem.count(product.getCategory()) == 0){
            cheapestItem[product.getCategory()] = product.getPrice();
        }
        else {
             if (product.getPrice() < cheapestItem[product.getCategory()] ){
                cheapestItem[product.getCategory()] = product.getPrice();
             }
        }
    }
    for (const auto&  row : cheapestItem){
        cout << row.first << " " << row.second << endl;
    }
}