
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;
const string ClientBankData = "ClientBankData.txt";
struct ClientData {
    string AccountNumber = "";
    string PinCode = "";
    string Name = "";
    string Phone = "";
    double AccountBalance = 0.0;
    bool MarkForDelete = false;
};
string ReadAccountNumber(string Message) {
    string AccountNumber = "";
    cout << Message;
    cin >> AccountNumber;
    return AccountNumber;
}

void deposit(vector<ClientData>vsClient);
void BankSystem(int num);
void GoBackToMainMenu();

void displayMenu();
void displayClients(vector<ClientData>& vsClient);
void Withdraw(vector<ClientData>vsClient);

vector<ClientData> getClientsFromFile(string AccountName);
void PrintClientCard(ClientData client) {

    std::cout << std::endl << "The following are the client details:" << std::endl;

    std::cout << "Account Number : " << client.AccountNumber << std::endl;
    std::cout << "Pin Code       : " << client.PinCode << std::endl;
    std::cout << "Name           : " << client.Name << std::endl;
    std::cout << "Phone          : " << client.Phone << std::endl;
    std::cout << "Account Balance: " << client.AccountBalance << std::endl;

}
vector<string> SplitString(string Text, string Delim = " ") {
    vector<string> vTokens;

    short pos = 0;
    string Token = "";
    while ((pos = Text.find(Delim)) != string::npos)
    {
        Token = Text.substr(0, pos);
        if (Token != "")
        {
            vTokens.push_back(Token);
        }
        Text.erase(0, pos + Delim.length());
    }
    if (Text != "")
    {
        vTokens.push_back(Text);
    }
    return vTokens;
}

string ConvertRecordToLine(ClientData NewClient, string Seperator = "#//#") {

    string formattedData = NewClient.AccountNumber + Seperator +
        NewClient.PinCode + Seperator +
        NewClient.Name + Seperator +
        NewClient.Phone + Seperator +
        to_string(NewClient.AccountBalance);
    return formattedData;
}
vector<ClientData> SaveClientsDataToFile(string FileName, vector<ClientData>& vsClients) {
    fstream ClientBankDataFile;
    ClientBankDataFile.open(ClientBankData, ios::out);
    string Line;
    if (ClientBankDataFile.is_open())
    {
        for (ClientData& C : vsClients)
        {
            if (C.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(C);
                ClientBankDataFile << Line << endl;

            }
        }
        ClientBankDataFile.close();
    }
    return vsClients;
}

ClientData ConvertLineToRecord(string Line, string Delim = "#//#") {
    ClientData Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Delim);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}
bool IsExist(string AccountNumber, vector<ClientData> vsClient) {

    for (ClientData& Client : vsClient)
    {
        if (Client.AccountNumber == AccountNumber) {
            return true;
        }

    }
    return false;

}
bool ReadClientData(ClientData& NewClient, vector<ClientData> vsClient) {

    cout << "Adding New Client: \n";
    cout << "\n";

    // Read Account Number
    cout << "Enter Account Number? ";
    getline(cin >> ws, NewClient.AccountNumber);

    if (!IsExist(NewClient.AccountNumber, vsClient))
    {
        // Read Pin Code
        cout << "Enter Pin Code? ";
        getline(cin, NewClient.PinCode);

        // Read Name (allows spaces)
        cout << "Enter Name? ";
        getline(cin, NewClient.Name);

        // Read Phone
        cout << "Enter Phone? ";
        getline(cin, NewClient.Phone);

        // Read Account Balance (use cin for numerical input)
        cout << "Enter Account Balance? ";
        cin >> NewClient.AccountBalance;
        return true;

    }
    else
    {
        cout << "this Account Number already exists!\n";
        return false;

    }
   

}


int ReadNumber(string Message);
void showMenu() {
    system("cls");
    cout << "==============================" << endl;
    cout << "      Main Menu Screen" << endl;
    cout << "==============================" << endl;
    cout << "[1] Show Client List." << endl;
    cout << "[2] Add New Client." << endl;
    cout << "[3] Delete Client." << endl;
    cout << "[4] Update Client Info." << endl;
    cout << "[5] Find Client." << endl;
    cout << "[6] Transactions." << endl;
    cout << "[7] Exit." << endl;
    cout << "==============================" << endl;

    BankSystem(ReadNumber("Choose what do you want to do? [1 to 7]? "));
}


void AddDataLineToFile(string ClientBankData, string Line) {
    fstream ClientBankDataFile;

    ClientBankDataFile.open(ClientBankData, ios::out | ios::app);

    if (ClientBankDataFile.is_open()) {

        ClientBankDataFile << Line << endl;
        ClientBankDataFile.close();
    }
    else {
        cout << "Unable to open file.";
    }
}


void addNewClient(vector<ClientData>& vsClient) {
    char AddMore = 'Y';
    do
    {
        cout << "==============================" << endl;
        cout << "      Add New Clients Screen" << endl;
        cout << "==============================" << endl;


        ClientData NewClient;
         
        if (ReadClientData(NewClient, vsClient))
        {
            AddDataLineToFile(ClientBankData, ConvertRecordToLine(NewClient));

            cout << "\nClient with Account Number " << NewClient.AccountNumber << " added successfully!" << endl;

            cout << "\ndo you want to add more Clients? \n";
            cin >> AddMore;
            if (toupper(AddMore) != 'Y')
            {
                GoBackToMainMenu();
            }
            system("cls");

            vsClient = getClientsFromFile(ClientBankData);
        }
        else
        {

            system("cls");
            cout << "this Account Number already exists!\n";
            addNewClient(vsClient);

        }
        

    } while (toupper(AddMore) == 'Y');
}
bool MarkClientsForDeleteByID(string AccountNumber, vector<ClientData>& Clients) {
    for (ClientData& C : Clients)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;

            return true;
        }
    }
    return false;
}

vector<ClientData> getClientsFromFile(string fileName) {

    fstream ClientBankDataFile;
    vector<ClientData> vsClientsData;

    ClientBankDataFile.open(fileName, ios::in);
    if (ClientBankDataFile.is_open())
    {
        string Line = "";
        while (getline(ClientBankDataFile, Line))
        {
            vsClientsData.push_back(ConvertLineToRecord(Line));
        }
        ClientBankDataFile.close();
    }
    return vsClientsData;
}

bool getClientByID(string AccountNumber, vector<ClientData>& vsClients, ClientData& Client) {

    vsClients = getClientsFromFile(ClientBankData);
    for (ClientData& Record : vsClients)
    {
        if (Record.AccountNumber == AccountNumber)
        {
            Client = Record;
            return true;
        }
    }
    return false;



}


bool DeleteClientByID(string AccountNumber, vector<ClientData>& vsClient) {
    ClientData Client;
    char Answer = 'n';
    if (getClientByID(AccountNumber, vsClient, Client))
    {
        PrintClientCard(Client);

        cout << "Are you sure you want to delete this Client? Y/N ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {
            MarkClientsForDeleteByID(AccountNumber, vsClient);
            SaveClientsDataToFile(ClientBankData, vsClient);
            vsClient = getClientsFromFile(ClientBankData);

            cout << "Client with Account Number " << AccountNumber << " deleted successfully!" << endl;
            return true;
        }
    }
    else
    {
        return false;
    }
}
void deleteClient(vector<ClientData>& vsClient) {
    char AddMore = 'n';

    do
    {
        cout << "==============================" << endl;
        cout << "      Delete Client Screen" << endl;
        cout << "==============================" << endl;

        string AccountNumber = ReadAccountNumber("Please enter Account Number? ");

        if (DeleteClientByID(AccountNumber, vsClient)) {
            cout << "\ndo you want to Delete more Clients? \n";
            cin >> AddMore;

            GoBackToMainMenu();

            system("cls");
        }
        else
        {
            system("cls");

            cout << "\nClient With Account Number (" << AccountNumber << ") not Found!";

        }

    } while (toupper(AddMore) == 'Y');

}
ClientData ReadForUpdateClient(ClientData Client) {

    // Read Pin Code
    cout << "Enter Pin Code? ";
    getline(cin >> ws, Client.PinCode);

    // Read Name (allows spaces)
    cout << "Enter Name? ";
    getline(cin, Client.Name);

    // Read Phone
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    // Read Account Balance (use cin for numerical input)
    cout << "Enter Account Balance? ";
    cin >> Client.AccountBalance;

    return Client;
}
bool UpdateClient(ClientData Client, vector<ClientData>& vsClient) {

    for (ClientData& Record : vsClient)
    {
        if (Record.AccountNumber == Client.AccountNumber)
        {
            Record.PinCode = Client.PinCode;
            Record.Name = Client.Name;
            Record.Phone = Client.Phone;
            Record.AccountBalance = Client.AccountBalance;
            return true;
        }
    }
    return false;

}

bool UpdateClientByID(string AccountNumber, vector<ClientData>& vsClient) {

    ClientData Client;
    char Answer = 'n';
    if (getClientByID(AccountNumber, vsClient, Client))
    {
        PrintClientCard(Client);

        cout << "Are you sure you want to Update this Client? Y/N ? ";
        cin >> Answer;
        if (toupper(Answer) == 'Y')
        {

            Client = ReadForUpdateClient(Client);
            UpdateClient(Client, vsClient);
            SaveClientsDataToFile(ClientBankData, vsClient);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient With Account Number (" << AccountNumber << ") not Found!";
        return false;
    }
}

void UpdateClientScreen(vector<ClientData>& vsClient) {
    char AddMore = 'n';
    do
    {
        cout << "==============================" << endl;
        cout << "      Update Client Screen" << endl;
        cout << "==============================" << endl;

        string AccountNumber = ReadAccountNumber("Please enter Account Number? ");

        if (UpdateClientByID(AccountNumber, vsClient)) {
            cout << "\ndo you want to Update more Clients? Y/N ? ";
            cin >> AddMore;
            system("cls");
        }
        else
        {
            system("cls");

            cout << "\nClient With Account Number (" << AccountNumber << ") not Found!";

        }

    } while (toupper(AddMore) == 'Y');
    GoBackToMainMenu();

}
void FindClient(vector<ClientData>& vsClient) {
    ClientData Client;
    char AddMore = 'n';
    do
    {
        cout << "==============================" << endl;
        cout << "      Find Client Screen" << endl;
        cout << "==============================" << endl;

        string AccountNumber = ReadAccountNumber("Please enter Account Number? ");

        if (getClientByID(AccountNumber, vsClient, Client))
        {
            PrintClientCard(Client);
            cout << "\ndo you want to Find more Clients? Y/N ? ";
            cin >> AddMore;
            system("cls");
        }

    } while (toupper(AddMore) == 'Y');
    GoBackToMainMenu();

}
void PrintClientData(const ClientData& client) {
    std::cout << "| " << std::setw(10) << std::left << client.AccountNumber
        << " | " << std::setw(6) << std::left << client.PinCode
        << " | " << std::setw(25) << std::left << client.Name
        << " | " << std::setw(10) << std::left << client.Phone
        << " | " << std::setw(10) << std::left << client.AccountBalance
        << " |" << std::endl;
}

void PrintHeader(short NumOfRecord = 0) {
    // Print header
    cout << setw(40) << "Client List (" << NumOfRecord << ") Client(s)." << endl;
    cout << string(70, '-') << endl; // Top border
    cout << "| " << setw(10) << left << "Account"
        << " | " << setw(6) << left << "Pin"
        << " | " << setw(25) << left << "Client Name"
        << " | " << setw(10) << left << "Phone"
        << " | " << setw(10) << left << "Balance"
        << " |" << endl;
    cout << string(70, '-') << endl; // Divider


    cout << string(70, '-') << endl; // Bottom border
}


void ShowClients(vector<ClientData> vsClientsData) {

    PrintHeader(vsClientsData.size());
    for (ClientData& Record : vsClientsData)
    {
        PrintClientData(Record);
    }
    GoBackToMainMenu();

}
void GoBackToMainMenu() {
    cout << "Press any Key to go back into Main Menu...";
    system("pause>0");
    showMenu();
}
int ReadNumber(string Message) {
    int num = 0;
    cout << Message;
    cin >> num;
    return num;
}
void BankSystem(int choice) {
    vector<ClientData>vsClient = getClientsFromFile(ClientBankData);


    switch (choice) {
    case 1:
        system("cls");
        ShowClients(vsClient);
        break;
    case 2:
        system("cls");

        addNewClient(vsClient);
        break;

    case 3:
        system("cls");

        deleteClient(vsClient);
        break;
    case 4:
        system("cls");

        UpdateClientScreen(vsClient);
        break;
    case 5:
        system("cls");

        FindClient(vsClient);
        break;
    case 6:
        system("cls");

        displayMenu();
        break;
    case 7:
        system("cls");

        cout << "Exiting..." << endl;
        break;
    default:
        system("cls");

        cout << "Invalid choice! Please select between 1 and 6." << endl;
        GoBackToMainMenu();

        break;
    }

}


int main()
{
    showMenu();
    return 0;
}

void GoBackToDisplayMenu() {
    cout << "Press any Key to go back into Main Menu...";
    system("pause>0");
    displayMenu();
}

void processMenu(int choice) {
    vector<ClientData>vsClient = getClientsFromFile(ClientBankData);

    switch (choice) {
    case 1:
        system("cls");
        deposit(vsClient);
        GoBackToDisplayMenu();

        break;
    case 2:
        system("cls");
        Withdraw(vsClient);
        GoBackToDisplayMenu();
        break;
    case 3:
        system("cls");
        displayClients(vsClient);
        GoBackToDisplayMenu();
        break;
    case 4:
        system("cls");
        showMenu();
        break;
    default:
        cout << "Invalid choice!" << endl;
        break;
    }
}
void displayMenu() {
    system("cls");
    cout << "\n==============================" << endl;
    cout << "Transactions Menu Screen" << endl;
    cout << "==============================" << endl;
    cout << "[1] Deposit" << endl;
    cout << "[2] Withdraw" << endl;
    cout << "[3] Total Balances" << endl;
    cout << "[4] Main Menu" << endl;
    cout << "==============================" << endl;

    processMenu(ReadNumber("Choose what do you want to do? [1 to 4]? "));
}
bool UpdateDeposit(ClientData Client, vector<ClientData>& vsClient) {

    for (ClientData& Record : vsClient)
    {
        if (Record.AccountNumber == Client.AccountNumber)
        {
            Record.AccountBalance = Client.AccountBalance;
            return true;
        }
    }
    return false;

}
void deposit(vector<ClientData>vsClient) {
    system("cls");
    cout << "==============================" << endl;
    cout << "      Deposit Screen" << endl;
    cout << "==============================" << endl;
    ClientData Client;
    char Answer = 'n';
    string AccountNumber = ReadAccountNumber("Please enter Account Number? ");

    if (getClientByID(AccountNumber, vsClient, Client))
    {
        PrintClientCard(Client);
    }
    else
    {
        system("cls");

        cout << "\nClient With Account Number (" << AccountNumber << ") not Found!";

    }
    double amount;
    cout << "Enter the amount to deposit: ";
    cin >> amount;
    if (amount > 0) {
        Client.AccountBalance += amount;
        UpdateDeposit(Client,vsClient);
        SaveClientsDataToFile(ClientBankData, vsClient);

        cout << "Deposited: $" << fixed << setprecision(2) << amount << endl;

    }
    else {
        cout << "Invalid deposit amount!" << endl;
    }
}
double calculateTotalBalance(vector<ClientData>& clients) {
    double total = 0;
    for (ClientData& client : clients) {
        total += client.AccountBalance;
    }
    return total;
}
 void displayClients(vector<ClientData>& vsClient) {
    cout << "Balances List (" << vsClient.size() << ") Client(s)." << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << setw(15) << left << "Account Number"
        << setw(30) << "Client Name"
        << setw(10) << right << "Balance" << endl;
    cout << "-------------------------------------------------------------" << endl;

    for (ClientData& client : vsClient) {
        cout << setw(15) << left << client.AccountNumber
            << setw(30) << client.Name
            << setw(10) << right << client.AccountBalance << endl;
    }
    double totalBalance = calculateTotalBalance(vsClient);
    cout << "\nTotal Balances = " << totalBalance << endl;


}
 void Withdraw(vector<ClientData>vsClient) {
     system("cls");
     cout << "==============================" << endl;
     cout << "      Withdraw Screen" << endl;
     cout << "==============================" << endl;
     ClientData Client;
     char Answer = 'n';
     string AccountNumber = ReadAccountNumber("Please enter Account Number? ");

     if (getClientByID(AccountNumber, vsClient, Client))
     {
         PrintClientCard(Client);
     }
     else
     {
         system("cls");

         cout << "\nClient With Account Number (" << AccountNumber << ") not Found!";

     }
     double amount;
     cout << "Enter the amount to Withdraw: ";
     cin >> amount;
     if (amount > 0 && amount <= Client.AccountBalance) {
         Client.AccountBalance -= amount;
         UpdateDeposit(Client, vsClient);
         SaveClientsDataToFile(ClientBankData, vsClient);
         cout << "Withdrawn: $" << fixed << setprecision(2) << amount << endl;
     }
     else if (amount > Client.AccountBalance) {
         cout << "Insufficient balance!" << endl;
     }
     else {
         cout << "Invalid withdraw amount!" << endl;
     }
 }