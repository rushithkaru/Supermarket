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

bool checkStock(map<string,Product> inventory, string item);

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
    map<string,Product>  createReceipt(map<string,Product> inventory, int receiptID);
};

Cart::Cart(/* args */)
{
}

Cart::~Cart()
{
}

void Cart::createCartList(string file){
    ifstream myfile;
    myfile.open("Book2.csv");

    if (!myfile.good()){
        cout << "file error" << endl;
        exit(1);
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
    /*Instantiate discount classes. These implement a method called applyDiscount.
    Add future discount types to this method as long as they implement the IDiscount 
    interface.
    */

    IdenticalDiscount ident;
    SetDiscount setD;
    ident.applyDiscount(inventory, this->cartData);
    this->discount1Val = ident.getDiscountVal();
    setD.applyDiscount(inventory,this->cartData);
    this->discount2Val = setD.getDiscountVal();
}

map<string,Product>  Cart::createReceipt(map<string,Product> inventory, int receiptID){

    ofstream outfile;
    outfile.open("Receipts/receipt.txt");
    string txt = "RECEIPT";
    outfile << txt << " " << receiptID << endl;
    string currentItem = get<0>(cartData[0]);
    Item item = {currentItem,0.0};
    Item discountedItem = {currentItem,0.0};
    int quantity = 0;
    int discountedQty = 0;

    for (auto& row: cartData){
        if (inventory.count(get<0>(row)) > 0){
            //put in function
            if (get<0>(row) != currentItem){
                auto it = inventory.find(get<0>(row));
                Product product = it->second;
                outfile <<  item.name << " (Qty " << quantity << ")" << " - $" << item.price << "\n";
                item.price = 0.0;
                quantity = 0;
                item.name = get<0>(row);
                currentItem = get<0>(row);
                if (discountedQty > 0){
                    outfile <<  discountedItem.name << " (Qty " << discountedQty << ")" << " - ($" << discountedItem.price << ")" << "\n";

                }
                discountedQty = 0;
                discountedItem.name = get<0>(row);
                discountedItem.price = 0;
            }
            if (checkStock(inventory,get<0>(row))){
                if (get<1>(row)){
                auto it = inventory.find(get<0>(row));
                Product product = it->second;
                item.price += product.getPrice();
                quantity++;
                totalCost += product.getPrice();
                inventory.find(get<0>(row))->second.setStock(1);
            }
                else{
                    auto it = inventory.find(get<0>(row));
                    Product product = it->second;
                    discountedItem.price += product.getPrice();
                    discountedQty++;
                    inventory.find(get<0>(row))->second.setStock(1);
                }
            } 
            else{
                cout << "Insufficient stock of item: " << get<0>(row) << endl;
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

    return inventory;
}

bool checkStock(map<string,Product> inventory, string item){
    if (inventory.find(item)->second.getStock() > 0){
        return true;
    }
    return false;
}