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

/* Class to represent and perform actions
* for a cart(items to be bought).
*/
class Cart
{
private:
    /* Vector to store cart data.
    * Stored as tuple (string: name of item, bool: check if discount applied).
    */
    vector<tuple<string, bool>> cartData;
    // Discount totals and receipt total
    float discount1Val;
    float discount2Val;
    float totalCost = 0.0;
public: 
    void createCartList(string file,map<string,Product> inventory);
    void printCart();
    vector<tuple<string, bool> > getCart();
    void applyDiscounts(map<string,Product> inventory);
    map<string,Product>  createReceipt(map<string,Product> inventory, int receiptID);
};

/*
* Populates cart information. 
* Data is taken from a csv file. 
* 
* @param file The name of the csv file
*/
void Cart::createCartList(string file , map<string,Product> inventory){
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
        string item,qty;

        //Handle incorrect input 
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
        //Handle incorrect input 
        try {
            quantity = stoi(qty);
        } 
        catch (const exception& e) {
            cerr << "Error converting input: " << e.what() << endl;
            continue; 
        }

        for (int i = 0 ; i < quantity ; i++){
            /*Check if the item is in the inventory and
            * add to vector.
            */
            if (inventory.count(item) > 0){
                cartData.push_back(make_tuple(item,true));
            }
            else{
                cout << "Item : " << item << " unavailable" << endl;
            }      
        }
    }

    //Check if the cart is empty and exit if needed
    if (cartData.size() == 0){
        cerr << "Empty Cart: exiting program" << endl;
        exit(1);
    }

}

/* Method to print cart items. (For debugging)
*/
void Cart::printCart(){
    for (auto element : cartData) {
        cout << get<0>(element) << " " << get<1>(element) << endl;
    }
}

// Getter for cart
vector<tuple<string, bool>> Cart::getCart(){
    return cartData;
}

/* Method to apply all discounts on the cart.
* Methods should implement the IDiscount interface.
*
* @ param invenory data
*/
void Cart::applyDiscounts(map<string,Product> inventory){
    /* Use relevant discount classes here.
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
        string currentRow = get<0>(row);
        if (inventory.count(currentRow) > 0){
            // Out put current cart item data to receipt
            if (currentRow != currentItem){
                Product product = inventory.find(currentRow)->second;
                outfile <<  item.name << " (Qty " << quantity << ")" << " - $" << item.price << "\n";
                item.price = 0.0;
                quantity = 0;
                item.name = currentRow;
                currentItem = currentRow;
                if (discountedQty > 0){
                    outfile <<  discountedItem.name << " (Qty " << discountedQty << ")" << " - ($" << discountedItem.price << ")" << "\n";
                }
                discountedQty = 0;
                discountedItem.name = currentRow;
                discountedItem.price = 0;
            }
            //Check if item is instock
            if (checkStock(inventory,currentItem)){
                //Check if item is not discounted
                if (get<1>(row)){
                    Product product = inventory.find(currentRow)->second;
                    item.price += product.getPrice();
                    quantity++;
                    totalCost += product.getPrice();
                    inventory.find(currentRow)->second.setStock(1);
                }
                //If discounted update approriate variable
                else{
                    Product product = inventory.find(currentRow)->second;
                    discountedItem.price += product.getPrice();
                    discountedQty++;
                    inventory.find(currentRow)->second.setStock(1);
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

    //write the discount values and total cost to receipt.
    outfile << "\n";
    outfile << "Discount 1 Total - $ " << "(" << this->discount1Val << ")" << "\n";
    outfile << "Discount 2 Total - $ " << "(" << this->discount2Val << ")" << "\n";
    outfile << "Total - $ " << this->totalCost << "\n";

    outfile.close();

    return inventory;
}

/* Check if there is enough stock in the inventory for sale
*
* @ param: inventory data
* @ param: cart item
*/
bool checkStock(map<string,Product> inventory, string item){
    if (inventory.find(item)->second.getStock() > 0){
        return true;
    }
    return false;
}