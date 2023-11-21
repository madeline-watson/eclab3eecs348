#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

class Account {
protected:
  string accountNumber;
  string accountHolder;
  double balance;

public:
  Account(string accNumber, string accHolder, double bal)
      : accountNumber(accNumber), accountHolder(accHolder), balance(bal) {}

  virtual void displayDetails() const {
    cout << "Account Details for Account ID " << accountNumber << ":" << endl;
    cout << "   Holder: " << accountHolder << endl;
    cout << "   Balance: $" << fixed << setprecision(2) << balance << endl;
  }

  void deposit(double amount) {
    balance += amount;
    cout << "Deposit successful." << endl;
  }

  virtual void withdraw(double amount) {
    if (balance >= amount) {
      balance -= amount;
      cout << "Withdrawal successful." << endl;
    } else {
      cout << "Insufficient funds." << endl;
    }
  }

  virtual double getBalance() const { 
    return balance; 
  }
};

class SavingsAccount : public Account {
private:
  static const double MIN_BALANCE;
  double interestRate;

public:
  SavingsAccount(string accNumber, string accHolder, double bal,
                 double interest)
      : Account(accNumber, accHolder, bal), interestRate(interest) {}

  void displayDetails() const override {
    Account::displayDetails();
    cout << "   Interest Rate: " << fixed << setprecision(2)
         << interestRate * 100 << "%" << endl
         << endl;
  }

  void withdraw(double amount) override {
    if (balance - amount >= MIN_BALANCE) {
      Account::withdraw(amount);
    } else {
      cout << "Minimum balance exceeded." << endl;
    }
  }
};

class CurrentAccount : public Account {
private:
  double overdraftLimit;

public:
  CurrentAccount(string accNumber, string accHolder, double bal,
                 double overdraftLim)
      : Account(accNumber, accHolder, bal), overdraftLimit(overdraftLim) {}

  void displayDetails() const override {
    Account::displayDetails();
    cout << "   Overdraft Limit: $" << fixed << setprecision(2)
         << overdraftLimit << endl
         << endl;
  }

  void withdraw(double amount) override {
    if (balance - amount >= -overdraftLimit) {
      Account::withdraw(amount);
    } else {
      cout << "Overdraft limit exceeded." << endl;
    }
  }
};

const double SavingsAccount::MIN_BALANCE = 500.0;

CurrentAccount operator+(CurrentAccount currentAccount,
                         const SavingsAccount &savingsAccount) {
  double transferAmount = savingsAccount.getBalance();
  if (transferAmount <= 0) {
    cout << "Cannot transfer non-positive amount." << endl;
    return currentAccount;
  }

  if (currentAccount.getBalance() + transferAmount >= 0) {
    currentAccount.deposit(transferAmount);
    const_cast<SavingsAccount &>(savingsAccount).withdraw(transferAmount);
  } else {
    cout << "Transfer exceeds overdraft limit." << endl;
  }
  return currentAccount;
}

ostream &operator<<(ostream &os, const Account &acc) {
  acc.displayDetails();
  return os;
}

int main() {
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    savings.displayDetails();
    current.displayDetails();

    savings.deposit(500);
    current.withdraw(1000);

    savings.displayDetails();
    current.displayDetails();

    // Transfer 300 from savings to current
    current = current + savings;

    savings.displayDetails();
    current.displayDetails();

    return 0;
}