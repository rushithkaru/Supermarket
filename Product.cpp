#include <iostream>
using namespace std;
#ifndef PRODUCT_H
#define PRODUCT_H
#define NUM_CATEGORIES 5

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
    void setStock(int soldAmount);
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

void Product::setStock(int soldAmount){
    this->stock -= soldAmount;
}


#endif // PRODUCT_H