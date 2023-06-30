#include <iostream>
using namespace std;
#include "Market.cpp"
#include "Cart.cpp"
#include "Product.cpp"
#define  MIN_INPUT_ARGS 3

int main(int argc, char const *argv[])
{
    if (argc < MIN_INPUT_ARGS){
        cout << "Insufficient input arguments" << endl;
        cout << "You have entered " << argc << " arguments:" << endl;
        return 0;
    }
    Market market;
    market.createInventory(argv[1]);
    for (int i = 2; i < argc; ++i){
        Cart cart;
        if (!cart.createCartList(argv[i], market.getInventory())){
            continue;
        }
        cart.applyDiscounts(market.getInventory());
        map<string,Product> updatedInventory = cart.createReceipt(market.getInventory(),i-1);
        market.setInventory(updatedInventory);
    }
    //reandomco
    return 0;
}