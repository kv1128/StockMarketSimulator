#ifndef _WALLET_H_
#define _WALLET_H_

#include <vector>
#include <string>

struct WalletAsset;

class Wallet
{
public:
	static void create(void);
	static void shutdown(void);
	static Wallet* get(void);
	void show(void);
	bool load(void);
	void withdrawAllAssets(void);
	bool withdraw(const std::string& name, unsigned int quantity);
	void deposit(const std::string& name, unsigned int quantity);
	unsigned int getMoney(void) const;
	void setMoney(unsigned int Money);
	void pnl(void) const;
private:
	static Wallet* instance;
	unsigned int money;
	std::vector<WalletAsset> walletAssets;
	Wallet(void);
	~Wallet(void);
	std::string getPNLString(int amount) const;
};

#endif	// _WALLET_H_