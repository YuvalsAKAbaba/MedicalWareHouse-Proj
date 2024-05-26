#pragma once
#include <string>
#include <vector>
#include "Action.h"
#include "Beneficiary.h"
#include "Volunteer.h"
#include "SupplyRequest.h"

class CoreAction;
class Beneficiary;
class Volunteer;
class SupplyRequest;
// Warehouse responsible for Volunteers, Beneficiaries Actions, and SupplyRequests.


class MedicalWarehouse {

    public:
        MedicalWarehouse(const string &configFilePath);
        void start();
        void addRequest(SupplyRequest* request);
        void addAction(CoreAction* action);
        void addBeneficiary(Beneficiary* beneficiary); 
        void addVolunteer(Volunteer* volunteer); 
        Beneficiary &getBeneficiary(int beneficiaryId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        SupplyRequest &getRequest(int requestId) const;
        bool requestExists(int requestId) const;
        bool volunteerExists(int volunteerId) const;
        bool beneficiaryExists(int beneficiaryId) const;
        const vector<CoreAction*> &getActions() const;
        const vector<string> &getActionsString() const; //added
        const vector<SupplyRequest*> &getPending() const;
        const vector<SupplyRequest*> &getProgress() const;
        const vector<SupplyRequest*> &getCompleted() const;
        const int getBeneficiaryCounter() const;
        const int getVolunteerCounter() const;
        const int getRequestCounter() const;
        bool getIsBackup() const; 
        void setIsBackup(); 
        bool isNumber(string& num);  
        void step(); //new
        bool canContinue(SupplyRequest& request, string type); 
        void open(); 
        void close();

        template<typename T>
        vector<T*> copyVector(const vector<T*>& source);
        ~MedicalWarehouse();
        MedicalWarehouse(const MedicalWarehouse& other);
        MedicalWarehouse& operator=(const MedicalWarehouse& other);
        MedicalWarehouse(MedicalWarehouse&& other) noexcept;
        MedicalWarehouse& operator=(MedicalWarehouse&& other) noexcept;
       

    private:
        bool isOpen;
        vector<CoreAction*> actionsLog;
        vector<string> actionsStringLog;//added
        vector<Volunteer*> volunteers;
        vector<SupplyRequest*> pendingRequests;
        vector<SupplyRequest*> inProcessRequests;
        vector<SupplyRequest*> completedRequests;
        vector<Beneficiary*> beneficiaries;
        int volunteerCounter; //For assigning unique volunteer IDs
        int requestCounter;
        int beneficiaryCounter; //For assigning unique Beneficiary IDs, first add 1 to counter then assign to new beneficiary
        bool isBackup; // new
};
