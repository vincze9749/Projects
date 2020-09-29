#include<iostream>
#include<fstream>
#include<map>
//#include<cstdlib>

using namespace std;

class Account {
private:
	long accountnumber=0;
	string name;
	long balance;
	static long nextaccnum;
public:
	long getaccnum() { return accountnumber; }
	long getbalance() { return balance; }

	Account() {};
	Account(string nname, long nbalance);
	void deposit(long ammount);
	void withdraw(long ammount);
	static void setlastaccnum(long accnum);
	static long getlastaccnum();

	friend ofstream& operator<<(ofstream& off, Account& a);
	friend ifstream& operator>>(ifstream& iff, Account& a);
	friend ostream& operator<<(ostream& o, Account& a);
};
long Account::nextaccnum = 0;

class Bank {
private:
	map <long, Account> accs;
public:
	Bank();
	Account NewAcc(string name, long balance);
	Account ShowAcc(long accnum);
	Account Deposit(long accnum, long ammount);
	Account Withdraw(long accnum, long ammount);
	void Showallacc();
	void Deleteacc(long accnum);
	~Bank();
};

Account::Account(string name, long balance) {
	nextaccnum++;
	accountnumber = nextaccnum;
	this->name = name;
	this->balance = balance;
}

void Account::deposit(long ammount) {
	balance += ammount;
}

void Account::withdraw(long ammount) {
	if (balance - ammount >= 0) {
		balance -= ammount;
	}
	else cout << "Insufficient funds." << endl;
}

void Account::setlastaccnum(long accnum) {
	nextaccnum = accnum;
}
long Account::getlastaccnum() {
	return nextaccnum;
}

ofstream& operator<<(ofstream& off, Account& a) {
	off << a.accountnumber << endl << a.name << endl << a.balance << endl;
	return off;
}
ifstream& operator>>(ifstream& iff, Account& a) {
	iff >> a.accountnumber >> a.name >> a.balance;
	return iff;
}
ostream& operator<<(ostream& o, Account& a) {
	o << a.accountnumber <<endl << a.name <<endl<< a.balance << endl;
	return o;
}

Bank::Bank() {
	Account acc;
	ifstream inpf;
	inpf.open("Accounts.txt");
	if (!inpf) {
		cout << "File not found!" << endl;
		return;
	}
	while (inpf >> acc) {
		accs.insert(pair<long, Account>(acc.getaccnum(), acc));
	}
	Account::setlastaccnum(acc.getaccnum());
	inpf.close();
}

Account Bank::NewAcc(string name, long balance) {
	ofstream outpf;
	Account acc(name, balance);
	accs.insert(pair<long, Account>(acc.getaccnum(), acc));
	outpf.open("Accounts.txt");

	map <long, Account>::iterator itr;
	for (itr = accs.begin(); itr != accs.end(); itr++) {
		outpf << itr->second;
	}
	outpf.close();
	return acc;
}

Account Bank::ShowAcc(long accnum) {
	if (accs.count(accnum) == 0) throw 1;
	else {
		map <long, Account>::iterator itr = accs.find(accnum);
		return itr->second;
	}
}

Account Bank::Deposit(long accnum, long ammount) {
	if (accs.count(accnum) == 0) throw 1;
	else {
		map <long, Account>::iterator itr = accs.find(accnum);
		itr->second.deposit(ammount);
		return itr->second;
	}
}

Account Bank::Withdraw(long accnum, long ammount) {
	if (accs.count(accnum) == 0) throw 1;
	else {
		map <long, Account>::iterator itr = accs.find(accnum);
		itr->second.withdraw(ammount);
		return itr->second;
	}
}

void Bank::Deleteacc(long accnum) {	
	if (accs.count(accnum) == 0) throw 1;
	else {
		accs.erase(accnum);
		cout << "Account has been closed" << endl;
	}
		
}

void Bank::Showallacc() {
	map <long, Account>::iterator itr;
	for (itr = accs.begin(); itr != accs.end(); itr++) {
		cout <<  endl << itr-> second;
	}
}

Bank::~Bank() {
	ofstream outpf;
	outpf.open("Accounts.txt");
	map <long, Account>::iterator itr;
	for (itr = accs.begin(); itr != accs.end(); itr++) {
		outpf << itr->second;
	}
	outpf.close();
}

int main() {

	Account a;
	Bank b;
	int choice;
	string name;
	long accnum;
	long balance;
	long amount;
	cout << "\t\tBanking system" << endl;
	do {
			cout << endl << "\tChose one of the options below:" << endl << "\t1. Open a new account" << endl << "\t2. Check Account information"
				<< endl << "\t3. Deposit" << endl << "\t4. Withdraw" << endl << "\t5. Close an account" << endl << "\t6. Show all accounts" << endl << "\t7. Exit" << endl;
			cin >> choice;
			switch (choice) {
			case 1:
				cout << "Enter name and amount" << endl;
				cin >> name >> balance;
				a = b.NewAcc(name, balance);
				cout << "Account is created" << endl;
				break;
			case 2:
				cout << "Enter account number" << endl;
				cin >> accnum;
				try {
					a = b.ShowAcc(accnum);
					cout << a << endl;
				}
				catch (int i) {
					cout << "Account not found" << endl;
				}
				break;
			case 3:
				cout << "Enter account number and amount to deposit" << endl;
				cin >> accnum >> amount;
				try {
					a = b.Deposit(accnum, amount);
					cout << a << endl;
				}
				catch (int i) {
					cout << "Account not found" << endl;
				}
				break;
			case 4:
				cout << "Enter account number and amount to withdraw" << endl;
				cin >> accnum >> amount;
				try {
					a = b.Withdraw(accnum, amount);
					cout << a << endl;
				}
				catch (int i) {
					cout << "Account not found" << endl;
				}
				break;
			case 5:
				cout << "Enter account number to close" << endl;
				cin >> accnum;
				try {
					b.Deleteacc(accnum);
				}
				catch (int i) {
					cout << "Account not found" << endl;
				}
				break;
			case 6:
				b.Showallacc();
				break;
			case 7:
				break;
			default:
				cout << "Enter a valid option" << endl;
				exit(0);
			}
	} while (choice != 7);

		return 0;
}