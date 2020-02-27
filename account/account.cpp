#include "account.hpp"
#include <pybind11/pybind11.h>




Account::Account() : balance(0.0) {}
Account::~Account() {}
void Account::deposit(const double amount) { balance += amount; }
void Account::withdraw(const double amount) { balance -= amount; }
double Account::get_balance() const { return balance; }




