#pragma once
#include "MedicalWarehouse.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class ActionStatus{
    COMPLETED, ERROR
};

enum class beneficiaryType{
    Hospital, Clinic
};

beneficiaryType stringToBeneficiaryType(const std::string& type);

class MedicalWarehouse;

class CoreAction{
    public:
        CoreAction();
        virtual ~CoreAction(); // Destructor
        CoreAction(const CoreAction& other); // Copy constructor
        CoreAction& operator=(const CoreAction& other); // Copy assignment operator 
        CoreAction(CoreAction&& other) noexcept; // Move constructor
        CoreAction& operator=(CoreAction&& other) noexcept; // Move assignment operator
        ActionStatus getStatus() const;
        virtual void act(MedicalWarehouse& medWareHouse)=0;
        virtual string toString() const=0;
        virtual CoreAction* clone() const=0;
        string actionStatusToString() const;

    protected:
        void complete();
        void error(string errorMsg);
        string getErrorMsg() const;

    private:
        string errorMsg;
        ActionStatus status;
};

class SimulateStep : public CoreAction {

    public:
        SimulateStep(int numOfSteps);
        void act(MedicalWarehouse  &medWareHouse) override;
        std::string toString() const override;
        SimulateStep *clone() const override;

    private:
        const int numOfSteps;
};

class AddRequest : public CoreAction {
    public:
        AddRequest(int id);
        void act(MedicalWarehouse  &medWareHouse) override;
        string toString() const override;
        AddRequest *clone() const override;
    private:
        const int beneficiaryId;
};


class RegisterBeneficiary : public CoreAction {
    public:
        RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests);
        void act(MedicalWarehouse &medWareHouse) override;
        RegisterBeneficiary *clone() const override;
        string toString() const override;
    private:
        const string beneficiaryName;
        const enum beneficiaryType beneficiaryType;
        const int distance;
        const int maxRequests;
};



class PrintRequestStatus : public CoreAction {
    public:
        PrintRequestStatus(int id);
        void act(MedicalWarehouse &medWareHouse) override;
        PrintRequestStatus *clone() const override;
        string toString() const override;
    private:
        const int requestId;
};

class PrintBeneficiaryStatus: public CoreAction {
    public:
        PrintBeneficiaryStatus(int BeneficiaryId);
        void act(MedicalWarehouse &medWareHouse) override;
        PrintBeneficiaryStatus *clone() const override;
        string toString() const override;
    private:
        const int beneficiaryId;
};


class PrintVolunteerStatus : public CoreAction {
    public:
        PrintVolunteerStatus(int id);
        void act(MedicalWarehouse &medWareHouse) override;
        PrintVolunteerStatus *clone() const override;
        string toString() const override;
    private:
        const int volunteerId;
};


class PrintActionsLog : public CoreAction {
    public:
        PrintActionsLog();
        void act(MedicalWarehouse &medWareHouse) override;
        PrintActionsLog *clone() const override;
        string toString() const override;
    private:
};

class Close : public CoreAction {
    public:
        Close();
        void act(MedicalWarehouse &medWareHouse) override;
        Close *clone() const override;
        string toString() const override;
    private:
};

class BackupWareHouse : public CoreAction {
    public:
        BackupWareHouse();
        void act(MedicalWarehouse &medWareHouse) override;
        BackupWareHouse *clone() const override;
        string toString() const override;
    private:
};


class RestoreWareHouse : public CoreAction {
    public:
        RestoreWareHouse();
        void act(MedicalWarehouse &medWareHouse) override;
        RestoreWareHouse *clone() const override;
        string toString() const override;
    private:
};

