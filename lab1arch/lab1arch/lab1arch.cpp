#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Operator;

class Bill {
private:
    double limitingAmount;
    double currentDebt;

public:
    Bill(double limitingAmount) : limitingAmount(limitingAmount), currentDebt(0.0) {}

    bool check(double amount) {
        return (currentDebt + amount) <= limitingAmount;
    }

    void add(double amount) {
        currentDebt += amount;
    }

    void pay(double amount) {
        if (amount > currentDebt) {
            currentDebt = 0.0;
        }
        else {
            currentDebt -= amount;
        }
    }

    void changeTheLimit(double amount) {
        limitingAmount = amount;
    }

    double getLimitingAmount() const {
        return limitingAmount;
    }

    double getCurrentDebt() const {
        return currentDebt;
    }
};

class Operator {
private:
    int ID;
    double talkingCharge;
    double messageCost;
    double networkCharge;
    int discountRate;

public:
    Operator(int ID, double talkingCharge, double messageCost, double networkCharge, int discountRate)
        : ID(ID), talkingCharge(talkingCharge), messageCost(messageCost), networkCharge(networkCharge), discountRate(discountRate) {}

    double calculateTalkingCost(int minute, const Bill& bill, int age) const {
        double cost = minute * talkingCharge;
        if (age < 18 || age > 65) {
            cost *= (1.0 - discountRate / 100.0);
        }
        if (!bill.check(cost)) {
            return 0.0;
        }
        return cost;
    }

    double calculateMessageCost(int quantity, const Operator& otherOperator, const Bill& bill, int age) const {
        double cost = quantity * messageCost;
        if (this == &otherOperator) {
            cost *= (1.0 - discountRate / 100.0);
        }
        if (!bill.check(cost)) {
            return 0.0;
        }
        return cost;
    }

    double calculateNetworkCost(double amount, const Bill& bill) const {
        double cost = amount * networkCharge;
        if (!bill.check(cost)) {
            return 0.0;
        }
        return cost;
    }

    int getID() const {
        return ID;
    }

    double getTalkingCharge() const {
        return talkingCharge;
    }

    double getMessageCost() const {
        return messageCost;
    }

    double getNetworkCharge() const {
        return networkCharge;
    }

    int getDiscountRate() const {
        return discountRate;
    }
};

class Customer {
private:
    int ID;
    std::string name;
    int age;
    Operator* operatorPtr;
    Bill* billPtr;

public:
    Customer(int ID, const std::string& name, int age, Operator* operatorPtr, Bill* billPtr)
        : ID(ID), name(name), age(age), operatorPtr(operatorPtr), billPtr(billPtr) {}

    void talk(int minute, Customer& other) {
        double cost = operatorPtr->calculateTalkingCost(minute, *billPtr, age);
        if (cost > 0.0) {
            billPtr->add(cost);
            other.billPtr->add(cost);
            std::cout << name << " talked to " << other.name << " for " << minute << " minutes. Cost: $" << cost << std::endl;
        }
        else {
            std::cout << "Talking not allowed. Exceeds bill limit." << std::endl;
        }
    }

    void message(int quantity, Customer& other) {
        double cost = operatorPtr->calculateMessageCost(quantity, *other.operatorPtr, *billPtr, age);
        if (cost > 0.0) {
            billPtr->add(cost);
            other.billPtr->add(cost);
            std::cout << name << " sent " << quantity << " messages to " << other.name << ". Cost: $" << cost << std::endl;
        }
        else {
            std::cout << "Messaging not allowed. Exceeds bill limit." << std::endl;
        }
    }

    void connection(double amount) {
        double cost = operatorPtr->calculateNetworkCost(amount, *billPtr);
        if (cost > 0.0) {
            billPtr->add(cost);
            std::cout << name << " connected to the internet. Cost: $" << cost << std::endl;
        }
        else {
            std::cout << "Internet connection not allowed. Exceeds bill limit." << std::endl;
        }
    }

    void payBill(double amount) {
        if (amount > 0.0) {
            if (amount > billPtr->getCurrentDebt()) {
                std::cout << "Warning: Paying more than the current debt. Excess will not be refunded." << std::endl;
            }
            billPtr->pay(amount);
            std::cout << name << " paid $" << amount << " of the bill. Remaining debt: $" << billPtr->getCurrentDebt() << std::endl;
        }
        else {
            std::cout << "Invalid payment amount. Payment must be greater than zero." << std::endl;
        }
    }

    void changeOperator(Operator* newOperator) {
        operatorPtr = newOperator;
        std::cout << name << " changed operator to " << newOperator->getID() << std::endl;
    }

    void changeBillLimit(double newLimit) {
        billPtr->changeTheLimit(newLimit);
        std::cout << name << "'s bill limit changed to $" << newLimit << std::endl;
    }

    int getID() const {
        return ID;
    }

    const std::string& getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    Operator* getOperator() const {
        return operatorPtr;
    }

    Bill* getBill() const {
        return billPtr;
    }
};

// Function to read input from a JSON file
void readInputFromJSON(const std::string& filename, std::vector<Customer>& customers, std::vector<Operator>& operators, std::vector<Bill>& bills) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return;
    }

    json inputData;
    input >> inputData;

    
}

// Function to write output to a JSON file
void writeOutputToJSON(const std::string& filename, const std::vector<Customer>& customers) {
    json outputData;

    

    std::ofstream output(filename);
    output << std::setw(4) << outputData; // Pretty print JSON
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input.json>" << std::endl;
        return 1;
    }

    const std::string inputFilename = "input.json";
    const std::string outputFilename = "output.json";

    std::vector<Customer> customers;
    std::vector<Operator> operators;
    std::vector<Bill> bills;

    readInputFromJSON(inputFilename, customers, operators, bills);

    // Perform actions based on the given input (as in the previous code)

    writeOutputToJSON(outputFilename, customers);

    return 0;
}
