#include "Wallet.h"
#include "Market.h"
#include "Commands.h"
#include <iostream>

int main(int argc, char* argv[])
{
	Wallet::create();
	Market::create();

	Wallet::get()->load();
	Market::get()->load();

	Market::get()->createPriceModifiers();

	std::map<std::string, Command*> commands;
	commands.insert(std::pair<std::string, Command*>(std::string("exit"), new CommandExit()));
	commands.insert(std::pair<std::string, Command*>(std::string("wallet"), new CommandShowWallet()));
	commands.insert(std::pair<std::string, Command*>(std::string("market"), new CommandShowMarket()));
	commands.insert(std::pair<std::string, Command*>(std::string("withdraw_all"), new CommandWithdrawAll()));
	commands.insert(std::pair<std::string, Command*>(std::string("withdraw"), new CommandWithdraw()));
	commands.insert(std::pair<std::string, Command*>(std::string("deposit"), new CommandDeposit()));
	commands.insert(std::pair<std::string, Command*>(std::string("buy"), new CommandBuy()));
	commands.insert(std::pair<std::string, Command*>(std::string("sell"), new CommandSell()));
	commands.insert(std::pair<std::string, Command*>(std::string("buy_limit"), new CommandBuyLimit()));
	commands.insert(std::pair<std::string, Command*>(std::string("sell_limit"), new CommandSellLimit()));
	commands.insert(std::pair<std::string, Command*>(std::string("pnl"), new CommandPNL()));

	std::string str;
	Command* command;
	do
	{
		std::cin >> str;
		command = nullptr;
		auto it = commands.find(str);
		if (it != commands.end())
		{
			command = it->second;
			if (Market::get()->isRefreshAllowed())
				Market::get()->setRefreshAllowed(false);
		}
		else
			std::cout << "Unknown command" << std::endl;
	} while (!command || command->run());

	for (std::map<std::string, Command*>::iterator it = commands.begin(); it != commands.end(); ++it)
	{
		Command* command = it->second;
		delete command;
	}

	Market::shutdown();
	Wallet::shutdown();

	return 0;
}