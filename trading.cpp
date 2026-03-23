#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <ctime>
#include <cstdlib>

using namespace std;

class Stock
{
public:
    string name;
    double price;

    Stock(string n,double p)
    {
        name=n;
        price=p;
    }
};

class Order
{
public:
    string user;
    string stock;
    int quantity;
    double price;

    Order(string u,string s,int q,double p)
    {
        user=u;
        stock=s;
        quantity=q;
        price=p;
    }
};

class BuyCompare
{
public:
    bool operator()(Order a,Order b)
    {
        return a.price < b.price;
    }
};

class SellCompare
{
public:
    bool operator()(Order a,Order b)
    {
        return a.price > b.price;
    }
};

class User
{
public:
    string name;
    double balance;
    map<string,int> portfolio;
    vector<string> history;

    User(string n)
    {
        name=n;
        balance=10000;
    }
};

class TradingEngine
{
private:

    vector<Stock> market;
    map<string,User> users;

    priority_queue<Order,vector<Order>,BuyCompare> buyOrders;
    priority_queue<Order,vector<Order>,SellCompare> sellOrders;

public:

    TradingEngine()
    {
        market.push_back(Stock("TATA",750));
        market.push_back(Stock("INFY",1450));
        market.push_back(Stock("RELIANCE",2400));
        market.push_back(Stock("HDFC",1600));
        market.push_back(Stock("WIPRO",420));

        srand(time(0));
    }

    void addUser()
    {
        string name;
        cout<<"Enter username: ";
        cin>>name;

        users.insert({name,User(name)});

        cout<<"User created\n";
    }

    void showUsers()
    {
        cout<<"\nUsers\n";

        for(auto &u:users)
        {
            cout<<u.first<<" Balance:"<<u.second.balance<<endl;
        }
    }

    void updatePrices()
    {
        for(auto &s:market)
        {
            int change=rand()%41-20;

            s.price+=change;

            if(s.price<50)
                s.price=50;
        }
    }

    void showMarket()
    {
        updatePrices();

        cout<<"\nMarket\n";

        for(int i=0;i<market.size();i++)
        {
            cout<<i+1<<" "<<market[i].name
                <<" Price:"<<market[i].price<<endl;
        }
    }

    double getPrice(string stock)
    {
        for(auto &s:market)
        {
            if(s.name==stock)
                return s.price;
        }

        return 0;
    }

    void placeBuyOrder()
    {
        string user,stock;
        int qty;

        cout<<"User: ";
        cin>>user;

        cout<<"Stock: ";
        cin>>stock;

        cout<<"Quantity: ";
        cin>>qty;

        double price=getPrice(stock);

        if(users[user].balance < price*qty)
        {
            cout<<"Insufficient balance\n";
            return;
        }

        Order order(user,stock,qty,price);

        buyOrders.push(order);

        matchOrders();
    }

    void placeSellOrder()
    {
        string user,stock;
        int qty;

        cout<<"User: ";
        cin>>user;

        cout<<"Stock: ";
        cin>>stock;

        cout<<"Quantity: ";
        cin>>qty;

        if(users[user].portfolio[stock] < qty)
        {
            cout<<"Not enough shares\n";
            return;
        }

        double price=getPrice(stock);

        Order order(user,stock,qty,price);

        sellOrders.push(order);

        matchOrders();
    }

    void matchOrders()
    {
        while(!buyOrders.empty() && !sellOrders.empty())
        {
            Order buy=buyOrders.top();
            Order sell=sellOrders.top();

            if(buy.price >= sell.price)
            {
                buyOrders.pop();
                sellOrders.pop();

                int tradedQty=min(buy.quantity,sell.quantity);

                double tradePrice=sell.price;

                users[buy.user].balance -= tradePrice*tradedQty;
                users[sell.user].balance += tradePrice*tradedQty;

                users[buy.user].portfolio[buy.stock]+=tradedQty;
                users[sell.user].portfolio[sell.stock]-=tradedQty;

                string buyRecord="BOUGHT "+buy.stock+" "+to_string(tradedQty);
                string sellRecord="SOLD "+sell.stock+" "+to_string(tradedQty);

                users[buy.user].history.push_back(buyRecord);
                users[sell.user].history.push_back(sellRecord);

                cout<<"Trade executed "<<tradedQty<<" shares "<<buy.stock<<endl;

                if(buy.quantity > tradedQty)
                {
                    buy.quantity -= tradedQty;
                    buyOrders.push(buy);
                }

                if(sell.quantity > tradedQty)
                {
                    sell.quantity -= tradedQty;
                    sellOrders.push(sell);
                }
            }
            else
                break;
        }
    }

    void showPortfolio()
    {
        string user;
        cout<<"Enter user: ";
        cin>>user;

        cout<<"\nPortfolio\n";

        double total=0;

        for(auto &p:users[user].portfolio)
        {
            double price=getPrice(p.first);

            double value=p.second*price;

            cout<<p.first<<" "<<p.second<<" value:"<<value<<endl;

            total+=value;
        }

        cout<<"Balance:"<<users[user].balance<<endl;

        cout<<"Total value:"<<total+users[user].balance<<endl;
    }

    void showHistory()
    {
        string user;

        cout<<"User:";
        cin>>user;

        cout<<"\nHistory\n";

        for(auto &h:users[user].history)
        {
            cout<<h<<endl;
        }
    }

    void menu()
    {
        int choice;

        while(true)
        {
            cout<<"\n1 Add User\n";
            cout<<"2 Show Market\n";
            cout<<"3 Buy Order\n";
            cout<<"4 Sell Order\n";
            cout<<"5 Portfolio\n";
            cout<<"6 History\n";
            cout<<"7 Show Users\n";
            cout<<"8 Exit\n";

            cin>>choice;

            switch(choice)
            {
                case 1: addUser(); break;
                case 2: showMarket(); break;
                case 3: placeBuyOrder(); break;
                case 4: placeSellOrder(); break;
                case 5: showPortfolio(); break;
                case 6: showHistory(); break;
                case 7: showUsers(); break;
                case 8: return;
                default: cout<<"Invalid\n";
            }
        }
    }
};

int main()
{
    TradingEngine engine;

    engine.menu();

    return 0;
}