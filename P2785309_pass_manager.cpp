#include <iostream> 
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

class PasswordManager {
private:
    string Username;
    string Password;
    string MasterPassword;
    vector<pair<string,string>> Credentials; //This vector function combines and pairs two strings (the username and password) and calls it 'Credentials'

    string RandomPasswordGenerator(int length) {
        const string validcharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?"; //These are all the valid characters which the generator will use to create a random password
        string password = "";
        srand(time(nullptr));
        for (int i = 0; i < length; ++i) {
            int index = rand() % validcharacters.length();
            password += validcharacters[index];
        }
        return password;
    }

    //Caeser encryption is used here to encrypt the randomly generated password
    string caeserencryption(string Password) {
        int caeser = 6;
        std::string EncryptedPassword = Password;
        for (int i = 0; i < Password.size(); i++) {
            char ch = Password[i];
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = (ch - base + caeser) % 26 + base;
            } else if (ch >= '!' && ch <= '@') {
                ch = (ch - '!' + caeser) % ('@' - '!' + 1) + '!';
            }
            EncryptedPassword[i] = ch;
        }
        return EncryptedPassword;
        }

    //caeser decryption is used to decrypt the password
    string caeserdecryption(string EncryptedPassword) {
        int caeser = 6;
        std::string DecryptedPassword = EncryptedPassword;
        for (int i = 0; i < EncryptedPassword.size(); i++) {
            char ch = EncryptedPassword[i];
            if (isalpha(ch)) {
                char base = isupper(ch) ? 'A' : 'a';
                ch = (ch - base - caeser + 26) % 26 + base;
            } else if (ch >= '!' && ch <= '@') {
                ch = (ch - '!' - caeser + ('@' - '!' + 1) * 2) % ('@' - '!' + 1) + '!';
            }
            DecryptedPassword[i] = ch;
        }
        return DecryptedPassword;
    }



public:
    void SetMasterPassword(string mUserInput) {
        MasterPassword = mUserInput;
    }
    
    //The first function allows the user to add a username to which the program will generate a random password for it
    void AddCredentials() {
        cout << "Please enter a username" << endl;
        cin >> Username;
        //First the program checks to see if the username entered already exists. If it does, it will ask the user if they want to replace the password
        for (const auto& cred : Credentials) {
        if (cred.first == Username) {
            cout << "Username already exists. Do you want to replace the existing password? (yes/no): ";
            string replaceChoice;
            cin >> replaceChoice;
            if (replaceChoice == "yes") {
                Password = RandomPasswordGenerator(16);
                string EncryptedPassword = caeserencryption(Password);
                // Update existing credential with new password
                for (auto& cred : Credentials) {
                    if (cred.first == Username) {
                        cred.second = EncryptedPassword;
                        break;
                    }
                }
                cout << "A new password has been generated and encrypted for this username." << endl;
                return;
            } else {
                cout << "Operation canceled." << endl;
                return;
            }
        }
    }
        //If the username has not been taken, it will continue this code and generate a password
        Password = RandomPasswordGenerator(16);
        //cout << "Your password is " << Password << endl;
        string EncryptedPassword = caeserencryption(Password);
        //cout << "Your encrypted password is " << EncryptedPassword << endl;
        string DecryptedPassword = caeserdecryption(EncryptedPassword);
        //cout << "Your decrypted password is " << DecryptedPassword << endl;
        Credentials.push_back(make_pair(Username,Password)); //This will turn the username the user entered and the new generated password that has been encrypted into one element which is stored in the vector 'Credentials'
        //The 'push_back' function allows the vector to create one more space for an element to be added into it
        cout << "A random password has been generated and encrypted for this username" << endl;
    }


    //This function allows the user to retrieve the credentials stored in the file. It shows the decrypted password
    void GetCredentials() {
    bool continueSearch = true;
    while (continueSearch) {
        cout << "\nEnter username to retrieve credentials (type 'quit' to exit): ";
        string searchUsername;
        cin >> searchUsername;

        //If the user types quit then it will stop the function and loop back to the menu
        if (searchUsername == "quit") {
            break;
        }

        //If the username is found it will print it with the decrypted password
        bool found = false;
        for (const auto& cred : Credentials) {
            if (cred.first == searchUsername) {
                cout << "Username: " << cred.first << endl;
                cout << "Decrypted Password: " << caeserdecryption(cred.second) << endl;
                found = true;
                break;
            }
        }

        //If the username doesnt match, it will ask the user if they want to search it again
        if (!found) {
            cout << "Credentials for username '" << searchUsername << "' not found. Please try again." << endl;
        } else {
            cout << "\nDo you want to search for another username? (yes/no): ";
            string choice;
            cin >> choice;
            if (choice != "yes") {
                continueSearch = false;
                cout << "\nExiting GetCredentials function." << endl;
                }
            }
        }
    }

   // This function deletes credentials that are saved in the file
    void DeleteCredentials() {
    // Asks the user to enter the username for which credentials need to be deleted.
    cout << "Enter username to delete credentials: ";
    string Username;
    cin >> Username;

    // I used an iterator here to find elements in the vector as the credentials are stored in the vector
    auto it = Credentials.begin();
    bool deleted = false;

    // Looks through the vector to find and match the username with its credentials
    while (it != Credentials.end()) {
        // If the username matches, delete the credentials associated with it.
        if (it->first == Username) {
            it = Credentials.erase(it);
            deleted = true;
        } else {
            ++it;
        }
    }

    // If credentials were deleted, update the credentials file.
    if (deleted) {
        ofstream Replace("Creds.txt");
        if (Replace.is_open()) {
            for (const auto &Cred : Credentials) {
                Replace << Cred.first << " " << Cred.second << endl;
            }
            // Close the file.
            Replace.close();
            cout << "The credentials for username '" << Username << "' have been deleted successfully." << endl;
            cout << "Credentials file updated." << endl;
        }
    } else {
        // If credentials were not found for the entered username, tell the user.
        cout << "Credentials for username '" << Username << "' not found." << endl;
    }
}

    //This is the save function which saves the credentials into a file
    void SaveToFile(){
        ofstream CredFile("Creds.txt",ios::app); //opens the file in append mode and allows the program to enter data into the file. This will allow it to save the entered credentials
        string CredUser,CredPass;
        bool ExistingUser = false;
        ifstream CredFileCheck("Creds.txt");
        for (const auto& Cred : Credentials) {
            while(CredFileCheck >> CredUser >> CredPass){ //This while condition is used to check if the credential pair is valid and reads to data. It stores the credentials into 2 variables (CredUser and CredPass)
                if(CredUser == Cred.first) {
                ExistingUser = true;
                break;
            }
            }
            if(!ExistingUser) {
                CredFile << Cred.first << " - " << Cred.second << endl;
            }
        }
        Credentials.clear();
        CredFile.close();
        cout << "Credentials have been saved to file successfully." << endl;
    }
    
    
    //This is the load function which loads the file in which the credentials are stored
    void LoadFromFile(){
        ifstream CredFile("Creds.txt");
    if (!CredFile.is_open()) {
        cout << "Error: Unable to open file for reading." << endl;
        return;
    }

    Credentials.clear();

    string UsernameInFile, PasswordInFile;
    while (CredFile >> UsernameInFile >> PasswordInFile) {
        Credentials.push_back(make_pair(UsernameInFile, PasswordInFile));
    }

    CredFile.close();
    cout << "Credentials have been loaded from file successfully." << endl;
    }

    };

    int main() {
    PasswordManager Admin;
    string mUserInput;
    cout << "Set master password" << endl;
    cin >> mUserInput; 
    Admin.SetMasterPassword(mUserInput);
    string Verify;
    cout << "Enter the master password" << endl;
    cin >> Verify;
    if (Verify == mUserInput) {
        char menu;
        while (menu != '6') {
            cout << "\n=========Password Manager=========" << endl; //This is the main menu where the user will choose from 6 options
            cout << "Please choose one of the 5 options" << endl;    
            cout << "1. Add Credentials" << endl;
            cout << "2. Get Credentials" << endl;   
            cout << "3. Delete Credentials" << endl;
            cout << "4. Save To File" << endl;
            cout << "5. Load From File" << endl;
            cout << "6. Closing the program, goodbye!" << endl;
            cin >> menu;

            switch(menu) {
                case '1': {
                    Admin.AddCredentials();
                    break;
                }
                case '2': {
                    Admin.GetCredentials();
                    break;
                }
                case '3': {
                    Admin.DeleteCredentials();
                    break;
                }
                case '4': {
                    Admin.SaveToFile();
                    break;
                }
                case '5': {
                    Admin.LoadFromFile();
                    break;
                }
                case '6': {
                    cout << "Exiting the program..." << endl;
                    return 0; 
                }
                default: {
                    cout << "Invalid Option, Please choose an option between 1 and 5" << endl; //Condtion so that if the user chooses anything not between 1 and 6, it tells the user to try again
                }
            }
        }
    } else {
        cout << "The master password is incorrect." << endl;
    }

}