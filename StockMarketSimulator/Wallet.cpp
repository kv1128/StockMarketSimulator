#include "Wallet.h"
#include "XMLLoader.h"
#include "Market.h"
#include <iostream>

Wallet* Wallet::instance = nullptr;

void Wallet::create(void)
{
	instance = new Wallet;
}

void Wallet::shutdown(void)
{
	delete instance;
}

Wallet* Wallet::get(void)
{
	return instance;
}

void Wallet::show(void)
{
	unsigned int total = money;

	std::map<std::string, unsigned int> assets;

	for (WalletAsset& asset : walletAssets)
	{
		assets[asset.name] += asset.quantity;
	}

	for (std::map<std::string, unsigned int>::const_iterator it = assets.cbegin(); it != assets.cend(); ++it)
	{
		unsigned int totalAssetValue = it->second * Market::get()->getPrice(it->first);
		total += totalAssetValue;
	}

	std::cout << "Total - " << total << "$" << std::endl;

	std::cout << "Money - " << money << "$" << std::endl;

	if (assets.size())
	{
		std::cout << "Assets: " << std::endl;

		for (std::map<std::string, unsigned int>::const_iterator it = assets.cbegin(); it != assets.cend(); ++it)
		{
			unsigned int totalAssetValue = it->second * Market::get()->getPrice(it->first);
			std::cout << it->first << " - " << it->second << ", " << totalAssetValue << "$" << std::endl;
		}
	}
}

bool Wallet::load(void)
{
	return XMLLoader::loadWallet(&money, &walletAssets);
}

void Wallet::withdrawAllAssets(void)
{
	walletAssets.clear();
}

bool Wallet::withdraw(const std::string& name, unsigned int quantity)
{
	for (std::vector<WalletAsset>::iterator it = walletAssets.begin(); it != walletAssets.end(); ++it)
	{
		WalletAsset& asset = *it;

		if (asset.name == name)
		{
			if (asset.quantity >= quantity)
			{
				asset.quantity -= quantity;

				if (asset.quantity == 0)
					walletAssets.erase(it);

				return true;
			}
			else
			{
				std::cout << "Not enough assets" << std::endl;
				return false;
			}
		}
	}

	std::cout << "Asset not found" << std::endl;
	return false;
}

void Wallet::deposit(const std::string& name, unsigned int quantity)
{
	unsigned int price = Market::get()->getPrice(name);
	if (!price)
	{
		std::cout << "Asset not found" << std::endl;
		return;
	}

	for (std::vector<WalletAsset>::iterator it = walletAssets.begin(); it != walletAssets.end(); ++it)
	{
		WalletAsset& asset = *it;

		if (asset.name == name)
		{
			asset.quantity += quantity;
			return;
		}
	}

	WalletAsset asset;
	asset.name = name;
	asset.price = price;
	asset.quantity = quantity;
	walletAssets.push_back(asset);
}

unsigned int Wallet::getMoney(void) const
{
	return money;
}

void Wallet::setMoney(unsigned int Money)
{
	money = Money;
}

void Wallet::pnl(void) const
{
	unsigned int total = 0;

	std::map<std::string, int> assetsPNL;

	for (const WalletAsset& asset : walletAssets)
	{
		int pnl = asset.quantity * (Market::get()->getPrice(asset.name) - asset.price);
		assetsPNL[asset.name] += pnl;
		total += pnl;
	}

	std::cout << "Total - " << getPNLString(total) << "$" << std::endl;

	for (std::map<std::string, int>::const_iterator it = assetsPNL.cbegin(); it != assetsPNL.cend(); ++it)
	{
		int totalAssetPNL = it->second;
		std::cout << it->first << " - " << getPNLString(totalAssetPNL) << "$" << std::endl;
	}
}

Wallet::Wallet(void) : money(0)
{
}

Wallet::~Wallet(void)
{
}

std::string Wallet::getPNLString(int amount) const
{
	std::string value = std::to_string(amount);

	if (amount)
	{
		std::string resetColor = "\033[0m";
		std::string color = amount > 0 ? "\033[1;32m" : "\033[0;31m";
		return color + value + resetColor;
	}

	return value;
}