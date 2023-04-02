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
    cart.createCartList("Book2.csv", market.getInventory());
    cart.applyDiscounts(market.getInventory());
    map<string,Product> updatedInventory = cart.createReceipt(market.getInventory(),1);
    market.setInventory(updatedInventory);

    /*cout << "You have entered " << argc << " arguments:"
         << "\n";
 
    for (int i = 1; i < argc; ++i)
        cout << argv[i] << "\n";*/

    return 0;
}