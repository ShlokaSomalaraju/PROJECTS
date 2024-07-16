#include <iostream>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
using namespace std;

class Stock {
public:
    string symbol;
    double price;
    int volume;
    Stock() : symbol(""), price(0.0), volume(0) {}
    Stock(const string& symbol, double price, int volume);
    void updatePrice(double newPrice);
    void updateVolume(int change);
};

class Trader {
public:
    string name;
    unordered_map<string, int> portfolio;
    Trader(const string& name);
    void placeOrder(const string& symbol, int quantity, bool isBuy);
};

void Trader::placeOrder(const string& symbol, int quantity, bool isBuy){
    if(isBuy == true){
        cout << name << " wants to buy " << quantity << " shares of " << symbol << std::endl;
        portfolio[symbol] += quantity; // Update the portfolio
    }
    else{
        cout << name << " wants to sell " << quantity << " shares of " << symbol << std::endl;
        if(portfolio[symbol] >= quantity){
            portfolio[symbol] -= quantity; // Update the portfolio
        }
        else{
            cout << "Not enough shares to sell" << std::endl;
        }
    }
}

class Market {
private:
    unordered_map<string, Stock> stocks;
    mutex marketMutex;
public:
    void addStock(const Stock& stock){
        marketMutex.lock();
        stocks[stock.symbol] = stock;
        marketMutex.unlock();
    }

    void executeOrder(Trader& trader, const string& symbol, int quantity, bool isBuy){
        marketMutex.lock();
        if(stocks.find(symbol) != stocks.end()){
            Stock& stock = stocks[symbol];
            //Trader wants to buy
            if(isBuy == true){
                if(stock.volume >= quantity) {
                    stock.updateVolume(-quantity);
                    trader.placeOrder(symbol, quantity, isBuy);
                    cout << "Order executed: Bought " << quantity << " shares of " << symbol << std::endl;
                }
                else{
                    cout << "Not enough stock volume to buy" << std::endl;
                }
            }
            // Trader wants to sell to company itself
            else{
                stock.updateVolume(quantity);
                trader.placeOrder(symbol, quantity, isBuy);
                cout << "Order executed: Sold " << quantity << " shares of " << symbol << std::endl;
            }
        }
        else{
            cout << "Stock symbol not found" << std::endl;
        }
    }
};


class Transaction {
public:
    string traderName;
    string stockSymbol;
    int quantity;
    bool isBuy;
    Transaction(const string& traderName, const string& stockSymbol, int quantity, bool isBuy);
};

int main() {
    Market market;
    market.addStock(Stock("AAPL", 150.0, 10000));
    market.addStock(Stock("GOOGL", 2800.0, 5000));

    Trader trader1("Alice");
    Trader trader2("Bob");

    thread t1([&market, &trader1]() {
        trader1.placeOrder("AAPL", 100, true);
        market.executeOrder(trader1, "AAPL", 100, true);
    });

    thread t2([&market, &trader2]() {
        trader2.placeOrder("GOOGL", 50, false);
        market.executeOrder(trader2, "GOOGL", 50, false);
    });

    t1.join();
    t2.join();

    return 0;
}

