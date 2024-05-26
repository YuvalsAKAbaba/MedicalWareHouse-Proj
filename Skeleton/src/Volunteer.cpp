#include "Volunteer.h"
#include "MedicalWarehouse.h"
#include <utility>

// Constructor
Volunteer::Volunteer(int id, const std::string &name) : id(id), name(name), activeRequestId(NO_REQUEST), completedRequestId(NO_REQUEST) {}
// Destructor
Volunteer::~Volunteer() = default;
// Copy Constructor
Volunteer::Volunteer(const Volunteer &other)
    : id(other.id), name(other.name), completedRequestId(other.completedRequestId), activeRequestId(other.activeRequestId) {}
// Copy Assignment Operator
Volunteer& Volunteer::operator=(const Volunteer &other) {
    if (this != &other) {
        completedRequestId = other.completedRequestId;
        activeRequestId = other.activeRequestId;
    }
    return *this;
}
// Move Constructor
Volunteer::Volunteer(Volunteer &&other) noexcept
    : id(other.id), name(std::move(other.name)), completedRequestId(other.completedRequestId), activeRequestId(other.activeRequestId) {
    other.completedRequestId = NO_REQUEST;
    other.activeRequestId = NO_REQUEST;
}
// Move Assignment Operator
Volunteer& Volunteer::operator=(Volunteer &&other) noexcept {
    if (this != &other) {
        completedRequestId = other.completedRequestId;
        activeRequestId = other.activeRequestId;

        other.completedRequestId = NO_REQUEST;
        other.activeRequestId = NO_REQUEST;        
    }
    return *this;
}

// Getter functions
int Volunteer::getId() const {
    return id;
}

const std::string &Volunteer::getName() const {
    return name;
}

int Volunteer::getActiveRequestId() const {
    return activeRequestId;
}

int Volunteer::getCompletedRequestId() const {
    return completedRequestId;
}

bool Volunteer::isBusy() const {
    return activeRequestId != NO_REQUEST;
}


//////////////////////////////InventoryManagerVolunteer class implementation///////////////////////////////////
InventoryManagerVolunteer::InventoryManagerVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(coolDown) {}

InventoryManagerVolunteer* InventoryManagerVolunteer::clone() const {
    return new InventoryManagerVolunteer(*this);
}

void InventoryManagerVolunteer::step() {
    if (decreaseTimeLeft() && activeRequestId != NO_REQUEST) {
        completedRequestId = activeRequestId;
        activeRequestId = NO_REQUEST;
    }
}

int InventoryManagerVolunteer::getCoolDown() const { return coolDown; }

int InventoryManagerVolunteer::getTimeLeft() const { return timeLeft; }

bool InventoryManagerVolunteer::decreaseTimeLeft() {
    if (timeLeft > 0) {
        --timeLeft;
    }
    return timeLeft == 0;
}

bool InventoryManagerVolunteer::hasRequestsLeft() const { return activeRequestId != NO_REQUEST; }

bool InventoryManagerVolunteer::canTakeRequest(const SupplyRequest &request) const {
    return !isBusy();
}

void InventoryManagerVolunteer::acceptRequest(const SupplyRequest &request) {
    activeRequestId = request.getId();
    timeLeft = coolDown;
}

std::string InventoryManagerVolunteer::toString() const {
    return "InventoryManagerVolunteer: " + getName();
}

///////////////////////////////////CourierVolunteer class implementation////////////////////////////////////////

CourierVolunteer::CourierVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

CourierVolunteer* CourierVolunteer::clone() const {
    return new CourierVolunteer(*this);
}

int CourierVolunteer::getDistanceLeft() const { return distanceLeft; }
int CourierVolunteer::getMaxDistance() const { return maxDistance; }
int CourierVolunteer::getDistancePerStep() const { return distancePerStep; }

bool CourierVolunteer::decreaseDistanceLeft() {
    distanceLeft = distanceLeft - distancePerStep;
    if(distanceLeft <= 0)
    {
        distanceLeft = 0;
        return true;
    }
    else
        return false;
}

bool CourierVolunteer::hasRequestsLeft() const { return activeRequestId != NO_REQUEST; }

bool CourierVolunteer::canTakeRequest(const SupplyRequest &request) const {
    return !isBusy() && request.getDistance() <= maxDistance;
}

void CourierVolunteer::acceptRequest(const SupplyRequest &request) {
    activeRequestId = request.getId();
    distanceLeft = request.getDistance();       
}

void CourierVolunteer::step() {    
    if (decreaseDistanceLeft() && activeRequestId != NO_REQUEST) {
        completedRequestId = activeRequestId;
        activeRequestId = NO_REQUEST;
    }    
}

std::string CourierVolunteer::toString() const {
    return "CourierVolunteer: " + getName();
}
