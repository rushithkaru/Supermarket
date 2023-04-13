#include <iostream>
#include <fstream>
using namespace std;
#include <map>
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
    * Stored as a tuple (string: name of item, bool: check if discount applied).
    */
    vector<tuple<string, bool>> cartData;
    // Discount totals and receipt total
    float discount1Val;
    float discount2Val;
    float totalCost = 0.0;
public: 
    bool createCartList(string file,map<string,Product> inventory);
    void printCart();
    vector<tuple<string, bool> > getCart();
    void applyDiscounts(map<string,Product> inventory);
    map<string,Product>  createReceipt(map<string,Product> inventory, int receiptID);
};

/*
* Populates cart information. 
* Data is taken from a csv file. 
* 
* @param file: The name of the csv file
*/
bool Cart::createCartList(string file , map<string,Product> inventory){
    ifstream myfile;
    myfile.open(file);

    if (!myfile.good()){
        cout << "file error" << endl;
        return false;
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
            * add it to cartData.
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
        cerr << "Empty Cart:" << endl;
        return false;
    }
    return true;
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
* @param inventory data
*/
void Cart::applyDiscounts(map<string,Product> inventory){
    /* Use relevant discount classes here.
    */
    IdenticalDiscount identicalDiscount;
    SetDiscount setDiscount;

    identicalDiscount.applyDiscount(inventory, this->cartData);
    this->discount1Val = identicalDiscount.getDiscountVal();
    setDiscount.applyDiscount(inventory,this->cartData);
    this->discount2Val = setDiscount.getDiscountVal();

}

/* Create and write to receipt. 
* Outputs to the .txt file in the following format.
* Item (Qty x) - $total_price
*
* @param: inventory data
* @param: receiptID (for multiple carts)
* @return: updated inventory data
*/
map<string,Product>  Cart::createReceipt(map<string,Product> inventory, int receiptID){
    ofstream outfile;
    string receiptFile = "Receipts/receipt" + to_string(receiptID) + ".txt";
    outfile.open(receiptFile);
    string txt = "RECEIPT";
    outfile << txt << " " << receiptID << endl;
    string currentItem = get<0>(cartData[0]);
    Item item = {currentItem,0.0};
    Item discountedItem = {currentItem,0.0};
    int quantity = 0;
    int discountedQty = 0;

    //iterate through cart and print to receipt
    for (auto& row: cartData){
        string currentRow = get<0>(row);
        if (inventory.count(currentRow) > 0){
            // Output current cart item data to receipt
            if (currentRow != currentItem){
                Product product = inventory.find(currentRow)->second;
                outfile <<  item.name << " (Qty " << quantity << ")" << " - $" << item.price << "\n";
                item.price = 0.0;
                quantity = 0;
                item.name = currentRow;
                currentItem = currentRow;
                //add any discount data to the receipt
                if (discountedQty > 0){
                    outfile <<  discountedItem.name << " (Discount Qty " << discountedQty << ")" 
                    << " - ($" << discountedItem.price << ")" << "\n";
                }
                discountedQty = 0;
                discountedItem.name = currentRow;
                discountedItem.price = 0;
            }
            //Check if item is instock
            if (checkStock(inventory,currentItem)){
                //Check if item is not discounted.
                if (get<1>(row)){
                    Product product = inventory.find(currentRow)->second;
                    item.price += product.getPrice();
                    quantity++;
                    totalCost += product.getPrice();
                    inventory.find(currentRow)->second.setStock(1);
                }
                //If discounted update approriate variables.
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
    //print last row of receipt
    Product product = inventory.find(get<0>(cartData.back()))->second;
    outfile <<  item.name << " (Qty " << quantity << ")" << " - $" << item.price << "\n";
    if (discountedQty > 0){
        outfile <<  discountedItem.name << " (Discount Qty " << discountedQty << ")" 
        << " - ($" << discountedItem.price << ")" << "\n";
    }
    //write the discount values and total cost to receipt.
    outfile << "\n";
    outfile << "Discount 1 Total - $ " << "(" << this->discount1Val << ")" << "\n";
    outfile << "Discount 2 Total - $ " << "(" << this->discount2Val << ")" << "\n";
    outfile << "Grand Total - $ " << this->totalCost << "\n";

    outfile.close();

    cout << "Receipt " << receiptID << " created successfully" << endl;

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