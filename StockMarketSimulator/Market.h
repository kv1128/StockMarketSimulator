#ifndef _MARKET_H_
#define _MARKET_H_

#include <vector>
#include <string>
#include <map>
#include <list>

struct MarketAsset;
struct WalletAsset;

class Modifier
{
public:
	virtual void update(void) = 0;
	void addAsset(MarketAsset* asset);
protected:
	std::vector<MarketAsset*> assets;
	int getRandomNumber(int a, int b);
};

class ModifierRandom : public Modifier
{
public:
	void update(void) override;
};

class ModifierTrandDown : public Modifier
{
public:
	ModifierTrandDown(void);
	void update(void) override;
private:
	int downTrandSpeed;
};

class ModifierTrandUp : public Modifier
{
public:
	ModifierTrandUp(void);
	void update(void) override;
private:
	int upTrandSpeed;
};

class Market
{
public:
	static void create(void);
	static void shutdown(void);
	static Market* get(void);
	void show(void);
	bool load(void);
	unsigned int getPrice(const std::string& name);
	void update(void);
	void buy(const std::string& name, unsigned int quantity);
	void sell(const std::string& name, unsigned int quantity);
	void buyLimit(const std::string& name, unsigned int quantity, unsigned int price);
	void sellLimit(const std::string& name, unsigned int quantity, unsigned int price);
	void createPriceModifiers(void);
	bool isRefreshAllowed(void);
	void setRefreshAllowed(bool RefreshAllowed);
private:
	static Market* instance;
	bool refreshAllowed;
	std::map<std::string, MarketAsset> tradeAssets;
	std::vector<Modifier*> priceModifiers;
	std::list<WalletAsset> buyOrders;
	std::list<WalletAsset> sellOrders;
	Market(void);
	~Market(void);
};

#endif	// _MARKET_H_