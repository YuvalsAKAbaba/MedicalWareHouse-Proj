#include "Action.h"
#include "MedicalWarehouse.h"
#include <iostream>

extern MedicalWarehouse *backup;
string CoreAction::actionStatusToString() const {
    switch (getStatus()) {
        case ActionStatus::COMPLETED:
            return "COMPLETED";
        case ActionStatus::ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}
std::string beneficiaryTypeToString(beneficiaryType type) {
    switch (type) {
        case beneficiaryType::Hospital:
            return "Hospital";
        case beneficiaryType::Clinic:
            return "Clinic";
        default:
            return "UNKNOWN";
    }
}
beneficiaryType stringToBeneficiaryType(const std::string& type) {
    if (type == "Hospital") {
        return beneficiaryType::Hospital;
    } else if (type == "Clinic") {
        return beneficiaryType::Clinic;
    } else {
        throw std::invalid_argument("Invalid beneficiary type");
    }
}


////////////////////////////////////////// CoreAction class //////////////////////////////////////////////////
// Constructor 
CoreAction::CoreAction() : errorMsg("ERROR: "), status(ActionStatus::COMPLETED) {}


// Destructor
CoreAction::~CoreAction() {}


// Copy constructor
CoreAction::CoreAction(const CoreAction& other)
    : errorMsg(other.errorMsg), status(other.status) {}


// Copy assignment operator
CoreAction& CoreAction::operator=(const CoreAction& other) {
    if (this != &other) {
        errorMsg = other.errorMsg;
        status = other.status;
    }
    return *this;
}


// Move constructor
CoreAction::CoreAction(CoreAction&& other) noexcept
    : errorMsg(std::move(other.errorMsg)), status(other.status) {
    other.status = ActionStatus::ERROR;
}


// Move assignment operator
CoreAction& CoreAction::operator=(CoreAction&& other) noexcept {
    if (this != &other) {
        errorMsg = std::move(other.errorMsg);
        status = other.status;
        other.status = ActionStatus::ERROR;
    }
    return *this;
}


//Getter functions
ActionStatus CoreAction::getStatus() const {
    return status;
}
string CoreAction::getErrorMsg() const {
    return errorMsg;
}


//Other CoreAction functions 
void CoreAction::complete() {
    status = ActionStatus::COMPLETED;
}
void CoreAction::error(string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << errorMsg << std::endl;
}


////////////////////////////////////////// SimulateStep class //////////////////////////////////////////////////
SimulateStep::SimulateStep(int _numOfSteps) : numOfSteps(_numOfSteps) {}
void SimulateStep::act(MedicalWarehouse &medWareHouse) {
    medWareHouse.addAction(this);
    for(int i = 0;i < numOfSteps; i++)
    {
        medWareHouse.step();
    }
}
std::string SimulateStep::toString() const {
    string output = "SimulateStep: " + std::to_string(numOfSteps) + " steps";
    if (getStatus() == ActionStatus::COMPLETED){
        return output + "COMPLETED";
    }
    return output + "ERROR";
}
SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}


////////////////////////////////////////// AddRequest class //////////////////////////////////////////////////
AddRequest::AddRequest(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void AddRequest::act(MedicalWarehouse &medWareHouse){
    if(beneficiaryId > medWareHouse.getBeneficiaryCounter())
        error("Cannot place this order");
    else{
        Beneficiary& beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
        if(!beneficiary.canMakeRequest())
            error("Cannot place this order");
        else{
            int requestId = medWareHouse.getRequestCounter();            
            beneficiary.addRequest(requestId);
            int distance = beneficiary.getBeneficiaryDistance();
            SupplyRequest* newSupplyRequest = new SupplyRequest(requestId, beneficiaryId, distance);
            newSupplyRequest->setStatus(RequestStatus::PENDING);
            medWareHouse.addRequest(newSupplyRequest);
        }
    }
    medWareHouse.addAction(this);
} 
string AddRequest::toString() const {
    return "Beneficiary ID: " + std::to_string(beneficiaryId);
}
AddRequest* AddRequest::clone() const {
    return new AddRequest(*this);
}


////////////////////////////////////////// RegisterBeneficiary class //////////////////////////////////////////////////
RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests)
    : beneficiaryName(beneficiaryName), beneficiaryType(stringToBeneficiaryType(beneficiaryType)), distance(distance), maxRequests(maxRequests) {}

void RegisterBeneficiary::act(MedicalWarehouse &medWareHouse) {
    Beneficiary* newBeneficiary = nullptr;
    if(beneficiaryType == beneficiaryType::Hospital){
        newBeneficiary = new HospitalBeneficiary(medWareHouse.getBeneficiaryCounter(), beneficiaryName, distance, maxRequests);
    }
    if(beneficiaryType == beneficiaryType::Clinic){
        newBeneficiary = new ClinicBeneficiary(medWareHouse.getBeneficiaryCounter(), beneficiaryName, distance, maxRequests);
    }
    medWareHouse.addBeneficiary(newBeneficiary);
    complete();
    medWareHouse.addAction(this);
} 
RegisterBeneficiary* RegisterBeneficiary::clone() const {
    return new RegisterBeneficiary(*this);
}
string RegisterBeneficiary::toString() const {
    return "RegisterBeneficiary: " +beneficiaryName + " (" + beneficiaryTypeToString(beneficiaryType) + ")";
}


////////////////////////////////////////// PrintRequestStatus class //////////////////////////////////////////////////
PrintRequestStatus::PrintRequestStatus(int requestId): requestId(requestId) {}
void PrintRequestStatus:: act(MedicalWarehouse& medWareHouse){
    medWareHouse.addAction(this);
    if (medWareHouse.requestExists(requestId)) 
    {
        SupplyRequest& request = medWareHouse.getRequest(requestId);
        std::cout << request.toString() << std::endl;
        
    }
    else {
        error("Request doesn't exist");
        return;
    }
    complete();
    medWareHouse.addAction(this);
}
PrintRequestStatus* PrintRequestStatus::clone() const {
    return new PrintRequestStatus(*this);
}
string PrintRequestStatus::toString() const {
    return "RequestStatus: " +  std::to_string(requestId); //Add requestStatus!!
}


////////////////////////////////////////// PrintBeneficiaryStatus class //////////////////////////////////////////////////
PrintBeneficiaryStatus::PrintBeneficiaryStatus(int beneficiaryId): beneficiaryId(beneficiaryId){}
void PrintBeneficiaryStatus:: act(MedicalWarehouse &medWareHouse) 
{
    if(medWareHouse.beneficiaryExists(beneficiaryId))
    {
      Beneficiary& beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
      std::cout<< "BeneficiaryID: " << beneficiaryId << std::endl;
      for (int requestId : beneficiary.getRequestsIds())
        {
            SupplyRequest& request = medWareHouse.getRequest(requestId);
            std::cout<< "RequestId: " << requestId << std::endl;
            std::cout<< "RequestStatus: " << request.statusToString(request.getRequestStatus())<< std::endl;
        }
        std::cout << "RequestsLeft: " << beneficiary.getMaxRequests() - beneficiary.getNumRequests()<<std::endl;
        complete();
    }
    else
    {
        error("Beneficiary doesn't exist");
    }
    medWareHouse.addAction(this);
}

PrintBeneficiaryStatus* PrintBeneficiaryStatus::clone() const {
    return new PrintBeneficiaryStatus(*this);
}

string PrintBeneficiaryStatus::toString() const {
    return "BeneficiaryStatus: " +  std::to_string(beneficiaryId) +  " ," + actionStatusToString();
}


////////////////////////////////////////// PrintVolunteerStatus class //////////////////////////////////////////////////
PrintVolunteerStatus::PrintVolunteerStatus(int volunteerId) : volunteerId(volunteerId) {}
void PrintVolunteerStatus::act(MedicalWarehouse &medWareHouse){
    if(medWareHouse.volunteerExists(volunteerId)){
        Volunteer& volunteer = medWareHouse.getVolunteer(volunteerId);
        std::cout<< "VolunteerID: " << volunteerId <<std::endl;
        bool isBusy = volunteer.isBusy();     
        std::cout<< "IsBusy: " << std::boolalpha << isBusy <<std::endl;          
        if(isBusy){    
            std::cout<< "RequestID: " << volunteer.getActiveRequestId() <<std::endl;
            if(const InventoryManagerVolunteer* imv = dynamic_cast<const InventoryManagerVolunteer*>(&volunteer)) {
                std::cout << "TimeLeft: " << imv->getTimeLeft() << std::endl;
            }
            if (const CourierVolunteer* cv = dynamic_cast<const CourierVolunteer*>(&volunteer)) {
                std::cout << "DistanceLeft: " << cv->getDistanceLeft() << std::endl;
            }           
        }
    }        
    else{
        error("Volunteer does not exist");
    }
    medWareHouse.addAction(this);
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}

string PrintVolunteerStatus::toString() const {
    return "VolunteerStatus: " +  std::to_string(volunteerId) +  " ," + actionStatusToString();
}    


////////////////////////////////////////// PrintActionsLog class //////////////////////////////////////////////////
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(MedicalWarehouse &medWareHouse) {
    vector<CoreAction*> actionsLog = medWareHouse.getActions();
    vector<string> actionsStringLog = medWareHouse.getActionsString();
    if (actionsLog.size() != actionsStringLog.size()) {
            std::cout << "Error: Vectors have different lengths. ActionsString: " + std::to_string(actionsStringLog.size()) + ", Actions: " + std::to_string(actionsLog.size())<< std::endl;            
    }
    for (size_t i = 0; i < actionsStringLog.size(); ++i) { //added
            // Print String           
        if(actionsStringLog[i] != "log"){
            std::cout << actionsStringLog[i]+", ";
            // Print corresponding CoreAction object details
            if (actionsLog[i]) {
                std::cout << actionsLog[i]->actionStatusToString()<< std::endl;
            } 
        }            
    }    
    complete();
    medWareHouse.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}
string PrintActionsLog::toString() const {
    return "log COMPLETED";
}


////////////////////////////////////////// Close class //////////////////////////////////////////////////
Close::Close() {}
void Close::act(MedicalWarehouse &medWareHouse) 
{
    medWareHouse.addAction(this);
    medWareHouse.close();    
}

Close *Close::clone() const {
    return new Close(*this);
}
string Close::toString() const 
{
   return "close COMPLETED";
}


////////////////////////////////////////// BackupWarehouse class //////////////////////////////////////////////////
BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(MedicalWarehouse &medWareHouse) 
{
    if (backup != nullptr) 
    {
        delete backup;
    }
    backup = new MedicalWarehouse(medWareHouse);
    complete();    
    medWareHouse.addAction(this);
}

BackupWareHouse *BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}
string BackupWareHouse::toString() const {
    return "backup COMPLETED";
}


////////////////////////////////////////// RestoreWarehouse class //////////////////////////////////////////////////
RestoreWareHouse::RestoreWareHouse() {}
void RestoreWareHouse::act(MedicalWarehouse &medWareHouse) 
{
    if (backup == nullptr) {
       error("No backup available");
       getErrorMsg();
    }
    else{
        medWareHouse = *backup;
        complete();
    }
    medWareHouse.addAction(this);
}

RestoreWareHouse *RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const {
    return "Restore " + actionStatusToString();
}

