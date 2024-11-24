
#include "CChain.h"
#include "CBlock.h"
#include "Transaction.h"
#include "Wallet.h"
#include "RSA.h"
#include "login.h" // Include login header
#include <iostream>
#include <vector>
#include <string>

using namespace std;



int main() {
    // Display login system and allow user to access the blockchain simulation
    Login loginSystem("users.txt"); // Initialize login system with file name
    loginSystem.displayMenu();      // Display the main menu

    return 0;
}
