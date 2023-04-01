#include <iostream>
#include <fstream>
using namespace std;
#include "Market.cpp"
#include "Cart.cpp"


int main(int argc, char const *argv[])
{
    Market market;
    market.createInventory("Book1.csv");
    Cart cart;
    //market.printInventory();
    cart.createCartList("Book2.csv");
    //cart.printCart();
    cart.applyDiscounts(market.getInventory());
    cart.createReceipt(market.getInventory(),0);
    cart.printCart();
    return 0;
}