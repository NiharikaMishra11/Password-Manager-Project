#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>
#include<stdio.h>
#include <cstdlib>
using namespace std;

struct Credential
{
    char website[50];
    char username[50];
    char password[50];
};

struct CredentialNode
{
    Credential credential;
    CredentialNode* left;
    CredentialNode* right;
};

struct PasswordHistory
{
    char password[50];
    PasswordHistory* next;
};
class Authenticator
{
private:
    static const int MAX_CREDENTIALS = 100;
    Credential validCredentials[MAX_CREDENTIALS];
    int numCredentials;

public:
    Authenticator()
    {
        numCredentials=0;
    }

    // Function to verify whether the provided username and password are valid

   bool verifyPassword(const char* username, const char* password) const
{
    if (username == nullptr || password == nullptr)
    {
        std::cerr << "Invalid input: username or password is nullptr.\n";
        return false;
    }

    for (int i = 0; i < numCredentials; ++i)
    {
        if (validCredentials[i].username == username && validCredentials[i].password == password)
        {
            return true; // returns true if valid
        }
    }
    return false; // otherwise false
}


    // Function to add a new valid credential to the array

    void addValidCredential( char* username, char* password)
    {
        if (numCredentials < MAX_CREDENTIALS)
        {
            strcpy(validCredentials[numCredentials].username, username);
            strcpy(validCredentials[numCredentials].password, password);
            ++numCredentials;
        }
        else
        {
            cout << "Max number of credentials reached.\n";
        }
    }
};
class TwoFactorAuthentication
{
private:
    int twoFactorCode;  // Store the generated 2FA code

public:
    TwoFactorAuthentication()

    {
        // Initialize the random seed
        srand(static_cast<unsigned int>(time(nullptr)));

        generateTwoFactorCode();
    }

    int getTwoFactorCode() const
    {
        return twoFactorCode;
    }

    void generateTwoFactorCode()
    {
        // Generate a random 6-digit number as the 2FA code
        twoFactorCode = rand() % 900000 + 100000;
    }

    bool validateTwoFactorCode(int enteredCode) const
    {
        // Validate the entered 2FA code
        return enteredCode == twoFactorCode;
    }
};

struct StackNode
{
    char data[50];
    StackNode* next;
};

class Stack // class to implement stack operations
{
public:
    StackNode* top;
    Stack()
    {
     top=NULL;
    }

    //insertion into stack
    void push(const char* data)
    {
        StackNode* newNode = new StackNode;
        strcpy(newNode->data, data);
        newNode->next = top;
        top = newNode;
    }

    // deletes and returns topmost entry from the stack
    const char* pop()
    {
        if (isEmpty())
        {
            return nullptr;
        }

        StackNode* temp = top;
        top = top->next;
        const char* poppedData = temp->data;
        delete temp;
        return poppedData;
    }

    //checks whether stack is empty
    bool isEmpty() const
    {
        return top == nullptr;
    }
};

class AccessProtector
{
private:
    char masterPassword[50];
    Stack passwordHistoryStack;
    CredentialNode* credentialsTree;
    Authenticator authenticator;
    TwoFactorAuthentication twoFactorAuthenticator;

public:
    AccessProtector()
    {
        credentialsTree=NULL;
    }

    void setMasterPassword()
    {
        cout << "Enter master password: ";
        cin >> masterPassword;
    }

    void storeCredentials()
    {
        char websiteInput[50], usernameInput[50], passwordInput[50];

        // ask user for the credentials as input
        cout << "Enter website: ";
        cin >> websiteInput;
        cout << "Enter username: ";
        cin >> usernameInput;
        cout << "Enter password: ";
        cin >> passwordInput;

        /*if (authenticator.verifyPassword(usernameInput, passwordInput))
        {*/
            credentialsTree = insertCredentials(credentialsTree, websiteInput, usernameInput, passwordInput);
            updatePasswordHistory(passwordInput);
        //}
        /*else
        {
            cout << "Invalid username or password. Credentials not stored.\n";
        }*/
    }

    CredentialNode* insertCredentials(CredentialNode* root, const char* website, const char* username, const char* password)
    {
        if (root == nullptr)
        {
            CredentialNode* newNode = new CredentialNode;
            strcpy(newNode->credential.website, website);
            strcpy(newNode->credential.username, username);
            strcpy(newNode->credential.password, password);
            newNode->left = newNode->right = nullptr;
            return newNode;
        }

        if (strcmp(website, root->credential.website) < 0)
        {
            root->left = insertCredentials(root->left, website, username, password);
        }
        else if (strcmp(website, root->credential.website) > 0)
        {
            root->right = insertCredentials(root->right, website, username, password);
        }
        else
        {
            // update the credentials if website already exists
            strcpy(root->credential.username, username);
            strcpy(root->credential.password, password);
        }

        return root;
    }

    const char* getStoredPassword()
    {
        char websiteRetrieveInput[50];
        cout << "Enter website to retrieve password: ";
        cin >> websiteRetrieveInput;

        // search whether the website name exists and return the corresponding password
        CredentialNode* result = searchCredentials(credentialsTree, websiteRetrieveInput);
        if (result != nullptr)
        {
            return result->credential.password;
        }
        else
        {
            return nullptr;
        }
    }

    // function to check whether the website and corresponding username, password exists in BST
    CredentialNode* searchCredentials(CredentialNode* root, const char* website)
    {
        while (root != nullptr && strcmp(website, root->credential.website) != 0)
        {
            root = (strcmp(website, root->credential.website) < 0) ? root->left : root->right;
        }
        return root;
    }
    void viewPasswordHistory()
    {
        // use stack operations to display password history (if exists)
        if (passwordHistoryStack.isEmpty())
        {
            cout << "Password history is empty.\n";
            return;
        }

        cout << "Password History:\n";
        StackNode* current = passwordHistoryStack.top;

        while (current != nullptr)
        {
            cout << current->data << endl;
            current = current->next;
        }
    }

    // function to update password history whenever a new password is inserted/saved
    void updatePasswordHistory(const char* newPassword)
    {
        passwordHistoryStack.push(newPassword);
    }

    void changePassword()
    {
        char websiteInput[50], currentPassword[50], newPassword[50];

        // TO get website, current password, and new password as input
        cout << "Enter website: ";
        cin >> websiteInput;
        cout << "Enter current password: ";
        cin >> currentPassword;
        cout << "Enter new password: ";
        cin >> newPassword;

        // Searches for given website
        CredentialNode* result = searchCredentials(credentialsTree, websiteInput);

        if (result != nullptr)
        {
            // Check if the stored password matches the provided password
            if (strcmp(currentPassword, result->credential.password) == 0)
            {
                // Update the password
                strcpy(result->credential.password, newPassword);
                cout << "Password updated successfully.\n";

                // update the password history
                updatePasswordHistory(newPassword);
            }
            else
            {
                cout << "Current password does not match. Password not changed.\n";
            }
        }
        else
        {
            cout << "Website not found. Password not changed.\n";
        }
    }

    void backupCredentials()
    {
        //  the previous password is saved to a file before changing it
        saveToBackupFile(passwordHistoryStack.top->data);
    }

    void saveToBackupFile(const char* data)
    {
        ofstream backupFile("backup.txt", ios::app);  // Opens file in append mode
        if (backupFile.is_open())
        {
            //save credentials to backup file
            backupFile<<data<<endl;
            backupFile.close();
            cout << "Backup successful. Previous password saved."<<endl;
        }
        else
        {
            cout << "Error: Unable to open backup file."<<endl;
        }
    }

    // function to help users generate a secure password
    const char* generateSecurePassword()
    {
        const int passwordLength = 8;

        // initialize a char array with possible characters that can be included in the password
        static const char characters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
        char* password = new char[passwordLength + 1];

        srand(static_cast<unsigned int>(time(0))); // initialize random number generator with current time

        for (int i=0; i<passwordLength; i++)
        {
            // selects a character from the array randomly
            password[i]=characters[rand() % (sizeof(characters) - 1)];
        }
        password[passwordLength]='\0';

        return password;
    }

    void autoFillLoginForm()
    {
        char websiteInput[50], usernameInput[50];

        // ask website and username as input from the user
        cout << "Enter website: ";
        cin >> websiteInput;
        cout << "Enter username: ";
        cin >> usernameInput;

        // Search for credentials for the given website
        CredentialNode* result = searchCredentials(credentialsTree, websiteInput);

        if (result != nullptr)
        {
            // Check if the stored username matches the provided username
            if (strcmp(usernameInput, result->credential.username) == 0)
            {
                cout << "Login form auto-filled. Username: " << result->credential.username
                     << ", Password: " << result->credential.password << endl;
            }
            else
            {
                cout << "Stored username does not match provided username. Login form not auto-filled.\n";
            }
        }
        else
        {
            cout << "Website not found. Login form not auto-filled.\n";
        }
    }

    void twoFactorAuthentication()
    {
        string enteredPassword;
        string enteredusername;
        string enteredCode;

        // ask the user for the regular password
        cout << "Enter your username: ";
        cin >> enteredusername;
        cout << "Enter your password: ";
        cin >> enteredPassword;
        string pswd=generateSecurePassword();
        cout<<"Generated captcha is: "<<pswd<<endl;

        // ask the user to enter the code
        cout << "Enter your 2FA code: ";
        cin >> enteredCode;

        /*authenticator.addValidCredential(enteredusername,enteredPassword);

        // authenticate both password and 2 factor authentication code
        if (authenticator.verifyPassword(enteredusername, enteredPassword) && twoFactorAuthenticator.validateTwoFactorCode(enteredCode))
        {*/
        if(pswd==enteredCode)
        {
               cout << "Authentication successful!\n";

        }
        else
        {
            cout << "Authentication failed. Invalid password or 2 factor authentication code.\n";
        }
    }
};

int main()
{
    AccessProtector accessProtector;

    int choice = -1;
    do
    {
       //system("cls");
        // menu of choices for the user
        cout << "\n===== Password Manager Menu =====\n";
        cout << "1. Set Master Password\n";
        cout << "2. Store Credentials\n";
        cout << "3. Retrieve Stored Password\n";
        cout << "4. Generate Secure Password\n";
        cout << "5. Auto-fill Login Form\n";
        cout << "6. Search Credentials\n";
        cout << "7. Trigger Security Alert\n";
        cout << "8. Backup Credentials\n";
        cout << "9. Change Password\n";
        cout << "10. Two-Factor Authentication\n";
        cout << "11. View Password History\n";
        cout << "0. Exit\n";
        cout << "===============================\n";

        //asks user for choice as input
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                accessProtector.setMasterPassword();
                break;
            case 2:
                accessProtector.storeCredentials();
                break;
            case 3:
            {
                const char* retrievedPassword = accessProtector.getStoredPassword();
                if (retrievedPassword != nullptr)
                {
                    cout << "Retrieved Password: " << retrievedPassword << endl;
                }
                else
                {
                    cout << "Website not found." << endl;
                }
                break;
            }
            case 4:
            {
                const char* securePassword = accessProtector.generateSecurePassword();
                cout << "Generated Secure Password: " << securePassword << endl;

                // Remember to deallocate the memory used for the generated password
                delete[] securePassword;
                break;
            }
            case 5:
                accessProtector.autoFillLoginForm();
                break;
            case 6:
                 //accessProtector.searchCredentials();
                cout << "Search Credentials feature not implemented.\n";
                break;
            case 7:
                cout << "Trigger Security Alert feature not implemented.\n";
                break;
            case 8:
                accessProtector.backupCredentials();
                break;
            case 9:
                accessProtector.changePassword();
                break;
            case 10:
                accessProtector.twoFactorAuthentication();
                break;
            case 11:
                accessProtector.viewPasswordHistory();
                break;
            case 0:
                cout << "Exiting the Password Manager. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
                break;
        }
    } while (choice != 0);


    return 0;
}
