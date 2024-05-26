#include "SupplyRequest.h"

// Constructor implementation
SupplyRequest::SupplyRequest(int id, int beneficiaryId, int distance)
    : id(id), beneficiaryId(beneficiaryId), distance(distance), status(RequestStatus::PENDING),
      inventoryManagerId(NO_VOLUNTEER), CourierId(NO_VOLUNTEER) {}

// Getter functions
int SupplyRequest::getId() const {
    return id;
}
int SupplyRequest::getBeneficiaryId() const {
    return beneficiaryId;
}
int SupplyRequest::getInventoryManagerId() const {
    return inventoryManagerId;
}
int SupplyRequest::getCourierId() const {
    return CourierId;
}
RequestStatus SupplyRequest::getRequestStatus() const {
    return status;
}
int SupplyRequest::getDistance() const {
    return distance;
}


// Setter functions
void SupplyRequest::setStatus(RequestStatus status) {
    this->status = status;
}
void SupplyRequest::setInventoryManagerId(int inventoryManagerId) {
    this->inventoryManagerId = inventoryManagerId;
}
void SupplyRequest::setCourierId(int CourierId) {
    this->CourierId = CourierId;
}

const string SupplyRequest:: statusToString(RequestStatus status) const {
    switch(status) {
        case RequestStatus::PENDING:
            return "Pending";
        case RequestStatus::COLLECTING:
            return "Collecting";
        case RequestStatus::ON_THE_WAY:
            return "On the Way";
        case RequestStatus::DONE:
            return "Done";
        default:
            return "Unknown";
    }
}

const string SupplyRequest :: toString() const{
    string imv = "None";
    int imvId = getInventoryManagerId();
    if (imvId != -1){
        imv = std::to_string(imvId);
    }
    string cv = "None";
    int cvId = getCourierId();
    if (cvId != -1){
        cv = std::to_string(cvId);
    }

    string output =
        "RequestId: " + std::to_string(id) +"\n"
        "RequestStatus: " + statusToString(getRequestStatus()) + "\n"
        "BeneficiaryID: " + std::to_string(beneficiaryId) + "\n"
        "InventoryManagerId: " + imv + "\n"
        "CourierId: " + cv;
    
    return output;
}

const string SupplyRequest :: closeInfo() const{
    string output =
        "RequestId: " + std::to_string(id) + " ,"
        "BeneficiaryId: " + std::to_string(beneficiaryId) + ", "
        "RequestStatus: " + statusToString(getRequestStatus());
    
    return output;
}

SupplyRequest* SupplyRequest :: clone() const {
    return new SupplyRequest (*this);
}