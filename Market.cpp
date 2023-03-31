#include <iostream>
#include <fstream>
using namespace std;
#include "Product.cpp"
#include <map>
#include <string>
#include <sstream>
class Market
{
private:
    /* data */
    map<string,Product> inventory;

public:
    Market(/* args */);
    ~Market();
    void createInventory(string file);
    void createReceipt();
};

Market::Market(/* args */)
{
}

Market::~Market()
{
}

void Market::createInventory(string file){
    ifstream myfile;
    myfile.open(file);

    if (!myfile.good()){
        cout << "file error" << endl;
        return;
    }

    string header,line;
    getline(myfile,header,'\n');

    while (getline(myfile, line)){
        string category,item,price,stock;

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

        cout << "Cat " << inventory.at(item).getCategory() << endl;
        cout << "Item " << inventory.at(item).getItem() << endl;
        cout << "Price " << inventory.at(item).getPrice() << endl;
        cout << "Stock " << inventory.at(item).getStock() << endl;

    }

}

