#include <iostream>
using namespace std;
#ifndef PRODUCT_H
#define PRODUCT_H
#define NUM_CATEGORIES 5


//Class for representing a product available in the supermarket
class Product
{
private:
    //category refers to the set each product belongs to
    int category;
    string item;
    float price;
    int stock;
    
public:
    Product(int category,string item,float price,int stock);
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

//getters and setters
struct Item {
        string name;
        float price;
        int index;
    };
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