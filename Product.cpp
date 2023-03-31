#include <iostream>
using namespace std;

class Product
{
private:
    /* data */
    int category;
    string item;
    float price;
    int stock;
    
public:
    Product(int category,string item,float price,int stock);
    ~Product();
    int getCategory();
    string getItem();
    float getPrice();
    int getStock();
};

Product::Product(int category,string item,float price,int stock)
{
    this->category = category;
    this->item = item;
    this->price = price;
    this->stock = stock;
}

Product::~Product()
{
}

int Product::getCategory(){
    return this->category;
}

string Product::getItem(){
    return this->item;
}

float Product::getPrice(){
    return this->price;
}

int Product::getStock(){
    return this->stock;
}
