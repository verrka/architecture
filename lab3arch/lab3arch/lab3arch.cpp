#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class IDGenerator {
public:
    static int generateID() {
        static int nextID = 0;
        return nextID++;
    }
};

class Item {
public:
    Item(int containerID, double weight, int count)
        : containerID(containerID), weight(weight), count(count), ID(IDGenerator::generateID()) {}

    virtual double getTotalWeight() const {
        return weight * count;
    }

    int getID() const {
        return ID;
    }

protected:
    int ID;
    double weight;
    int count;
    int containerID;
};

class Small : public Item {
public:
    Small(int containerID, double weight, int count, bool specificAttribute)
        : Item(containerID, weight, count), specificAttribute(specificAttribute) {}

    bool getSpecificAttribute() const {
        return specificAttribute;
    }

private:
    bool specificAttribute;
};

class Heavy : public Item {
public:
    Heavy(int containerID, double weight, int count, bool specificAttribute)
        : Item(containerID, weight, count), specificAttribute(specificAttribute) {}

    bool getSpecificAttribute() const {
        return specificAttribute;
    }

private:
    bool specificAttribute;
};

class Refrigerated : public Item {
public:
    Refrigerated(int containerID, double weight, int count, bool specificAttribute)
        : Item(containerID, weight, count), specificAttribute(specificAttribute) {}

    bool getSpecificAttribute() const {
        return specificAttribute;
    }

private:
    bool specificAttribute;
};

class Liquid : public Item {
public:
    Liquid(int containerID, double weight, int count, bool specificAttribute)
        : Item(containerID, weight, count), specificAttribute(specificAttribute) {}

    bool getSpecificAttribute() const {
        return specificAttribute;
    }

private:
    bool specificAttribute;
};

class ItemFactory {
public:
    virtual Item* createItem(int containerID, double weight, int count, bool specificAttribute) = 0;
};

class SmallFactory : public ItemFactory {
public:
    Item* createItem(int containerID, double weight, int count, bool specificAttribute) override {
        return new Small(containerID, weight, count, specificAttribute);
    }
};

class HeavyFactory : public ItemFactory {
public:
    Item* createItem(int containerID, double weight, int count, bool specificAttribute) override {
        return new Heavy(containerID, weight, count, specificAttribute);
    }
};

class RefrigeratedFactory : public ItemFactory {
public:
    Item* createItem(int containerID, double weight, int count, bool specificAttribute) override {
        return new Refrigerated(containerID, weight, count, specificAttribute);
    }
};

class LiquidFactory : public ItemFactory {
public:
    Item* createItem(int containerID, double weight, int count, bool specificAttribute) override {
        return new Liquid(containerID, weight, count, specificAttribute);
    }
};

class ShipBuilder {
public:
    virtual void buildFuelConsumption() = 0;
    virtual void buildTotalWeightCapacity() = 0;
    virtual void buildMaxNumAllContainers() = 0;
    virtual void buildMaxNumHeavyContainers() = 0;
    virtual void buildMaxNumRefrigeratedContainers() = 0;
    virtual void buildMaxNumLiquidContainers() = 0;
    virtual void buildSpecificShipComponent() = 0;
    virtual Ship* getShip() = 0;
};

class LightWeightShipBuilder : public ShipBuilder {
private:
    LightWeightShip* ship;

public:
    LightWeightShipBuilder() : ship(new LightWeightShip()) {}

    void buildFuelConsumption() override {
        ship->setFuelConsumptionPerKM(3.0);
    }

    void buildTotalWeightCapacity() override {
        ship->setTotalWeightCapacity(500);
    }

    void buildMaxNumAllContainers() override {
        ship->setMaxNumAllContainers(50);
    }

    void buildMaxNumHeavyContainers() override {
        ship->setMaxNumHeavyContainers(20);
    }

    void buildMaxNumRefrigeratedContainers() override {
        ship->setMaxNumRefrigeratedContainers(10);
    }

    void buildMaxNumLiquidContainers() override {
        ship->setMaxNumLiquidContainers(5);
    }

    void buildSpecificShipComponent() override {
        // LightWeightShip has no specific component
    }

    Ship* getShip() override {
        return ship;
    }
};

class MediumShipBuilder : public ShipBuilder {
private:
    MediumShip* ship;

public:
    MediumShipBuilder() : ship(new MediumShip()) {}

    void buildFuelConsumption() override {
        ship->setFuelConsumptionPerKM(4.0);
    }

    void buildTotalWeightCapacity() override {
        ship->setTotalWeightCapacity(1000);
    }

    void buildMaxNumAllContainers() override {
        ship->setMaxNumAllContainers(100);
    }

    void buildMaxNumHeavyContainers() override {
        ship->setMaxNumHeavyContainers(40);
    }

    void buildMaxNumRefrigeratedContainers() override {
        ship->setMaxNumRefrigeratedContainers(20);
    }

    void buildMaxNumLiquidContainers() override {
        ship->setMaxNumLiquidContainers(10);
    }

    void buildSpecificShipComponent() override {
       
        
    }

    Ship* getShip() override {
        return ship;
    }
};

class HeavyShipBuilder : public ShipBuilder {
private:
    HeavyShip* ship;

public:
    HeavyShipBuilder() : ship(new HeavyShip()) {}

    void buildFuelConsumption() override {
        ship->setFuelConsumptionPerKM(5.0);
    }

    void buildTotalWeightCapacity() override {
        ship->setTotalWeightCapacity(1500);
    }

    void buildMaxNumAllContainers() override {
        ship->setMaxNumAllContainers(150);
    }

    void buildMaxNumHeavyContainers() override {
        ship->setMaxNumHeavyContainers(60);
    }

    void buildMaxNumRefrigeratedContainers() override {
        ship->setMaxNumRefrigeratedContainers(30);
    }

    void buildMaxNumLiquidContainers() override {
        ship->setMaxNumLiquidContainers(15);
    }

    void buildSpecificShipComponent() override {
        
    }

    Ship* getShip() override {
        return ship;
    }
};

class ShipDirector {
private:
    ShipBuilder* builder;

public:
    ShipDirector(ShipBuilder* builder) : builder(builder) {}

    Ship* construct() {
        builder->buildFuelConsumption();
        builder->buildTotalWeightCapacity();
        builder->buildMaxNumAllContainers();
        builder->buildMaxNumHeavyContainers();
        builder->buildMaxNumRefrigeratedContainers();
        builder->buildMaxNumLiquidContainers();
        builder->buildSpecificShipComponent();
        return builder->getShip();
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
    virtual bool load(Item* item) = 0;
    virtual bool unLoad(Item* item) = 0;
    virtual void printContainers() const = 0;
    virtual int getID() const = 0;
    virtual double getFuel() const = 0;
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
       
        return 0.0;
    }

    int getID() const { return ID; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }

    void printPort() const {
        std::cout << "Port ID: " << ID << " (" << latitude << ", " << longitude << ")" << std::endl;

        // Print items in the port
        std::cout << "{Small, Heavy, Refrigerated, Liquid}: [";
        for (const auto& item : items) {
            std::cout << item->getID() << ", ";
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
    std::vector<Item*> items;
    std::vector<Ship*> history;
    std::vector<Ship*> current;
};

class Ship : public IShip {
public:
    Ship(int portID, int totalWeightCapacity, int maxNumAllContainers,
        int maxNumHeavyContainers, int maxNumRefrigeratedContainers,
        int maxNumLiquidContainers, double fuelConsumptionPerKM)
        : ID(IDGenerator::generateID()), fuel(0.0), currentPort(portID),
        totalWeightCapacity(totalWeightCapacity), maxNumAllContainers(maxNumAllContainers),
        maxNumHeavyContainers(maxNumHeavyContainers), maxNumRefrigeratedContainers(maxNumRefrigeratedContainers),
        maxNumLiquidContainers(maxNumLiquidContainers), fuelConsumptionPerKM(fuelConsumptionPerKM) {}

    bool sailTo(Port* p) override {
        double requiredFuel = calculateRequiredFuel();
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

    bool load(Item* item) override {
        // Check if the ship has enough capacity
        if (items.size() < maxNumAllContainers) {
            items.push_back(item);
            return true;
        }
        else {
            return false;
        }
    }

    bool unLoad(Item* item) override {
        // Find the item in the ship and remove it
        auto it = std::find_if(items.begin(), items.end(),
            [item](Item* i) { return i->getID() == item->getID(); });

        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        else {
            return false;
        }
    }

    void printContainers() const override {
        for (const auto& item : items) {
            std::cout << "  Item ID: " << item->getID() << " Weight: " << item->getTotalWeight() << std::endl;
        }
    }

    int getID() const override {
        return ID;
    }

    double getFuel() const override {
        return fuel;
    }

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
    std::vector<Item*> items;

    double calculateRequiredFuel() const {
        // Calculate fuel required based on ship's consumption and items
        double totalConsumption = fuelConsumptionPerKM;
        for (const auto& item : items) {
            totalConsumption += item->getTotalWeight();
        }
        return totalConsumption * Port(currentPort, 0, 0).getDistance(Port(0, 0, 0));
    }
};

// Main function
int main() {
    // Read input from JSON file
    std::ifstream input("input.json");
    json inputData;
    input >> inputData;

    // Creating items using the factory pattern
    ItemFactory* smallFactory = new SmallFactory();
    Item* smallItem = smallFactory->createItem(1, 10.0, 5, true);

    // Creating ships using the builder pattern
    ShipBuilder* lightWeightShipBuilder = new LightWeightShipBuilder();
    ShipDirector lightWeightShipDirector(lightWeightShipBuilder);
    Ship* lightWeightShip = lightWeightShipDirector.construct();

    // Print output in JSON format
    json outputData;
    outputData["Port"]["ID"] = port.getID();
    outputData["Port"]["latitude"] = port.getLatitude();
    outputData["Port"]["longitude"] = port.getLongitude();

    outputData["Ship"]["ID"] = lightWeightShip->getID();
    outputData["Ship"]["fuelLeft"] = lightWeightShip->getFuel();

    // Print the final output
    std::ofstream output("output.json");
    output << std::setw(4) << outputData << std::endl;

    // Clean up allocated memory
    delete smallFactory;
    delete smallItem;
    delete lightWeightShipBuilder;
    delete lightWeightShip;

    return 0;
}
