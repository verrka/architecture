#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Container {
public:
    Container(int id, int weight) : ID(id), weight(weight) {}

    virtual double consumption() const = 0;

    bool equals(const Container& other) const {
        return (typeid(*this) == typeid(other)) && (ID == other.ID) && (weight == other.weight);
    }

    int getID() const { return ID; }
    int getWeight() const { return weight; }

private:
    int ID;
    int weight;
};

class BasicContainer : public Container {
public:
    BasicContainer(int id, int weight) : Container(id, weight) {}

    double consumption() const override {
        return 2.50 * getWeight();
    }
};

class HeavyContainer : public Container {
public:
    HeavyContainer(int id, int weight) : Container(id, weight) {}

    double consumption() const override = 0;  // Pure virtual function
};

class RefrigeratedContainer : public HeavyContainer {
public:
    RefrigeratedContainer(int id, int weight) : HeavyContainer(id, weight) {}

    double consumption() const override {
        return 5.00 * getWeight();
    }
};

class LiquidContainer : public HeavyContainer {
public:
    LiquidContainer(int id, int weight) : HeavyContainer(id, weight) {}

    double consumption() const override {
        return 4.00 * getWeight();
    }
};

class IPort {
public:
    virtual void incomingShip(class Ship* s) = 0;
    virtual void outgoingShip(class Ship* s) = 0;
};

class IShip {
public:
    virtual bool sailTo(class Port* p) = 0;
    virtual void reFuel(double newFuel) = 0;
    virtual bool load(Container* cont) = 0;
    virtual bool unLoad(Container* cont) = 0;
};

class Port : public IPort {
public:
    Port(int ID, double latitude, double longitude) : ID(ID), latitude(latitude), longitude(longitude) {}

    void incomingShip(Ship* s) override {
        current.push_back(s);
    }

    void outgoingShip(Ship* s) override {
        history.push_back(s);
    }

    double getDistance(const Port& other) const {
        // Implement geospatial distance calculation
        // This is just a placeholder, actual implementation depends on your requirements
        return 0.0;
    }

    int getID() const { return ID; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }

    void printPort() const {
        std::cout << "Port ID: " << ID << " (" << latitude << ", " << longitude << ")" << std::endl;
        // Print containers in the port
        std::cout << "{BasicContainer, HeavyContainer, RefrigeratedContainer, LiquidContainer}: [";
        for (const auto& cont : containers) {
            std::cout << cont->getID() << ", ";
        }
        std::cout << "]" << std::endl;

        // Print ships in the port
        for (const auto& ship : current) {
            std::cout << "Ship ID: " << ship->getID() << " FUEL_LEFT: " << std::fixed << std::setprecision(2)
                << ship->getFuel() << std::endl;
            ship->printContainers();
        }
    }

private:
    int ID;
    double latitude;
    double longitude;
    std::vector<Container*> containers;
    std::vector<Ship*> history;
    std::vector<Ship*> current;
};

class Ship : public IShip {
public:
    Ship(int portID, int totalWeightCapacity, int maxNumAllContainers,
        int maxNumHeavyContainers, int maxNumRefrigeratedContainers,
        int maxNumLiquidContainers, double fuelConsumptionPerKM)
        : ID(GenerateUniqueShipID()), fuel(0.0), totalWeightCapacity(totalWeightCapacity),
        maxNumAllContainers(maxNumAllContainers), maxNumHeavyContainers(maxNumHeavyContainers),
        maxNumRefrigeratedContainers(maxNumRefrigeratedContainers),
        maxNumLiquidContainers(maxNumLiquidContainers), fuelConsumptionPerKM(fuelConsumptionPerKM) {
        currentPort = portID;
    }

    bool sailTo(Port* p) override {
        double requiredFuel = CalculateRequiredFuel();
        if (fuel >= requiredFuel) {
            // Update ship's position and consume fuel
            fuel -= requiredFuel;
            currentPort->outgoingShip(this);
            p->incomingShip(this);
            currentPort = p;
            return true;
        }
        else {
            return false;
        }
    }

    void reFuel(double newFuel) override {
        fuel += newFuel;
    }

    bool load(Container* cont) override {
        // Check if the ship has enough capacity
        if (containers.size() < maxNumAllContainers) {
            containers.push_back(cont);
            return true;
        }
        else {
            return false;
        }
    }

    bool unLoad(Container* cont) override {
        // Find the container in the ship and remove it
        auto it = std::find_if(containers.begin(), containers.end(),
            [cont](Container* c) { return c->equals(*cont); });

        if (it != containers.end()) {
            containers.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    void printContainers() const {
        for (const auto& cont : containers) {
            std::cout << "  Container ID: " << cont->getID() << " Weight: " << cont->getWeight() << std::endl;
        }
    }

    int getID() const { return ID; }
    double getFuel() const { return fuel; }

private:
    int ID;
    double fuel;
    int currentPort;
    int totalWeightCapacity;
    int maxNumAllContainers;
    int maxNumHeavyContainers;
    int maxNumRefrigeratedContainers;
    int maxNumLiquidContainers;
    double fuelConsumptionPerKM;
    std::vector<Container*> containers;

    static int GenerateUniqueShipID() {
        // Implement a method to generate unique ship IDs
        // This is just a placeholder, actual implementation depends on your requirements
        static int nextID = 0;
        return nextID++;
    }

    double CalculateRequiredFuel() const {
        // Calculate fuel required based on ship's consumption and containers
        double totalConsumption = fuelConsumptionPerKM;
        for (const auto& cont : containers) {
            totalConsumption += cont->consumption();
        }
        return totalConsumption * Port(currentPort, 0, 0).getDistance(Port(0, 0, 0));
    }
};

// Read input from JSON and print output
int main() {
    // Read input from JSON file
    std::ifstream input("input.json");
    json inputData;
    input >> inputData;

    //  Creating a Port from JSON data
    Port port(inputData["Port"]["ID"], inputData["Port"]["lat"], inputData["Port"]["lon"]);

    //  Creating a Ship from JSON data
    Ship ship(inputData["Ship"]["portID"], inputData["Ship"]["totalWeightCapacity"],
        inputData["Ship"]["maxNumAllContainers"], inputData["Ship"]["maxNumHeavyContainers"],
        inputData["Ship"]["maxNumRefrigeratedContainers"], inputData["Ship"]["maxNumLiquidContainers"],
        inputData["Ship"]["fuelConsumptionPerKM"]);

    //  Creating a BasicContainer from JSON data
    BasicContainer basicContainer(inputData["Container"]["ID"], inputData["Container"]["weight"]);

    //  Loading the BasicContainer into the Ship
    if (ship.load(&basicContainer)) {
        std::cout << "Container loaded successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to load container. Ship at maximum capacity." << std::endl;
    }

    //Unloading the BasicContainer from the Ship
    if (ship.unLoad(&basicContainer)) {
        std::cout << "Container unloaded successfully!" << std::endl;
    }
    else {
        std::cout << "Failed to unload container. Container not found on the ship." << std::endl;
    }

    //  Ship sails to another port
    Port destinationPort(inputData["DestinationPort"]["ID"], inputData["DestinationPort"]["lat"],
        inputData["DestinationPort"]["lon"]);
    if (ship.sailTo(&destinationPort)) {
        std::cout << "Ship sailed successfully to the destination port!" << std::endl;
    }
    else {
        std::cout << "Ship couldn't sail to the destination port. Insufficient fuel." << std::endl;
    }

    // Print output in JSON format
    json outputData;
    outputData["Port"]["ID"] = port.getID();
    outputData["Port"]["latitude"] = port.getLatitude();
    outputData["Port"]["longitude"] = port.getLongitude();

    outputData["Ship"]["ID"] = ship.getID();
    outputData["Ship"]["fuelLeft"] = ship.getFuel();
    

    // Print the final output
    port.printPort();

    return 0;
}


