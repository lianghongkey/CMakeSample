#pragma once
#include <pybind11/pybind11.h>



class Account
{
public:
  Account();
  ~Account();
  void deposit(const double amount);
  void withdraw(const double amount);
  double get_balance() const;

private:
  double balance;
};


PYBIND11_MODULE(account, m)
{
  pybind11::class_<Account>(m, "Account")
      .def(pybind11::init())
      .def("deposit", &Account::deposit)
      .def("withdraw", &Account::withdraw)
      .def("get_balance", &Account::get_balance);
}
