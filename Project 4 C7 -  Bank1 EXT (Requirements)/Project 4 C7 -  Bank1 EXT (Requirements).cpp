#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

const string CleintsFillName = "RecordClient.txt";
const string UserFillName = "UserRecord.txt";

struct stUserData
{
	string UserName;
	string Password;
	int Permissions;

	bool MarkToDelete = false;
};

void ShowTransactionsMenueScreen();

void MainBankAppPage();

enum enMainOptions
{
	Show_Client = 1,
	Add_New_Client = 2,
	Delete_Client = 3,
	Updat_Client = 4,
	Find_Client = 5,
	Transactions = 6,
	Manage_Usre = 7,
	Logout = 8
};

enum enMainMenuePermissions
{
	pAll = -1,
	pListClient = 1,
	pAddNewClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pManageUser = 64
};

enum enTransactionOpt
{
	eDeposit = 1,
	eWithdraw = 2,
	eTotalBalances = 3,
	eMain_Menue = 4
};

enum enManageOpt
{
	ShowUsers = 1,
	Add_New_User = 2,
	Delete_User = 3,
	Update_User = 4,
	Find_User = 5,
	Main_Menue = 6
};

struct stClientData
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;

	bool MarkToDelete = false;
};

stUserData CurrentUser;

string ReadText(string Message)
{
	string Text;

	cout << Message;
	getline(cin >> ws, Text);

	return Text;
}

string ReadAccountNumber()
{

	string AccountNumber = "";
	cout << "Please enter AccountNumber? ";
	cin >> AccountNumber;

	return AccountNumber;
}

double ReadNumber_D(string Message)
{
	double Number;

	cout << Message;
	cin >> Number;

	return Number;
}

stClientData ChangeClientRecord(string AccountNumber)
{
	stClientData ClientData;
	// Usage of std::ws will extract allthe whitespace character
	ClientData.AccountNumber = AccountNumber;

	cout << "Enter PinCode? ";
	getline(cin >> ws, ClientData.PinCode);

	cout << "Enter Name? ";
	getline(cin, ClientData.Name);

	cout << "Enter Phone Number? ";
	getline(cin, ClientData.Phone);

	ClientData.AccountBalance = ReadNumber_D("Enter AccountBalance? ");

	return ClientData;
}

vector<string> SplitString(string Text, string delim = "#//#")
{
	vector<string> vData;
	short pos = 0; // position
	string word;

	while ((pos = Text.find(delim)) != std::string::npos)
	{
		word = Text.substr(0, pos);
		if (word != "")
			vData.push_back(word);

		Text.erase(0, (pos + delim.length()));
	}

	if (Text != "")
		vData.push_back(Text);

	return vData;
}

string ConvetClientDataToLine(stClientData Data, string Seperator = "#//#")
{
	string Record = "";

	Record += Data.AccountNumber + Seperator;
	Record += Data.PinCode + Seperator;
	Record += Data.Name + Seperator;
	Record += Data.Phone + Seperator;
	Record += to_string(Data.AccountBalance);

	return Record;
}

stClientData ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	stClientData ClientData;
	vector<string> vData;

	vData = SplitString(Line, Seperator);

	ClientData.AccountNumber = vData[0];
	ClientData.PinCode = vData[1];
	ClientData.Name = vData[2];
	ClientData.Phone = vData[3];
	ClientData.AccountBalance = stod(vData[4]);

	return ClientData;
}

vector<stClientData> LoadClientsDataFromFile(string url)
{
	vector<stClientData> vDataRecordClients;

	fstream TheFile;
	TheFile.open(url, ios::in); // read mode

	if (TheFile.is_open())
	{

		string line;
		stClientData Client;

		while (getline(TheFile, line))
		{
			if (line != "")
			{
				Client = ConvertLineToRecord(line);
				vDataRecordClients.push_back(Client);
			}
		}
		TheFile.close();
	}

	return vDataRecordClients;
}

void AddDataLineToFile(string url = "Text.txt", string RecordText = "Null")
{

	fstream TheFile;
	TheFile.open(url, ios::out | ios::app);

	if (TheFile.is_open())
	{
		TheFile << RecordText + '\n';
		TheFile.close();
	}
	else
	{
		cout << "There is something wrong, We can't write know.\n";
		TheFile.close();
	}
}

bool CheckIfTheClientExisting(string AccountNumber)
{
	vector<stClientData> vClientsData;
	vClientsData = LoadClientsDataFromFile(CleintsFillName);
	for (stClientData &C : vClientsData)
	{
		if (C.AccountNumber == AccountNumber)
		{
			return true;
		}
	}
	return false;
}

// Print Function

void PrintClientData(stClientData ClientData)
{
	/*cout << "The following are the client details:\n\n";*/
	cout << "\nAccount Number : " << ClientData.AccountNumber;
	cout << "\nPIN Code       : " << ClientData.PinCode;
	cout << "\nClient Name    : " << ClientData.Name;
	cout << "\nPhone          : " << ClientData.Phone;
	cout << "\nAccount Balance: " << ClientData.AccountBalance << "$";
	cout << endl;
}

// Show Clients Function:

void HeadOfPageForShowAllClients(int unsigned NumberOfClients = 0)
{
	cout << "\n\t\t\t\t Client List (" + to_string(NumberOfClients);
	if (NumberOfClients > 1)
	{
		cout << ") Clients.";
	}
	else
	{
		cout << ") Client.";
	}
	cout << "\n------------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "PIN Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance";
	cout << "\n------------------------------------------------------------------------------------------------------";
	cout << endl;
}

void PrintClientDataByLine(stClientData ClientData)
{
	cout << "\n| " << setw(15) << left << ClientData.AccountNumber;
	cout << "| " << setw(10) << left << ClientData.PinCode;
	cout << "| " << setw(40) << left << ClientData.Name;
	cout << "| " << setw(12) << left << ClientData.Phone;
	cout << "| " << setw(12) << left << ClientData.AccountBalance;
	cout << endl;
}

void ShowAllClients()
{
	vector<stClientData> vClients;
	vClients = LoadClientsDataFromFile(CleintsFillName);
	int unsigned ClientsNumber = vClients.size();

	HeadOfPageForShowAllClients(ClientsNumber);
	cout << endl;

	for (stClientData &Line : vClients)
	{
		PrintClientDataByLine(Line);
	}
	cout << "\n------------------------------------------------------------------------------------------------------\n";
}

// Add New Client Function:

stClientData ReadNewClient()
{
	stClientData ClientData;

	cout << "\nPlease Enter Client Data: \n\n";

	// Usage of std::ws will extract allthe whitespace character
	cout << "Enter Account Number? ";
	getline(cin >> ws, ClientData.AccountNumber);

	while (CheckIfTheClientExisting(ClientData.AccountNumber))
	{
		cout << "Client with Account Number (" + ClientData.AccountNumber + ") not Found, Enter anouther Account Number? ";
		getline(cin >> ws, ClientData.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, ClientData.PinCode);

	cout << "Enter Name? ";
	getline(cin, ClientData.Name);

	cout << "Enter Phone Number? ";
	getline(cin, ClientData.Phone);

	ClientData.AccountBalance = ReadNumber_D("Enter AccountBalance? ");

	return ClientData;
}

void AddNewClient()
{

	stClientData Client;
	Client = ReadNewClient();
	AddDataLineToFile(CleintsFillName, ConvetClientDataToLine(Client));
}

void AddClient()
{
	char AddMoreClient = 'y';
	do
	{
		AddNewClient();

		cout << "Client Added Successfully, do you want to add more client?";
		cin >> AddMoreClient;
		cin.ignore(1, '\n');

	} while (toupper(AddMoreClient) == 'Y');
}

void ShowAddNewClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tAdd New Clients Screen";
	cout << "\n---------------------------------------------\n";
	AddClient();
}

// Find Client Function:

bool FindClient(stClientData &Client, vector<stClientData> vClientsData, string AccountNumber)
{
	for (stClientData &C : vClientsData)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

void FindClientByAccountNumber(vector<stClientData> vClientsData, string AccountNumber)
{
	stClientData Client;
	if (FindClient(Client, vClientsData, AccountNumber))
	{
		PrintClientData(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" + AccountNumber + ") not Found!\n";
	}
}

void ShowFindClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tFind Clients Screen";
	cout << "\n---------------------------------------------\n";

	vector<stClientData> vClients = LoadClientsDataFromFile(CleintsFillName);
	string AccountNumber = ReadAccountNumber();
	FindClientByAccountNumber(vClients, AccountNumber);
}

// Save Client Function:

void SaveCleintsDataToFile(string URL, vector<stClientData> vClientsData)
{
	fstream TheFile;
	TheFile.open(URL, ios::out); // read mode

	if (TheFile.is_open())
	{

		for (stClientData Client : vClientsData)
		{

			if (Client.MarkToDelete == false)
			{
				TheFile << ConvetClientDataToLine(Client) << endl;
			}
		}
		TheFile.close();
	}
}

// Update Client Info Function:

void UpdatClientForDeleteByAccountNumber(string TargetAccount, vector<stClientData> &vClientsData)
{

	for (stClientData &Client : vClientsData)
	{
		if (Client.AccountNumber == TargetAccount)
		{
			Client = ChangeClientRecord(TargetAccount);
			break;
		}
	}
}

void UpdateClientByAccountNumber(vector<stClientData> &vClients, string AccountNumber)
{
	stClientData Client;

	if (FindClient(Client, vClients, AccountNumber))
	{
		cout << "\nThe following are the client details:\n\n";
		PrintClientData(Client);

		char Answer = 'n';

		cout << "\nAre you sure want update this clients? y/n ? ";

		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			cout << "\n\n";
			UpdatClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(CleintsFillName, vClients);

			cout << "\nClient Update Successfully.\n";
		}
	}
	else
	{
		cout << "\nClient with Account Number (" + AccountNumber + ") not Found!\n";
	}
}

void ShowUpdateClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tUpdate Clients Screen";
	cout << "\n---------------------------------------------\n";

	vector<stClientData> vClients = LoadClientsDataFromFile(CleintsFillName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(vClients, AccountNumber);
}

// Delete Client Function:

void MarkClientForDeleteByAccountNumber(string TargetAccount, vector<stClientData> &vClientsData)
{

	for (stClientData &Client : vClientsData)
	{
		if (Client.AccountNumber == TargetAccount)
		{
			Client.MarkToDelete = true;
		}
	}
}

void DeleteClientByAccountNumber(vector<stClientData> &vClients, string AccountNumber)
{
	stClientData Client;

	if (FindClient(Client, vClients, AccountNumber))
	{
		PrintClientData(Client);

		char Answer;

		cout << "\nAre you sure want delete this clients? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(CleintsFillName, vClients);

			// Refresh Clients
			vClients = LoadClientsDataFromFile(CleintsFillName);

			cout << "\nClient Deleted Successfully.\n";
		}
	}
	else
	{
		cout << "Client with Account Number (" + AccountNumber + ") not Found!";
	}
}

void ShowDeleteClientScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\Delete Clients Screen";
	cout << "\n---------------------------------------------\n";

	vector<stClientData> vClients = LoadClientsDataFromFile(CleintsFillName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(vClients, AccountNumber);
}

void LoginScreen();

// Declaring Function

void HeadOfLoginScreen();

void ShowMangeUsersScreen();

// Head Of App

void ShowMainMenue()
{
	cout << "\n=============================================\n";
	cout << "\t\t";
	cout << "Main Menue Screen";
	cout << "\n=============================================\n";
	cout << "\t [1] Show Client List.\n";
	cout << "\t [2] Add New Client.\n";
	cout << "\t [3] Delete Client.\n";
	cout << "\t [4] Update Client Info.\n";
	cout << "\t [5] Find Client.\n";
	cout << "\t [6] Transactions.\n";
	cout << "\t [7] Manage Users.\n";
	cout << "\t [8] Logout.";
	cout << "\n=============================================\n";
}

void ShowEndScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tProgram Ends :-)";
	cout << "\n---------------------------------------------\n";
	system("pause>0");
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	MainBankAppPage();
}

enMainOptions ReadMainMenueOption(string Message)
{
	short OptNumber;
	do
	{
		cout << Message;
		cin >> OptNumber;

	} while (!(OptNumber > 0 && OptNumber <= 8));

	enMainOptions TheOpt = (enMainOptions)OptNumber;
	return TheOpt;
}

void ShowAccessDeniedMessage()
{
	printf("-------------------------------------\n");
	printf("Access Demied,\nYou dont Have Permission To Do this,\nPlease Conact Your Admin.");
	printf("\n-------------------------------------\n");
}

void CheckTheUserCanAccess(enMainMenuePermissions Permissions)
{

	if (CurrentUser.Permissions == enMainMenuePermissions::pAll)
		return;

	if (!((Permissions & CurrentUser.Permissions) == Permissions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
	}
}

void PerformMainMenueOption(enMainOptions Opt)
{
	// vector <stClientData> vClients;
	// vClients = LoadClientsDataFromFile(CleintsFillName);

	switch (Opt)
	{
	case Show_Client:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pListClient);

		ShowAllClients();
		GoBackToMainMenue();
		break;
	case Add_New_Client:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pAddNewClient);

		ShowAddNewClientScreen();
		GoBackToMainMenue();
		break;
	case Delete_Client:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pDeleteClient);

		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case Updat_Client:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pUpdateClient);

		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case Find_Client:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pFindClient);

		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case Transactions:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pTransactions);

		ShowTransactionsMenueScreen();
		break;
	case Manage_Usre:
		system("CLS");

		CheckTheUserCanAccess(enMainMenuePermissions::pManageUser);

		ShowMangeUsersScreen();
	case Logout:
		system("CLS");
		LoginScreen();
		break;
	}
}

void MainBankAppPage()
{
	enMainOptions Option;

	system("CLS");
	ShowMainMenue();
	Option = ReadMainMenueOption("Choose what do you want to do? [1 to 8]? ");
	PerformMainMenueOption(Option);
}

// Transactions Menue Screen

void GoBackToTransactionsMenueScreen()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowTransactionsMenueScreen();
}

enTransactionOpt ReadTransactionsMenueOption(string Message)
{
	short OptNumber;
	do
	{
		cout << Message;
		cin >> OptNumber;

	} while (!(OptNumber > 0 && OptNumber <= 6));

	enTransactionOpt TheOpt = (enTransactionOpt)OptNumber;
	return TheOpt;
}

void ShowTransactionsMenue()
{
	cout << "\n=============================================\n";
	cout << "\t\t";
	cout << "Transactions Menue Screen";
	cout << "\n=============================================\n";
	cout << "\t [1] Deposit.\n";
	cout << "\t [2] Withdraw.\n";
	cout << "\t [3] Total Balances.\n";
	cout << "\t [4] Main Menue.";
	cout << "\n=============================================\n";
}

void HeadOfPageForShowTotalBalances(int unsigned NumberOfClients = 0)
{
	cout << "\n\t\t\t\t Balances List (" + to_string(NumberOfClients);
	if (NumberOfClients > 1)
	{
		cout << ") Clients.";
	}
	else
	{
		cout << ") Client.";
	}
	cout << "\n--------------------------------------------------------------------------------\n";
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Balance";
	cout << "\n--------------------------------------------------------------------------------";
	cout << endl;
}

void PrintClientsTotalBalances(stClientData ClientData)
{
	cout << "\n| " << setw(15) << left << ClientData.AccountNumber;
	cout << "| " << setw(40) << left << ClientData.Name;
	cout << "| " << setw(12) << left << ClientData.AccountBalance;
	cout << endl;
}

void ShowTotalBalancesScreen()
{
	vector<stClientData> vClientData;
	vClientData = LoadClientsDataFromFile(CleintsFillName);

	int NumberOfClients = vClientData.size();

	HeadOfPageForShowTotalBalances(NumberOfClients);

	if (NumberOfClients == 0)
	{
		cout << "\n\n\t\t No Clients Available In The System";
	}
	for (stClientData C : vClientData)
	{
		PrintClientsTotalBalances(C);
	}
	cout << "\n--------------------------------------------------------------------------------";
}

void DepositAction(stClientData &Client, vector<stClientData> &vClients, string TargetAccount, double Deposit)
{

	for (stClientData &c : vClients)
	{
		if (c.AccountNumber == TargetAccount)
		{
			c.AccountBalance += Deposit;
			Client = c;
			break;
		}
	}
}

void DepositByAccountNumber(stClientData &Client, vector<stClientData> &vClientData, string AccountNumber)
{

	double DepositAmount;
	char Answer = 'n';

	cout << "\n\nThe following are the client details:\n";
	cout << "---------------------------------------------\n";
	PrintClientData(Client);
	cout << "---------------------------------------------\n\n";

	DepositAmount = ReadNumber_D("Please enter deposit amount? ");

	cout << "Are you sure you want perfrom this transaction? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{

		DepositAction(Client, vClientData, AccountNumber, DepositAmount);
		SaveCleintsDataToFile(CleintsFillName, vClientData);

		cout << "Done successfully. ";
		cout << "New Balance : ";
		cout << Client.AccountBalance;

		cout << endl;
		GoBackToTransactionsMenueScreen();
	}

	ShowTransactionsMenueScreen();
}

void ShowDepositScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\Deposit Screen";
	cout << "\n---------------------------------------------\n";

	vector<stClientData> vClientsData;
	stClientData Client;
	string AccountNumber;

	vClientsData = LoadClientsDataFromFile(CleintsFillName);
	AccountNumber = ReadAccountNumber();

	while (!FindClient(Client, vClientsData, AccountNumber))
	{
		cout << "Client with [" + AccountNumber + "] dose not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	DepositByAccountNumber(Client, vClientsData, AccountNumber);
}

void WithdrawByAccountNumber(stClientData &Client, vector<stClientData> &vClientData, string AccountNumber)
{

	double WithdrawAmount;
	char Answer = 'n';

	cout << "\n\nThe following are the client details:\n\n";
	cout << "---------------------------------------------\n";
	PrintClientData(Client);
	cout << "---------------------------------------------\n";

	WithdrawAmount = ReadNumber_D("Please enter Withdraw amount? ");

	while (WithdrawAmount > Client.AccountBalance)
	{
		cout << "Amount Exceeds the balance, you can withdraw up to: " << Client.AccountBalance << endl;
		cout << "Please enter another amount? \n";
		cin >> WithdrawAmount;
	}

	cout << "Are you sure you want perfrom this transaction? y/n? ";
	cin >> Answer;

	if (Answer == 'y' || Answer == 'Y')
	{

		WithdrawAmount *= -1;
		DepositAction(Client, vClientData, AccountNumber, WithdrawAmount);
		SaveCleintsDataToFile(CleintsFillName, vClientData);

		cout << "Done successfully. ";
		cout << "New Balance : ";
		cout << Client.AccountBalance;

		cout << endl;
		GoBackToTransactionsMenueScreen();
	}

	ShowTransactionsMenueScreen();
}

void ShowWithdrawScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tWithdraw Screen";
	cout << "\n---------------------------------------------\n";

	vector<stClientData> vClientsData;
	stClientData Client;
	string AccountNumber;

	vClientsData = LoadClientsDataFromFile(CleintsFillName);
	AccountNumber = ReadAccountNumber();

	while (!FindClient(Client, vClientsData, AccountNumber))
	{
		cout << "Client with [" + AccountNumber + "] dose not exist.\n";
		AccountNumber = ReadAccountNumber();
	}

	WithdrawByAccountNumber(Client, vClientsData, AccountNumber);
}

void PerformTransactionsMenueOption(enTransactionOpt TransOpt)
{
	switch (TransOpt)
	{
	case eDeposit:
		system("CLS");
		ShowDepositScreen();
		break;
	case eWithdraw:
		system("CLS");
		ShowWithdrawScreen();
		break;
	case eTotalBalances:
		system("CLS");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenueScreen();
		break;
	case eMain_Menue:
		MainBankAppPage();
		break;
	}
}

void ShowTransactionsMenueScreen()
{
	system("CLS");
	ShowTransactionsMenue();
	PerformTransactionsMenueOption(ReadTransactionsMenueOption("Choose what do you want to do? [1 to 4]? "));
}

// EXT 2 :

// Print User Info Function

void PrintUserData(stUserData UserData)
{
	cout << "The following are the User details:\n";
	cout << "---------------------------------------";
	cout << "\nUsername     : " << UserData.UserName;
	cout << "\nPassword     : " << UserData.Password;
	cout << "\nPermissions  : " << UserData.Permissions;
	cout << "\n---------------------------------------\n\n";
}

string ConvetUserDataToLineForUser(stUserData Data, string Seperator = "#//#")
{
	string Record = "";

	Record += Data.UserName + Seperator;
	Record += Data.Password + Seperator;
	Record += to_string(Data.Permissions) + Seperator;

	return Record;
}

stUserData ConvertLineToRecordForUser(string Line, string Seperator = "#//#")
{
	stUserData UserData;
	vector<string> vData;

	vData = SplitString(Line, Seperator);

	UserData.UserName = vData[0];
	UserData.Password = vData[1];
	UserData.Permissions = stoi(vData[2]);

	return UserData;
}

vector<stUserData> LoadUserDataFromFile(string url)
{
	vector<stUserData> vDataRecordUser;

	fstream TheFile;
	TheFile.open(url, ios::in); // read mode

	if (TheFile.is_open())
	{

		string line;
		stUserData User;

		while (getline(TheFile, line))
		{
			if (line != "")
			{
				User = ConvertLineToRecordForUser(line);
				vDataRecordUser.push_back(User);
			}
		}
		TheFile.close();
	}

	return vDataRecordUser;
}

// User Function:

bool FindUserByUsername(stUserData &User, string UserName)
{
	vector<stUserData> vUserData;
	vUserData = LoadUserDataFromFile(UserFillName);

	for (stUserData &U : vUserData)
	{
		if ((U.UserName == UserName))
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserAndGetValueByUsernameAndPassword(stUserData &User, string UserName, string Password)
{
	vector<stUserData> vUserData;
	vUserData = LoadUserDataFromFile(UserFillName);

	for (stUserData &U : vUserData)
	{
		if ((U.UserName == UserName) && (U.Password == Password))
		{
			User = U;
			return true;
		}
	}
	return false;
}

void FindUserByUsernameAndPassword(string Username, string Password)
{
	vector<stUserData> vUserData;
	vUserData = LoadUserDataFromFile(UserFillName);

	stUserData Client;
	if (FindUserAndGetValueByUsernameAndPassword(Client, Username, Password))
	{
		PrintUserData(Client);
	}
	else
	{
		cout << "\nClient with Account Number (" + Username + ") not Found!\n";
	}
}

bool LoadUserInfo(stUserData &User, string UserName, string Password)
{
	if (FindUserAndGetValueByUsernameAndPassword(User, UserName, Password))
		return true;
	else
		return false;
}

void GetUserInfo()
{

	string UserName, Password;

	bool LodingFaild = false;
	do
	{
		system("CLS");

		HeadOfLoginScreen();

		if (LodingFaild)
		{
			printf("Invalid Username/Password!\n");
		}

		UserName = ReadText("Enter Username? ");
		Password = ReadText("Enter Username? ");

		LodingFaild = !LoadUserInfo(CurrentUser, UserName, Password);

	} while (LodingFaild);
}

// Login

void HeadOfLoginScreen()
{
	cout << "\n=============================================\n";
	cout << "\t\t";
	cout << "Login Screen";
	cout << "\n=============================================\n";
}

void LoginScreen()
{
	stUserData UserLog;
	HeadOfLoginScreen();

	GetUserInfo();

	MainBankAppPage();
}

/////////////////////////////////////////////////////// Mange Users ///////////////////////////////////////////////////////

void GoBackToManageMenue();

void HeadOfPageForShowAllUsers(int unsigned NumberOfClients = 0)
{
	cout << "\n\t\t\t\t Users List (" + to_string(NumberOfClients);
	if (NumberOfClients > 1)
	{
		cout << ") Users.";
	}
	else
	{
		cout << ") User.";
	}
	cout << "\n------------------------------------------------------------------------------------------------------\n";
	cout << "| " << setw(40) << left << "User Number";
	cout << "| " << setw(12) << left << "Password";
	cout << "| " << setw(12) << left << "Permissions";
	cout << "\n------------------------------------------------------------------------------------------------------";
	cout << endl;
}

void PrintUserDataByLine(stUserData UserInfo)
{
	cout << "\n| " << setw(40) << left << UserInfo.UserName;
	cout << "| " << setw(12) << left << UserInfo.Password;
	cout << "| " << setw(12) << left << UserInfo.Permissions;

	cout << endl;
}

string ReadUserName()
{
	string UserName;
	cout << "\nPlease enter Username? ";
	cin >> UserName;

	return UserName;
}

// Show All Users

void ShowAllUsers()
{
	vector<stUserData> vUsers;
	vUsers = LoadUserDataFromFile(UserFillName);
	int unsigned UsersNumber = vUsers.size();

	HeadOfPageForShowAllUsers(UsersNumber);
	cout << endl;

	for (stUserData &Line : vUsers)
	{
		PrintUserDataByLine(Line);
	}
	cout << "\n------------------------------------------------------------------------------------------------------\n";
}

void PrintManageUsers()
{
	cout << "\n=============================================\n";
	cout << "\t\t";
	cout << "Mange User Menue Screen";
	cout << "\n=============================================\n";
	cout << "\t [1] List Users.\n";
	cout << "\t [2] Add New User.\n";
	cout << "\t [3] Delete User.\n";
	cout << "\t [4] Update User.\n";
	cout << "\t [5] Find User.\n";
	cout << "\t [6] Main Menue.\n";
	cout << "\n=============================================\n";
}

enManageOpt ReadManageUserMenueOption(string Message)
{
	short OptNumber;
	do
	{
		cout << Message;
		cin >> OptNumber;

	} while (!(OptNumber > 0 && OptNumber <= 6));

	enManageOpt TheOpt = (enManageOpt)OptNumber;
	return TheOpt;
}

// Find User

bool CheckIfTheUserExisting(string UserName)
{
	vector<stUserData> vUserData;
	vUserData = LoadUserDataFromFile(UserFillName);
	for (stUserData &C : vUserData)
	{
		if (C.UserName == UserName)
		{
			return true;
		}
	}
	return false;
}

// Add New Users Function:

int unsigned ReadThePerission(string Message, enMainMenuePermissions PermFor)
{
	char Ans;
	cout << Message;
	cin >> Ans;

	if (Ans == 'y' || Ans == 'Y')
	{
		return PermFor;
	}
	else
	{
		return 0;
	}
}

int ReadPermissionsToSet()
{
	short PermissionsNumber = 0;

	PermissionsNumber += ReadThePerission("Do you want to give full access? Y/N? ", enMainMenuePermissions::pAll);
	if (PermissionsNumber == -1)
		return PermissionsNumber;

	printf("\n\nDo you want to give access to : \n");
	PermissionsNumber += ReadThePerission("\nShow New Client? y/n? ", enMainMenuePermissions::pListClient);
	PermissionsNumber += ReadThePerission("\nAdd New Client ? y/n? ", enMainMenuePermissions::pAddNewClient);
	PermissionsNumber += ReadThePerission("\nDelete Client  ? y/n? ", enMainMenuePermissions::pDeleteClient);
	PermissionsNumber += ReadThePerission("\nUpdate Client  ? y/n? ", enMainMenuePermissions::pUpdateClient);
	PermissionsNumber += ReadThePerission("\nFind Client    ? y/n? ", enMainMenuePermissions::pFindClient);
	PermissionsNumber += ReadThePerission("\nTransaction    ? y/n? ", enMainMenuePermissions::pTransactions);
	PermissionsNumber += ReadThePerission("\nManage Users   ? y/n? ", enMainMenuePermissions::pManageUser);

	return PermissionsNumber;
}

stUserData ReadNewUser()
{
	stUserData UserData;

	cout << "\nPlease Enter Client Data: \n\n";

	// Usage of std::ws will extract allthe whitespace character
	cout << "Enter User Name? ";
	getline(cin >> ws, UserData.UserName);

	// Read User Name

	while (CheckIfTheUserExisting(UserData.UserName))
	{
		cout << "User with (" + UserData.UserName + ") already exists, Enter anouther Username? ";
		getline(cin >> ws, UserData.UserName);
	}

	// Read Password
	cout << "Enter Password? ";
	getline(cin, UserData.Password);

	char Access = 'y';

	// Read Permissions

	UserData.Permissions = ReadPermissionsToSet();

	return UserData;
}

void AddNewUser()
{

	stUserData User;
	User = ReadNewUser();
	AddDataLineToFile(UserFillName, ConvetUserDataToLineForUser(User));
}

void AddUser()
{
	char AddMoreUser = 'y';
	do
	{
		AddNewUser();

		cout << "\n\nUser Added Successfully, do you want to add more client?";
		cin >> AddMoreUser;
		cin.ignore(1, '\n');

	} while (toupper(AddMoreUser) == 'Y');
}

void ShowAddNewUserScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tAdd New User Screen";
	cout << "\n---------------------------------------------\n";
	AddUser();
}

// Save Client Function:

void SaveUsersDataToFile(string URL, vector<stUserData> vUserData)
{
	fstream TheFile;
	TheFile.open(URL, ios::out); // read mode

	if (TheFile.is_open())
	{

		for (stUserData User : vUserData)
		{

			if (User.MarkToDelete == false)
			{
				TheFile << ConvetUserDataToLineForUser(User) << endl;
			}
		}
		TheFile.close();
	}
}

// Delete User Function:

void MarkUserForDeleteByUsername(string TargetUser, vector<stUserData> &vUserData)
{

	for (stUserData &User : vUserData)
	{
		if (User.UserName == TargetUser)
		{
			User.MarkToDelete = true;
		}
	}
}

void DeleteUserByUsername(vector<stUserData> &vUsers, string Username)
{
	stUserData User;

	if (FindUserByUsername(User, Username))
	{
		PrintUserData(User);

		char Answer;

		cout << "\nAre you sure want delete this clients? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkUserForDeleteByUsername(User.UserName, vUsers);
			SaveUsersDataToFile(UserFillName, vUsers);

			// Refresh vUsers
			vUsers = LoadUserDataFromFile(UserFillName);

			cout << "\nUser Deleted Successfully.\n";
		}
	}
	else
	{
		cout << "User (" + Username + ") not Found!";
	}
}

void ShowDeleteUserScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\Delete User Screen";
	cout << "\n---------------------------------------------\n";

	vector<stUserData> vUser = LoadUserDataFromFile(UserFillName);
	string UserName = ReadUserName();

	if (UserName == "Admin")
	{
		printf("\n\nYou cannout Delete This User!\n");
		GoBackToManageMenue();
	}
	DeleteUserByUsername(vUser, UserName);
}

// Update User Info Function:

stUserData ChangeUserRecord(string Username)
{
	stUserData UserData;
	UserData.UserName = Username;

	// Usage of std::ws will extract allthe whitespace character
	cout << "Enter Password? ";
	getline(cin >> ws, UserData.Password);

	char Access = 'y';

	// Read Permissions
	cout << "\nDo you want to give full access? Y/N? ";
	cin >> Access;

	if (Access == 'y' || Access == 'Y')
	{
		UserData.Permissions = -1;
	}
	else
	{
		UserData.Permissions = ReadPermissionsToSet();
	}

	return UserData;
}

void UpdatUserForDeleteByUsername(string TargetUser, vector<stUserData> &vUserData)
{

	for (stUserData &User : vUserData)
	{
		if (User.UserName == TargetUser)
		{
			User = ChangeUserRecord(TargetUser);
			break;
		}
	}
}

void UpdateUserByUsername(vector<stUserData> &vUsers, string Username)
{
	stUserData User;

	if (FindUserByUsername(User, Username))
	{
		PrintUserData(User);

		char Answer = 'n';

		cout << "\nAre you sure want update this User? y/n ? ";

		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			cout << "\n\n";
			UpdatUserForDeleteByUsername(Username, vUsers);
			SaveUsersDataToFile(UserFillName, vUsers);

			cout << "\n\nUser Update Successfully.\n";
		}
	}
	else
	{
		cout << "\nUser with this name (" + Username + ") not Found!\n";
	}
}

void ShowUpdateUserScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tUpdate User Screen";
	cout << "\n---------------------------------------------\n";

	vector<stUserData> vUsers = LoadUserDataFromFile(UserFillName);
	string Username = ReadUserName();
	UpdateUserByUsername(vUsers, Username);
}

// Find User Function:

void ShowFindUserScreen()
{
	cout << "\n---------------------------------------------\n";
	cout << "\t\tFind User Screen";
	cout << "\n---------------------------------------------\n";

	vector<stUserData> vUsers = LoadUserDataFromFile(UserFillName);
	string Username = ReadUserName();
	stUserData User;
	if (FindUserByUsername(User, Username))
	{
		PrintUserData(User);
	}
	else
	{
		cout << "\nUser with this name (" + Username + ") not Found!\n";
	}
}

void GoBackToManageMenue()
{
	cout << "\n\nPress any key to go back to Manage User Menue...";
	system("pause>0");
	ShowMangeUsersScreen();
}

void PerformManageUserOption(enManageOpt Opt)
{
	vector<stUserData> vUsers;
	vUsers = LoadUserDataFromFile(UserFillName);

	switch (Opt)
	{
	case ShowUsers:
		system("CLS");
		ShowAllUsers();
		GoBackToManageMenue();
		break;
	case Add_New_User:
		system("CLS");
		ShowAddNewUserScreen();
		GoBackToManageMenue();
		break;
	case Delete_User:
		system("CLS");
		ShowDeleteUserScreen();
		GoBackToManageMenue();
		break;
	case Update_User:
		system("CLS");
		ShowUpdateUserScreen();
		GoBackToManageMenue();
		break;
	case Find_User:
		system("CLS");
		ShowFindUserScreen();
		GoBackToManageMenue();
		break;
	case Main_Menue:
		system("CLS");
		MainBankAppPage();
		break;
	}
}

void ShowMangeUsersScreen()
{
	system("CLS");
	PrintManageUsers();
	PerformManageUserOption(ReadManageUserMenueOption("Chosose what do you want to do? [1 to 6]? "));
}

int main()
{
	LoginScreen();

	return 0;
}