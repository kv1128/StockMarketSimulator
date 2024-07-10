#ifndef _XMLLOADER_H_
#define _XMLLOADER_H_

#include <string>
#include <vector>
#include <map>

struct WalletAsset
{
	std::string name;
	unsigned int quantity;
	int price;
};

struct MarketAsset
{
	int price;
};

class XMLLoader
{
public:
	static bool loadWallet(unsigned int* money, std::vector<WalletAsset>* assets);
	static bool loadPrices(std::map<std::string, MarketAsset>* assets);
};

#endif	// _XMLLOADER_H_