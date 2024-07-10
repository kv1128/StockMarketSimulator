#include "Market.h"
#include "XMLLoader.h"
#include "Wallet.h"
#include <iostream>
#include <thread>

void Modifier::addAsset(MarketAsset* asset)
{
	assets.push_back(asset);
}

int Modifier::getRandomNumber(int a, int b)
{
	return a + (std::rand() % (b - a + 1));
}

void ModifierRandom::update(void)
{
	for (MarketAsset* asset : assets)
	{
		asset->price += getRandomNumber(0, 6);
		if (asset->price < 5)
			asset->price = 5;
		else if (asset->price > 500)
			asset->price = 500;
	}
}

ModifierTrandDown::ModifierTrandDown(void)
{
	downTrandSpeed = -getRandomNumber(0, 6);
}

void ModifierTrandDown::update(void)
{
	for (MarketAsset* asset : assets)
	{
		asset->price += downTrandSpeed + getRandomNumber(0, 3);
		if (asset->price < 5)
			asset->price = 5;
	}
}

ModifierTrandUp::ModifierTrandUp(void)
{
	upTrandSpeed = getRandomNumber(0, 6);
}

void ModifierTrandUp::update(void)
{
	for (MarketAsset* asset : assets)
	{
		asset->price += upTrandSpeed + getRandomNumber(0, 3);
		if (asset->price > 500)
			asset->price = 500;
	}
}

Market* Market::instance = nullptr;

void Market::create(void)
{
	instance = new Market;

	std::thread th([&]()
		{
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds{ 5 });
				Market::get()->update();
			}
		});
	th.detach();
}

void Market::shutdown(void)
{
	delete instance;
}

Market* Market::get(void)
{
	return instance;
}

void Market::show(void)
{
	std::cout << "Stocks:" << std::endl;

	for (std::map<std::string, MarketAsset>::const_iterator it = tradeAssets.cbegin(); it != tradeAssets.cend(); ++it)
	{
		const MarketAsset& asset = it->second;
		std::cout << it->first << " - " << asset.price << "$" << std::endl;
	}

	if (buyOrders.size())
	{
		std::cout << "Buy orders:" << std::endl;

		for (std::list<WalletAsset>::const_iterator it = buyOrders.cbegin(); it != buyOrders.cend(); ++it)
		{
			std::cout << it->name << " - " << it->quantity << " - " << it->price << "$" << std::endl;
		}
	}

	if (sellOrders.size())
	{
		std::cout << "Sell orders:" << std::endl;

		for (std::list<WalletAsset>::const_iterator it = sellOrders.cbegin(); it != sellOrders.cend(); ++it)
		{
			std::cout << it->name << " - " << it->quantity << " - " << it->price << "$" << std::endl;
		}
	}
}

bool Market::load(void)
{
	return XMLLoader::loadPrices(&tradeAssets);
}

unsigned int Market::getPrice(const std::string& name)
{
	if (tradeAssets.find(name) == tradeAssets.end())
		return 0;

	return tradeAssets[name].price;
}

void Market::update(void)
{
	for (Modifier* modifier : priceModifiers)
		modifier->update();

	if (buyOrders.size())
	{
		for (std::list<WalletAsset>::const_iterator it = buyOrders.cbegin(); it != buyOrders.cend(); ++it)
		{
			if (tradeAssets[it->name].price <= it->price)
			{
				buy(it->name, it->quantity);
				buyOrders.erase(it);
				break;
			}
		}
	}

	if (sellOrders.size())
	{
		for (std::list<WalletAsset>::const_iterator it = sellOrders.cbegin(); it != sellOrders.cend(); ++it)
		{
			if (tradeAssets[it->name].price >= it->price)
			{
				sell(it->name, it->quantity);
				sellOrders.erase(it);
				break;
			}
		}
	}

	if (refreshAllowed)
	{
		system("cls");
		show();
	}
}

void Market::buy(const std::string& name, unsigned int quantity)
{
	unsigned int price = getPrice(name);
	if (!price)
	{
		std::cout << "Asset not found" << std::endl;
		return;
	}

	unsigned int totalPrice = quantity * price;
	unsigned int money = Wallet::get()->getMoney();

	if (totalPrice > money)
	{
		std::cout << "Not enough money" << std::endl;
		return;
	}

	Wallet::get()->setMoney(money - totalPrice);
	Wallet::get()->deposit(name, quantity);

	std::cout << "Bought " << quantity << " " << name << " at price " << price << std::endl;
}

void Market::sell(const std::string& name, unsigned int quantity)
{
	if (!Wallet::get()->withdraw(name, quantity))
		return;

	unsigned int price = getPrice(name);
	unsigned int totalPrice = quantity * price;

	Wallet::get()->setMoney(Wallet::get()->getMoney() + totalPrice);

	std::cout << "Sold " << quantity << " " << name << " at price " << price << std::endl;
}

void Market::buyLimit(const std::string& name, unsigned int quantity, unsigned int price)
{
	WalletAsset asset;
	asset.name = name;
	asset.quantity = quantity;
	asset.price = price;
	buyOrders.push_back(asset);
}

void Market::sellLimit(const std::string& name, unsigned int quantity, unsigned int price)
{
	WalletAsset asset;
	asset.name = name;
	asset.quantity = quantity;
	asset.price = price;
	sellOrders.push_back(asset);
}

void Market::createPriceModifiers(void)
{
	priceModifiers[0]->addAsset(&tradeAssets["NW"]);
	priceModifiers[1]->addAsset(&tradeAssets["FTR"]);
	priceModifiers[2]->addAsset(&tradeAssets["RND"]);
}

bool Market::isRefreshAllowed(void)
{
	return refreshAllowed;
}

void Market::setRefreshAllowed(bool RefreshAllowed)
{
	refreshAllowed = RefreshAllowed;
}

Market::Market(void) : refreshAllowed(false)
{
	ModifierRandom* random = new ModifierRandom();
	priceModifiers.push_back(random);
	ModifierTrandDown* trandDown = new ModifierTrandDown();
	priceModifiers.push_back(trandDown);
	ModifierTrandUp* trandUp = new ModifierTrandUp();
	priceModifiers.push_back(trandUp);
}

Market::~Market(void)
{
	for (Modifier* modifier : priceModifiers)
		delete modifier;
}