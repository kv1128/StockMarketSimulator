#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <string>

class Command
{
public:
	virtual bool run(void) = 0;
};

class CommandExit : public Command
{
public:
	bool run(void) override;
};

class CommandInput : public Command
{
protected:
	std::string getName(void);
	int getQuantity(void);
	int getPrice(void);
};

class CommandWithdrawAll : public Command
{
public:
	bool run(void) override;
};

class CommandWithdraw : public CommandInput
{
public:
	bool run(void) override;
};

class CommandDeposit : public CommandInput
{
public:
	bool run(void) override;
};

class CommandShowWallet : public Command
{
public:
	bool run(void) override;
private:
};

class CommandShowMarket : public Command
{
public:
	bool run(void) override;
};

class CommandBuy : public CommandInput
{
public:
	bool run(void) override;
};

class CommandSell : public CommandInput
{
public:
	bool run(void) override;
};

class CommandBuyLimit : public CommandInput
{
public:
	bool run(void) override;
};

class CommandSellLimit : public CommandInput
{
public:
	bool run(void) override;
};

class CommandPNL : public Command
{
public:
	bool run(void) override;
};

#endif	// _COMMANDS_H_