#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <fstream>
using namespace std;
#include "Product.cpp"
#include <map>
#include <string>
#include <sstream>

//class to represent a supermarket
class Market
{
private:
    //map to store product information for quick look up.
    map<string,Product> inventory;

public:
    
    void createInventory(string file);
    void createReceipt();
    map<string,Product> getInventory();
    void printInventory();
    void setInventory(map<string,Product> inventory);
};

/*
* Populates product information for the store inventory. 
* Data is taken from a csv file. 
* 
* @param file The name of the csv file
*/
void Market::createInventory(string file){
    ifstream myfile;
    myfile.open(file);

    if (!myfile.good()){
        cout << "file error" << endl;
        exit(1);
    }

    // Handle file header
    string header,line;
    getline(myfile,header,'\n');

    // Read input file
    while (getline(myfile, line)){

        string category,item,price,stock;
        //Handle incorrect input 
        try {
            stringstream ss(line);
            getline(ss,category,',');
            getline(ss,item,',');
            getline(ss,price,',');
            getline(ss,stock,',');
        } 
        catch (const exception& e) {
            cerr << "Error reading rows: " << e.what() << endl;
            continue; 
        }
        int product_category, stock_val;
        float unit_price;
        
        //Handle incorrect input 
        try {
            product_category = stoi(category);
            stock_val = stoi(stock);
            unit_price = stof(price);
        } 
        catch (const exception& e) {
            cerr << "Error converting input: " << e.what() << endl;
            continue; 
        }

        Product prod(product_category,item,unit_price,stock_val);
        inventory.insert(pair<const std::string, Product>(item,prod));
    }

    //Check if inventory map is non empty
    if (inventory.empty()){
        cerr << "No inventory data: exiting program " << endl;
        exit(1);
    }
}

//Getter for inventory map
map<string,Product> Market::getInventory(){
        return this->inventory;
}

//Print inventory data. Useful for debugging.
void Market::printInventory(){
    for (const auto& pair : inventory) {
        cout << "Categ " << inventory.at(pair.first).getCategory() << endl;
        cout << "Item " << inventory.at(pair.first).getItem() << endl;
        cout << "Price " << inventory.at(pair.first).getPrice() << endl;
        cout << "Stock " << inventory.at(pair.first).getStock() << endl;
    }
}

//Setter for inventory.
void Market::setInventory(map<string,Product> inventory){
    this->inventory = inventory;
}

#endif  // MARKET_H