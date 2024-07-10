#include "Commands.h"
#include "Wallet.h"
#include "Market.h"
#include <iostream>

bool CommandExit::run(void)
{
	return 0;
}

std::string CommandInput::getName(void)
{
	std::cout << "Name:" << std::endl;

	std::string name;
	std::cin >> name;

	return name;
}

int CommandInput::getQuantity(void)
{
	std::cout << "Quantity:" << std::endl;

	std::string quantity;
	std::cin >> quantity;

	return std::stoi(quantity);
}

int CommandInput::getPrice(void)
{
	std::cout << "Price:" << std::endl;

	std::string price;
	std::cin >> price;

	return std::stoi(price);
}

bool CommandWithdrawAll::run(void)
{
	Wallet::get()->withdrawAllAssets();

	return 1;
}

bool CommandWithdraw::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();

	if (quantity > 0)
		Wallet::get()->withdraw(name, quantity);

	return 1;
}

bool CommandDeposit::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();

	if (quantity > 0)
		Wallet::get()->deposit(name, quantity);

	return 1;
}

bool CommandShowWallet::run(void)
{
	Wallet::get()->show();

	return 1;
}

bool CommandShowMarket::run(void)
{
	Market::get()->setRefreshAllowed(true);
	Market::get()->show();

	return 1;
}

bool CommandBuy::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();

	if (quantity > 0)
		Market::get()->buy(name, quantity);

	return 1;
}

bool CommandSell::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();

	if (quantity > 0)
		Market::get()->sell(name, quantity);

	return 1;
}

bool CommandBuyLimit::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();
	int price = getPrice();

	if (quantity > 0 && price > 0)
		Market::get()->buyLimit(name, quantity, price);

	return 1;
}

bool CommandSellLimit::run(void)
{
	std::string name = getName();
	int quantity = getQuantity();
	int price = getPrice();

	if (quantity > 0 && price > 0)
		Market::get()->sellLimit(name, quantity, price);

	return 1;
}

bool CommandPNL::run(void)
{
	Wallet::get()->pnl();

	return 1;
}