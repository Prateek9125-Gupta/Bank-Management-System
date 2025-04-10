#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <regex>
using namespace std;

class Bank {
    int accNo;
    char name[25];
    char Fname[25];
    char cnic[25];
    char P_no[25];
    char email[25];
    char password[25];
    float amount;
    int newAmount;
    fstream file, file1, transFile;

public:
    int search;
    char searchPass[25];

    void createAccount();
    void depositAmount();
    void withdrawAmount();
    void checkInfo();
    void adminPanel();
    void showTransactionHistory();
    bool validateCNIC(const char* cnic);
    bool validateEmail(const char* email);
    bool login();
};

int main() {
    Bank obj;
    char choice;
    bool running = true;

    while (running) {
        cout<<"\n\n\n\t\t  ......:::Bahram's Bank:::......";
        cout<<"\n\t\t::    press 1 to Login Account   ::";
        cout<<"\n\t\t::    press 2 to Create Account  ::";
        cout<<"\n\t\t::    press 3 to Admin Panel     ::";
        cout<<"\n\t\t::    press 0 to Exit            ::";
        cout<<"\n\t\t:: ............................. ::\n\t\t\t\t";
        cin>> choice;

        switch (choice) {
            case '1': {
                if (!obj.login()) {
                    cout << "\nInvalid account number or password!\n";
                    break;
                }

                while (true) {
                    cout << "\n\n\t\t.......:::Bahram's Bank Menu:::.......";
                    cout << "\n\t\t::      press 1 to Deposit Amount     ::";
                    cout << "\n\t\t::      press 2 to Withdraw Amount    ::";
                    cout << "\n\t\t::      press 3 to Check Info         ::";
                    cout << "\n\t\t::      press 4 to Transaction History::";
                    cout << "\n\t\t::      press 0 to Back to Main Menu  ::";
                    cout << "\n\t\t:: .................................  ::\n\t\t\t\t";
                    cout << "\n\tEnter choice: ";
                    cin >> choice;

                    if (choice == '0') break;

                    switch (choice) {
                        case '1': obj.depositAmount(); break;
                        case '2': obj.withdrawAmount(); break;
                        case '3': obj.checkInfo(); break;
                        case '4': obj.showTransactionHistory(); break;
                        default: cout << "\nInvalid Choice!\n"; break;
                    }
                }
                break;
            }

            case '2':
                obj.createAccount();
                break;

            case '3':
                obj.adminPanel();
                break;

            case '0':
                running = false;
                cout << "\nThank you for using Bahram's Bank!\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
                break;
        }
    }

    return 0;
}

bool Bank::login() {
    cout << "\nEnter your account number :: ";
    cin >> search;
    cout << "Enter your password       :: ";
    cin >> searchPass;

    file.open("data.txt", ios::in);
    if (!file) {
        cout << "Unable to open data file.\n";
        return false;
    }

    bool authenticated = false;
    while (file >> accNo >> name >> Fname >> cnic >> P_no >> email >> password >> amount) {
        if (accNo == search && strcmp(password, searchPass) == 0) {
            authenticated = true;
            break;
        }
    }
    file.close();

    return authenticated;
}

void Bank::createAccount() {
    srand(time(0));
    accNo = rand() % 1000000 + 100000;

    cout<<"Enter Your Name  :: "; cin >> name;
    cout<<"Enter Your Father Name  :: "; cin >> Fname;

    do {
        cout<<"Enter Your CNIC (xxxxx-xxxxxxx-x) :: ";
        cin >> cnic;
    } while (!validateCNIC(cnic));

    cout<<"Enter Your Phone Number :: "; cin >> P_no;

    do {
        cout<<"Enter Your Email  :: ";
        cin >> email;
    } while (!validateEmail(email));

    cout<<"Set Your Password  :: "; cin >> password;
    cout<<"Enter Initial Amount  :: "; cin >> amount;

    cout<<"\nYour Account Number is: " << accNo << " (Please save it!)\n\n";

    file.open("data.txt", ios::out | ios::app);
    file << accNo << "\t" << name << "\t" << Fname << "\t" << cnic << "\t" << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
    file.close();
}

void Bank::depositAmount() {
    cout<<"Enter amount to deposit :: ";
    cin>>newAmount;

    bool found = false;
    file.open("data.txt", ios::in);
    file1.open("data1.txt", ios::out);

    while (file >> accNo >> name >> Fname >> cnic >> P_no >> email >> password >> amount) {
        if (accNo == search) {
            found = true;
            amount += newAmount;
            cout << "\nUpdated Balance :: " << amount << endl;
            transFile.open((to_string(accNo) + "_trans.txt").c_str(), ios::app);
            transFile << "Deposited: " << newAmount << "\tNew Balance: " << amount << endl;
            transFile.close();
        }
        file1 << accNo << "\t" << name << "\t" << Fname << "\t" << cnic << "\t" << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
    }

    file.close(); file1.close();
    remove("data.txt"); rename("data1.txt", "data.txt");

    if (!found) cout << "\nAccount not found!" << endl;
}

void Bank::withdrawAmount() {
    cout<<"Enter amount to withdraw :: ";
    cin>>newAmount;

    bool found = false;
    file.open("data.txt", ios::in);
    file1.open("data1.txt", ios::out);

    while (file >> accNo >> name >> Fname >> cnic >> P_no >> email >> password >> amount) {
        if (accNo == search) {
            found = true;
            if (newAmount > amount) {
                cout << "\nInsufficient balance!" << endl;
            } else {
                amount -= newAmount;
                cout << "\nUpdated Balance :: " << amount << endl;
                transFile.open((to_string(accNo) + "_trans.txt").c_str(), ios::app);
                transFile << "Withdrawn: " << newAmount << "\tNew Balance: " << amount << endl;
                transFile.close();
            }
        }
        file1 << accNo << "\t" << name << "\t" << Fname << "\t" << cnic << "\t" << P_no << "\t" << email << "\t" << password << "\t" << amount << endl;
    }

    file.close(); file1.close();
    remove("data.txt"); rename("data1.txt", "data.txt");

    if (!found) cout << "\nAccount not found!" << endl;
}

void Bank::checkInfo() {
    bool found = false;
    file.open("data.txt", ios::in);
    while (file >> accNo >> name >> Fname >> cnic >> P_no >> email >> password >> amount) {
        if (accNo == search) {
            found = true;
            cout << "\n---------------------------\n";
            cout << "Account Number   :: " << accNo << endl;
            cout << "Name             :: " << name << endl;
            cout << "Father Name      :: " << Fname << endl;
            cout << "CNIC             :: " << cnic << endl;
            cout << "Phone Number     :: " << P_no << endl;
            cout << "Email            :: " << email << endl;
            cout << "Current Balance  :: " << amount << endl;
            cout << "---------------------------\n";
        }
    }
    file.close();
    if (!found) cout << "\nAccount not found!" << endl;
}

void Bank::showTransactionHistory() {
    string line;
    transFile.open((to_string(search) + "_trans.txt").c_str(), ios::in);
    if (!transFile) {
        cout << "\nNo transaction history available.\n";
        return;
    }
    cout << "\nTransaction History:\n";
    while (getline(transFile, line)) {
        cout << line << endl;
    }
    transFile.close();
}

void Bank::adminPanel() {
    cout << "\n::: Admin Panel - All Accounts :::\n";
    file.open("data.txt", ios::in);
    if (!file) {
        cout << "\nNo data found.\n";
        return;
    }
    while (file >> accNo >> name >> Fname >> cnic >> P_no >> email >> password >> amount) {
        cout << "\n---------------------------\n";
        cout << "Account Number :: " << accNo << endl;
        cout << "Name           :: " << name << endl;
        cout << "CNIC           :: " << cnic << endl;
        cout << "Email          :: " << email << endl;
        cout << "Phone Number   :: " << P_no << endl;
        cout << "Balance        :: " << amount << endl;
        cout << "---------------------------\n";
    }
    file.close();
}

bool Bank::validateCNIC(const char* cnic) {
    regex cnicPattern("^[0-9]{5}-[0-9]{7}-[0-9]$");
    if (!regex_match(cnic, cnicPattern)) {
        cout << "Invalid CNIC format. Correct format: 12345-1234567-1\n";
        return false;
    }
    return true;
}

bool Bank::validateEmail(const char* email) {
    regex emailPattern("^[\\w.-]+@[a-zA-Z_]+?\\.[a-zA-Z]{2,3}$");
    if (!regex_match(email, emailPattern)) {
        cout << "Invalid email format. Try again.\n";
        return false;
    }
    return true;
}
