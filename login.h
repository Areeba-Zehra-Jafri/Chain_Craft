#ifndef LOGIN_H
#define LOGIN_H

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include "wallet.h"      
#include "Cblock.h"  
#include "transaction.h" 
#include "CChain.h"

using namespace std;

class Login {
private:
    string filename;  
    map<string, string> users; 
    vector<Wallet*> wallets;  
    Blockchain myBlockchain; 

    bool fileExists();  
    void loadUsers();   

public:
    Login(const string& file); 
    ~Login();               

    void signup();      
    Wallet* login();      
    void performTransaction(Wallet* sender); 
    void mineBlock();    
    void viewChain();     

    void userMenu(Wallet* userWallet);  
    void displayMenu();    
};

#endif // LOGIN_H
