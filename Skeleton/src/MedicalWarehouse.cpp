#include "Action.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

// Constructor implementation
MedicalWarehouse::MedicalWarehouse(const std::string &configFilePath) : isOpen(false),actionsLog({}), volunteers({}), pendingRequests({}), inProcessRequests({}), completedRequests({}), beneficiaries({}) ,beneficiaryCounter(0), volunteerCounter(0), requestCounter(0) {
    // Load configuration from file (if needed)
    std::ifstream configFile(configFilePath);
    if (!configFile) {
        throw std::runtime_error("Could not open config file.");
    }    
    // Reading the file line by line
    std::string line;
    std::vector<std::string> words;
    while (std::getline(configFile, line)) {        
        if(!line.empty() && line[0]!='#'){
            std::istringstream iss(line);            
            //Iterate through the line (5 words each)
            std::string word;
            int i = 0;
            while (iss >> word && i<5) {
                words.push_back(word);                
                i+=1;
            }            
            if(words[0]=="beneficiary" && words.size() != 0){
                if(words[2]=="hospital"){
                    beneficiaries.push_back(new HospitalBeneficiary(beneficiaryCounter, words[1], std::stoi(words[3]), std::stoi(words[4])));                    
                }
                if(words[2]=="clinic"){
                    beneficiaries.push_back(new ClinicBeneficiary(beneficiaryCounter, words[1], std::stoi(words[3]), std::stoi(words[4])));
                }
                beneficiaryCounter++;
            }
            if(words[0]=="volunteer" && words.size() != 0){
                if(words[2]=="inventory"){
                    volunteers.push_back(new InventoryManagerVolunteer(volunteerCounter, words[1], std::stoi(words[4])));
                }
                if(words[2]=="courier"){
                    volunteers.push_back(new CourierVolunteer(volunteerCounter, words[1], std::stoi(words[3]),std::stoi(words[4])));
                }
                volunteerCounter++;
            }
            words.clear();
        }
    }
    configFile.close();
}

void MedicalWarehouse :: start(){
    open();
    std :: cout << "Enter action to execute"<< std :: endl;
    string action;
    while(isOpen){
        std::getline(std::cin,action);
        std::istringstream iss(action);
        std::vector<std::string> words;
        string firstWord;        
        while(iss>>firstWord)
        {
            words.push_back(firstWord);   
        }        
        if (words[0] == "step") {
            string number;
            number = words[1];
            if (isNumber(number)){
                int numOfSteps = std::stoi(number);
                SimulateStep* step = new SimulateStep(numOfSteps);
                step->act(*this);
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "request") {
            string number;
            number = words[1];
            if (isNumber(number)){
                int beneficiaryId = std::stoi(number);
                AddRequest* addReq = new AddRequest(beneficiaryId);                
                addReq->act(*this);
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "beneficiary") {
            string name;
            string type;
            string distance;
            string maxRequests;
            name = words[1];
            type = words[2];
            distance = words[3];
            maxRequests = words[4];
            if(isNumber(distance) && isNumber(maxRequests)){
                RegisterBeneficiary* registerBeneficiary = new RegisterBeneficiary(name, type, std::stoi(distance), std::stoi(maxRequests));
                registerBeneficiary -> act(*this);
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "requestStatus") {
            string number;
            number = words[1];
            if(isNumber(number)){
                PrintRequestStatus* requestStatus = new PrintRequestStatus(std::stoi(number));
                requestStatus -> act(*this);
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "beneficiaryStatus") {
            string number;
            number = words[1];
            if( isNumber(number)){
                PrintBeneficiaryStatus* beneficiaryStatus = new PrintBeneficiaryStatus(std::stoi(number));
                beneficiaryStatus -> act(*this);                
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "volunteerStatus") {
            string number;
            number = words[1];
            if(isNumber(number)){
                PrintVolunteerStatus* volunteerStatus = new PrintVolunteerStatus(std::stoi(number));
                volunteerStatus -> act(*this);
                actionsStringLog.push_back(action);//added
            }
            continue;
        }
        if (words[0] == "log") {
            PrintActionsLog* log = new PrintActionsLog();
            log -> act(*this);
            actionsStringLog.push_back(action);//added

        }

        if (words[0] == "close") {
            Close* cl = new Close(); 
            cl -> act(*this);
            actionsStringLog.push_back(action);//added
        }

        if (words[0] == "backup") {
            BackupWareHouse* bac = new BackupWareHouse();
            bac -> act(*this);
            this->setIsBackup();
            actionsStringLog.push_back(action);//added
        }

        if (words[0] == "restore") {
            RestoreWareHouse* restore = new RestoreWareHouse();
            restore -> act(*this);
            actionsStringLog.push_back(action);//added
        } 
    }
}

void MedicalWarehouse::addRequest(SupplyRequest* request) {
    if (isOpen) {
        pendingRequests.push_back(request);
        requestCounter++;
    } else {
        throw std::runtime_error("Warehouse is closed. Cannot add request.");
    }
}

void MedicalWarehouse :: addAction (CoreAction* action)
{
    actionsLog.push_back(action);
}

void MedicalWarehouse:: addBeneficiary(Beneficiary* beneficiary){
    beneficiaries.push_back(beneficiary);
    beneficiaryCounter++;
}

void MedicalWarehouse:: addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
    volunteerCounter++;
}

Beneficiary& MedicalWarehouse :: getBeneficiary (int beneficiaryId) const
{
    for (Beneficiary* beneficiary : beneficiaries) 
    {
        if (beneficiary->getId() == beneficiaryId) 
        {
            return *beneficiary;
        }
    }
    throw std::runtime_error("We dont have this beneficiary.");
}

Volunteer& MedicalWarehouse :: getVolunteer (int volunteerId) const
{
    for (Volunteer* volunteer : volunteers) 
    {
        if (volunteer->getId() == volunteerId) 
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("We dont have this volunteer.");
}

// Retrieves a request by ID
SupplyRequest& MedicalWarehouse::getRequest(int requestId) const {
    for (auto* request : pendingRequests) {
        if (request->getId() == requestId) {
            return *request;
        }
    }
    for (auto* request : inProcessRequests) {
        if (request->getId() == requestId) {
            return *request;
        }
    }
    for (auto* request : completedRequests) {
        if (request->getId() == requestId) {
            return *request;
        }
    }
    throw std::runtime_error("Request not found.");
}

//Exist functions
bool MedicalWarehouse:: requestExists(int id) const{
    return (id<=requestCounter && id>=0);
}

bool MedicalWarehouse:: volunteerExists(int id) const{
    return (id<=volunteerCounter && id>=0);
}

bool MedicalWarehouse:: beneficiaryExists(int id) const{
    return (id<=beneficiaryCounter && id>=0);
}

const std::vector<CoreAction*>& MedicalWarehouse::getActions() const {
    return actionsLog;
}
const std::vector<string> & MedicalWarehouse::getActionsString() const {//added
    return actionsStringLog;
}

//Getters for the counters
const int MedicalWarehouse::getBeneficiaryCounter() const {
    return beneficiaryCounter;
}
const int MedicalWarehouse::getVolunteerCounter() const {
    return volunteerCounter;
}
const int MedicalWarehouse::getRequestCounter() const{
    return requestCounter;
}


//Getters for the vectors
const vector<SupplyRequest*>& MedicalWarehouse :: getPending() const{
    return pendingRequests;
}
const vector<SupplyRequest*>& MedicalWarehouse :: getProgress() const{
    return inProcessRequests;
}
const vector<SupplyRequest*>& MedicalWarehouse :: getCompleted() const{
    return completedRequests;
}


bool MedicalWarehouse:: getIsBackup() const{
    return isBackup;
}


void MedicalWarehouse::setIsBackup() {
    isBackup = true;
}


bool MedicalWarehouse :: isNumber(string& num)
{
    for (char c : num) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

void MedicalWarehouse::step(){ 
    //handling the pending requests    
    auto requestPend = pendingRequests.begin();
    while (requestPend != pendingRequests.end()) {   
        SupplyRequest* reqP = *requestPend;
        if (reqP->getRequestStatus() == RequestStatus::PENDING && canContinue(*reqP, "InventoryManagerVolunteer")){ 
            requestPend = pendingRequests.erase(requestPend);
            continue; 
        }
        if (reqP->getRequestStatus() == RequestStatus::COLLECTING && canContinue(*reqP, "CourierVolunteer")){ 
            requestPend = pendingRequests.erase(requestPend);
            continue;
        }
        ++requestPend;
    }

    //handling volunteers
    for (Volunteer* vol : volunteers) {
        if (auto* invMgr = dynamic_cast<InventoryManagerVolunteer*>(vol)) {
            invMgr->step(); // Calls InventoryManagerVolunteer::step()
        }
        if (auto* courier = dynamic_cast<CourierVolunteer*>(vol)) {
            courier->step(); // Calls CourierVolunteer::step()
        }
    }

    auto requestPro = inProcessRequests.begin();
    while (requestPro != inProcessRequests.end()) {
        SupplyRequest* req = *requestPro;
        int inventoryManagerId = req->getInventoryManagerId();
        int courierId = req->getCourierId();    

        if (volunteerExists(inventoryManagerId)) {
            Volunteer* imv = &(getVolunteer(inventoryManagerId));
            if (imv && req->getRequestStatus() == RequestStatus::COLLECTING && !(imv->isBusy()) && static_cast<InventoryManagerVolunteer*>(imv)->getTimeLeft() == 0) {
                pendingRequests.push_back(req); 
                requestPro = inProcessRequests.erase(requestPro);   
                continue;
            }
        }    

        if (volunteerExists(courierId)) {
            Volunteer* courier = &(getVolunteer(courierId));
            if (courier && req->getRequestStatus() == RequestStatus::ON_THE_WAY && !(courier->isBusy()) && static_cast<CourierVolunteer*>(courier)->getDistanceLeft() == 0) {
                req->setStatus(RequestStatus::DONE);
                completedRequests.push_back(req);
                requestPro = inProcessRequests.erase(requestPro);
                continue;
            }
        }        
        ++requestPro;            
    }
}

bool MedicalWarehouse::canContinue(SupplyRequest& request, string type) {
    for (Volunteer* volunteer : volunteers) {
        if (type == "InventoryManagerVolunteer") {
            InventoryManagerVolunteer* inventoryManager = dynamic_cast<InventoryManagerVolunteer*>(volunteer);
            if (inventoryManager && inventoryManager->canTakeRequest(request)) {
                inventoryManager->acceptRequest(request);
                request.setStatus(RequestStatus::COLLECTING);
                request.setInventoryManagerId(inventoryManager->getId());
                inProcessRequests.push_back(&request);
                return true;
            }
        } else if (type == "CourierVolunteer") {
            CourierVolunteer* courier = dynamic_cast<CourierVolunteer*>(volunteer);
            if (courier && courier->canTakeRequest(request)) {
                courier->acceptRequest(request);                
                request.setStatus(RequestStatus::ON_THE_WAY);
                request.setCourierId(courier->getId());
                inProcessRequests.push_back(&request);
                return true;
            }
        }        
    }
    return false;
}

//copy vectors of objects
template<typename T>
vector<T*> MedicalWarehouse::copyVector(const vector<T*>& other) {
    vector<T*> newVector;
    for (const auto& obj : other) {
        newVector.push_back(obj->clone());
    }
    return newVector;
}


//destructor
MedicalWarehouse :: ~MedicalWarehouse(){
    for (CoreAction* action : actionsLog) {
            delete action;
        }
        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        for (SupplyRequest* request : pendingRequests) {
            delete request;
        }
        for (SupplyRequest* request : inProcessRequests) {
            delete request;
        }
        for (SupplyRequest* request : completedRequests) {
            delete request;
        }
        for (Beneficiary* beneficiary : beneficiaries) {
            delete beneficiary;
        }
}


//copy constructor
MedicalWarehouse::MedicalWarehouse(const MedicalWarehouse& other) :
    isOpen(other.isOpen), 
    actionsLog(copyVector<CoreAction>(other.actionsLog)),
    volunteers(copyVector<Volunteer>(other.volunteers)),
    pendingRequests(copyVector<SupplyRequest>(other.pendingRequests)),
    inProcessRequests(copyVector<SupplyRequest>(other.inProcessRequests)),
    completedRequests(copyVector<SupplyRequest>(other.completedRequests)),
    beneficiaries(copyVector<Beneficiary>(other.beneficiaries)),
    beneficiaryCounter(other.getBeneficiaryCounter()), 
    volunteerCounter(other.getVolunteerCounter()), 
    requestCounter(other.getRequestCounter()){}




//copy assignment operator
MedicalWarehouse& MedicalWarehouse::operator=(const MedicalWarehouse& other) {
    if (this != &other) {
        isOpen = other.isOpen;

        for (CoreAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
        for (CoreAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone());
        }

        for (Volunteer* volunteer : volunteers) {
            delete volunteer;
        }
        volunteers.clear();
        for (Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone());
        }

        for (SupplyRequest* request : pendingRequests) {
            delete request;
        }
        pendingRequests.clear();
        for (SupplyRequest* request : other.pendingRequests) {
            pendingRequests.push_back(new SupplyRequest(*request));
        }

        for (SupplyRequest* request : inProcessRequests) {
            delete request;
        }
        inProcessRequests.clear();
        for (SupplyRequest* request : other.inProcessRequests) {
            inProcessRequests.push_back(new SupplyRequest(*request));
        }

        for (SupplyRequest* request : completedRequests) {
            delete request;
        }
        completedRequests.clear();
        for (SupplyRequest* request : other.completedRequests) {
            completedRequests.push_back(new SupplyRequest(*request));
        }

        for (Beneficiary* beneficiary : beneficiaries) {
            delete beneficiary;
        }
        beneficiaries.clear();
        for (Beneficiary* beneficiary : other.beneficiaries) {
            beneficiaries.push_back(beneficiary->clone());
        }

        beneficiaryCounter = other.beneficiaryCounter;
        volunteerCounter = other.volunteerCounter;
        requestCounter = other.requestCounter;
    }
    return *this;
}


//move constructor
MedicalWarehouse::MedicalWarehouse(MedicalWarehouse&& other) noexcept
    : isOpen(other.isOpen), actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)), pendingRequests(std::move(other.pendingRequests)),
      inProcessRequests(std::move(other.inProcessRequests)), completedRequests(std::move(other.completedRequests)),
      beneficiaries(std::move(other.beneficiaries)), beneficiaryCounter(other.beneficiaryCounter),
      volunteerCounter(other.volunteerCounter), requestCounter(other.requestCounter) {}




//move assignment operator
MedicalWarehouse& MedicalWarehouse::operator=(MedicalWarehouse&& other) noexcept {
    if (this != &other) {
        for (auto action : actionsLog) {
        delete action;
        }
    
    for (auto volunteer : volunteers) {
        delete volunteer;
        }

    for (auto request : pendingRequests) {
        delete request;
        }

    for (auto request : inProcessRequests) {
        delete request;
        }

    for (auto request : completedRequests) {
        delete request;
        }

    for (auto beneficiary : beneficiaries) {
        delete beneficiary;
        }

        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingRequests = std::move(other.pendingRequests);
        inProcessRequests = std::move(other.inProcessRequests);
        completedRequests = std::move(other.completedRequests);
        beneficiaries = std::move(other.beneficiaries);
        isOpen = std::move(other.isOpen);
        beneficiaryCounter = std::move(other.beneficiaryCounter);
        volunteerCounter = std::move(other.volunteerCounter);
        requestCounter = std::move(other.requestCounter);
        isBackup = std::move(other.isBackup);
    }
    return *this;
}

// Opens the warehouse
void MedicalWarehouse::open() {
    if (!isOpen) 
    {
        isOpen = true;
        std::cout << "Medical services are now open!" << std::endl;
    } 
    else
    {
        std::cout << "The Warehouse is already open." << std::endl;
    }
}


// Closes the warehouse
void MedicalWarehouse::close() {
    std::cout << "Pre-Closing Requests:" << std::endl;
    for(SupplyRequest* request : pendingRequests){
        if(request){std::cout << request->closeInfo() << std::endl;}  //added          
    }
    for(SupplyRequest* request : inProcessRequests){
        if(request){std::cout << request->closeInfo() << std::endl;} //added
    }
    for(SupplyRequest* request : completedRequests){
        if(request){std::cout << request->closeInfo() << std::endl;} //added
    }    
    isOpen = false;    
}



