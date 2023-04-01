#include <iostream>
#include <fstream>
using namespace std;
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>
#include "IdenticalDiscount.cpp"
#include "SetDiscount.cpp"
class Cart
{
private:
    /* data */
    vector<tuple<string, bool> > cartData;
public:
    Cart(/* args */);
    ~Cart();
    void createCartList(string file);
    void printCart();
    vector<tuple<string, bool> > getCart();
    void applyDiscounts(map<string,Product> inventory);
    void createReceipt(map<string,Product> inventory, int receiptID);
};

Cart::Cart(/* args */)
{
}

Cart::~Cart()
{
}

void Cart::createCartList(string file){
    ifstream myfile;
    myfile.open(file);

    if (!myfile.good()){
        cout << "file error" << endl;
        return;
    }

    string header,line;
    getline(myfile,header,'\n');

    while (getline(myfile, line)){
        string item,qty;

        try {
            stringstream ss(line);
            getline(ss,item,',');
            getline(ss,qty,',');
        } 
        catch (const exception& e) {
            cerr << "Error reading rows: " << e.what() << endl;
            continue; 
        }
        int quantity;
         try {
            quantity = stoi(qty);
        } 
        catch (const exception& e) {
            cerr << "Error converting input: " << e.what() << endl;
            continue; 
        }

        for (int i = 0 ; i < quantity ; i++){
            cartData.push_back(make_tuple(item,true));
        }
        
    }
}

void Cart::printCart(){
    for (auto element : cartData) {
        cout << get<0>(element) << " " << get<1>(element) << endl;
    }
}

vector<tuple<string, bool>> Cart::getCart(){
    return cartData;
}

void Cart::applyDiscounts(map<string,Product> inventory){
    IdenticalDiscount ident;
    SetDiscount setD;
    ident.applyDiscount(inventory,this->cartData);
    setD.applyDiscount(inventory,this->cartData);
}

void Cart::createReceipt(map<string,Product> inventory, int receiptID){

    cout << "THE RECEIPT " << endl;

}