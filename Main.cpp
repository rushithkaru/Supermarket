#include <iostream>
#include <fstream>
using namespace std;
#include "Market.cpp"


int main(int argc, char const *argv[])
{
    Market market;
    market.createInventory("Book1.csv");
    

    return 0;
}