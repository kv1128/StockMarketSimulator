#include "XMLLoader.h"
#include "tinyxml2.h"
#include <iostream>

using namespace tinyxml2;

bool XMLLoader::loadWallet(unsigned int* money, std::vector<WalletAsset>* assets)
{
	XMLDocument doc;
	if (doc.LoadFile("wallet.xml") != XML_SUCCESS)
	{
		std::cout << "Wallet not found" << std::endl;
		return false;
	}

	XMLElement* wallet = doc.FirstChildElement("wallet");

	XMLElement* dollars = wallet->FirstChildElement("money");

	*money = std::stoi(dollars->GetText());

	XMLElement* stocks = wallet->FirstChildElement("assets");

	for (XMLElement* child = stocks->FirstChildElement("asset"); child; child = child->NextSiblingElement())
	{
		WalletAsset asset;

		for (XMLElement* Property = child->FirstChildElement("property"); Property; Property = Property->NextSiblingElement())
		{
			std::string Key = Property->Attribute("name");
			std::string Value = Property->GetText();

			if (Key == "name")
				asset.name = Value;
			else if (Key == "quantity")
				asset.quantity = std::stoi(Value);
			else if (Key == "price")
				asset.price = std::stoi(Value);
		}

		assets->push_back(asset);
	}

	return true;
}

bool XMLLoader::loadPrices(std::map<std::string, MarketAsset>* assets)
{
	XMLDocument doc;
	if (doc.LoadFile("prices.xml") != XML_SUCCESS)
	{
		std::cout << "Prices not found" << std::endl;
		return false;
	}

	XMLElement* prices = doc.FirstChildElement("prices");

	for (XMLElement* child = prices->FirstChildElement("asset"); child; child = child->NextSiblingElement())
	{
		std::string name;
		unsigned int price;

		for (XMLElement* Property = child->FirstChildElement("property"); Property; Property = Property->NextSiblingElement())
		{
			std::string Key = Property->Attribute("name");
			std::string Value = Property->GetText();

			if (Key == "name")
				name = Value;
			else if (Key == "price")
				price = std::stoi(Value);
		}

		MarketAsset asset;
		asset.price = price;
		assets->insert(std::pair<std::string, MarketAsset>(name, asset));
	}

	return true;
}