#include <iostream>
#include <fstream>
using namespace std;
#include "Market.cpp"
#include "Cart.cpp"

#include "Product.cpp"

int main(int argc, char const *argv[])
{
    Market market;
    market.createInventory("Book1.csv");
    Cart cart;
    //market.printInventory();
    cart.createCartList("Book2.csv");
    //cart.printCart();
    cart.applyDiscounts(market.getInventory());
    map<string,Product> updatedInventory = cart.createReceipt(market.getInventory(),1);
    //cart.printCart();
    cout << "\n";
    market.setInventory(updatedInventory);
    //market.printInventory();
    return 0;
}