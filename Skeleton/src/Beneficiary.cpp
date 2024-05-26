#include "Beneficiary.h"
#include <utility>

// Constructor
Beneficiary::Beneficiary(int id, const std::string &name, int locationDistance, int maxRequests)
    : id(id), name(name), locationDistance(locationDistance), maxRequests(maxRequests), requestsId({}) {}
// Destructor
Beneficiary::~Beneficiary() = default;

// Copy Constructor
Beneficiary::Beneficiary(const Beneficiary& other)
    : id(other.id), name(other.name), locationDistance(other.locationDistance), maxRequests(other.maxRequests), requestsId(other.requestsId) {}

// Copy Assignment Operator
Beneficiary& Beneficiary::operator=(const Beneficiary& other) {
    if (this != &other) {
        // Note: id, locationDistance, and maxRequests are const and cannot be reassigned
        // const_cast is used here for demonstration purposes but should be avoided in practice
        const_cast<int&>(this->id) = other.id;
        const_cast<string&>(this->name) = other.name;
        const_cast<int&>(this->locationDistance) = other.locationDistance;
        const_cast<int&>(this->maxRequests) = other.maxRequests;
        requestsId = other.requestsId;
    }
    return *this;
}

// Move Constructor
Beneficiary::Beneficiary(Beneficiary&& other) noexcept
    : id(other.id), name(std::move(other.name)), locationDistance(other.locationDistance), maxRequests(other.maxRequests), requestsId(std::move(other.requestsId)) {
    const_cast<int&>(other.id) = 0;
    const_cast<int&>(other.locationDistance) = 0;
    const_cast<int&>(other.maxRequests) = 0;
}

// Move Assignment Operator
Beneficiary& Beneficiary::operator=(Beneficiary&& other) noexcept {
    if (this != &other) {
        const_cast<int&>(this->id) = other.id;
        const_cast<string&>(this->name) = std::move(other.name);
        const_cast<int&>(this->locationDistance) = other.locationDistance;
        const_cast<int&>(this->maxRequests) = other.maxRequests;
        requestsId = std::move(other.requestsId);

        const_cast<int&>(other.id) = 0;
        const_cast<int&>(other.locationDistance) = 0;
        const_cast<int&>(other.maxRequests) = 0;
    }
    return *this;
}

//Getter functions
const std::string &Beneficiary::getName() const {
    return name;
}

int Beneficiary::getId() const {
    return id;
}

int Beneficiary::getBeneficiaryDistance() const {
    return locationDistance;
}

int Beneficiary::getMaxRequests() const {
    return maxRequests;
}

int Beneficiary::getNumRequests() const {
    return requestsId.size();
}

//Other functions
bool Beneficiary::canMakeRequest() const {
    return getNumRequests() < maxRequests;
}

const vector<int> &Beneficiary::getRequestsIds() const {
    return requestsId;
}

int Beneficiary::addRequest(int RequestId) {
    if (canMakeRequest()) {
        requestsId.push_back(RequestId);
        return RequestId;
    }
    return -1; // if request wasn't added
}

/////////////////////////////////// HospitalBeneficiary class implementation ////////////////////////////////
HospitalBeneficiary::HospitalBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}

HospitalBeneficiary* HospitalBeneficiary::clone() const {
    return new HospitalBeneficiary(*this);
}

///////////////////////////////////// ClinicBeneficiary class implementation /////////////////////////////////
ClinicBeneficiary::ClinicBeneficiary(int id, const string &name, int locationDistance, int maxRequests)
    : Beneficiary(id, name, locationDistance, maxRequests) {}

ClinicBeneficiary* ClinicBeneficiary::clone() const {
    return new ClinicBeneficiary(*this);
}

