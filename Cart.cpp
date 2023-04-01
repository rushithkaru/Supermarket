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
    float discount1Val;
    float discount2Val;
    float totalCost = 0.0;
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

    if (cartData.size() == 0){
        cerr << "Empty Cart: exiting program" << endl;
        exit(1);
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
    ident.applyDiscount(inventory, this->cartData);
    this->discount1Val = ident.getDiscountVal();
    setD.applyDiscount(inventory,this->cartData);
    this->discount2Val = setD.getDiscountVal();
}

void Cart::createReceipt(map<string,Product> inventory, int receiptID){

    ofstream outfile;
    outfile.open("Receipts/receipt.txt");
    string txt = "RECEIPT";
    outfile << txt << " " << receiptID << endl;

    for (auto& row: cartData){
        if (inventory.count(get<0>(row)) > 0){
            if (get<1>(row)){
                auto it = inventory.find(get<0>(row));
                Product product = it->second;
                outfile << get<0>(row) << " - " << product.getPrice() << "\n";
                totalCost += product.getPrice();
            }
            else{
                outfile << get<0>(row) << " - " << "(0.0)" << "\n";
            }
        }
        else{
            outfile << get<0>(row) << " - " << "Unavailable" << "\n";
        }
    }

    outfile << "\n";

    outfile << "Discount 1 Total - $ " << "(" << this->discount1Val << ")" << "\n";
    outfile << "Discount 2 Total - $ " << "(" << this->discount2Val << ")" << "\n";
    outfile << "Total - $ " << this->totalCost << "\n";

    outfile.close();

}