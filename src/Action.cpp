//
// Created by Boaz on 11/22/2021.
//
#include "Action.h"
#include "Trainer.h"
#include "Studio.h"
using namespace std;
extern Studio *backup;


BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete(){
    status= COMPLETED;
}

void BaseAction::error(std::string errorMsg){
    status= ERROR;
    this->errorMsg = "Error: <"+errorMsg+">";
    std::cout<<errorMsg<<endl;
}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}
BaseAction::~BaseAction()= default;

// /////////////////////////////////////// OpenTrainer /////////////////////////////////////////////////
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):
        trainerId(id),customers(customersList){
    for (Customer* customer:customers) {
        customerString.push_back(customer->toString());
    }
}

void OpenTrainer::act(Studio &studio){
    Trainer *my_trainer = studio.getTrainer(trainerId);
    if(my_trainer==0 || my_trainer->isOpen() == true){
        error("Workout session does not exist or is already open.");
    }
    else {
        int my_capacity = my_trainer->getCapacity();
        for (int i = 0; i < customers.size(); i++) {
            if (my_capacity > 0) {
                my_trainer->addCustomer(customers[i]);
                my_capacity = my_capacity - 1;
            } else {
                delete customers[i];  //deletes customers that can't enter trainer's session
            }
        }
    }
    my_trainer->openTrainer();
    customers.clear();
    complete();
}

string OpenTrainer::toString() const {
    string msg="open "+ to_string(trainerId);
    for (string customer: customerString) {
        msg += " " + customer;
    }
    if (getStatus() == COMPLETED)
        return msg+" Completed";
    else
        return msg+" "+getErrorMsg();
}

// //////////////////////////////////////////// Order /////////////////////////////////////////////////

Order::Order(int id): trainerId(id){}
void Order::act(Studio &studio) {
    Trainer* trainer=studio.getTrainer(trainerId);
    if(trainer== nullptr || !trainer->isOpen()) {
        error("Trainer does not exist or is not open");
    }
    else{
        for(int i=0;i<trainer->getCustomers().size();i++){    //for each customer of trainer, orders his order
            vector<int> customerWorkoutIds=trainer->getCustomers()[i]->order(studio.getWorkoutOptions());
            const int customerId=trainer->getCustomers()[i]->getId();
            trainer->order(customerId,customerWorkoutIds,studio.getWorkoutOptions());
        }
        //now prints the workouts happening
        for(int i=0;i<trainer->getOrders().size();i++){
            int customerId=trainer->getOrders()[i].first;
            string customerName=trainer->getCustomer(customerId)->getName();
            string workoutName=trainer->getOrders()[i].second.getName();
            cout<< customerName + " Is Doing "+workoutName<<endl;
        }
        complete();
    }
}
std::string Order::toString() const {
    string message="order "+ to_string(trainerId)+" ";
    if(getStatus()==COMPLETED)
        message+="Completed";
    else
        message+=getErrorMsg();
    return message;
}
// /////////////////////////////////////// MoveCustomer /////////////////////////////////////////////////
MoveCustomer::MoveCustomer(int src, int dst, int customerId):
    srcTrainer(src),dstTrainer(dst),id(customerId){}

void MoveCustomer::act(Studio &studio) {
    Trainer *src = studio.getTrainer(srcTrainer);
    Trainer *dst = studio.getTrainer(dstTrainer);
    if (src == nullptr || dst == nullptr || !src->isOpen() || !dst->isOpen()||  //if trainers or customer are invalid
            src->getCustomer(id)== nullptr || dst->getCapacity()==0) {
        error("Cannot move customer");
    }else{
        vector <pair<int, Workout>> srcOrders=src->getOrders();
        Customer *customerPtr=src->getCustomer(id);
        dst->addCustomer(customerPtr);
        for(int i=0;i<srcOrders.size();i++){ //adds past orders to dst orders
            if(srcOrders[i].first==id) {
                dst->addOrder(srcOrders[i]);
            }
        }
        src->removeCustomerOrderHistory(id); //deletes past orders of customer in src
        src->removeCustomer(id);
        if(src->getCustomers().size()==0){ //checks if src needs to be closed
            src->closeTrainer();
        }
        complete();
    }
}
string MoveCustomer::toString() const{
   string message="move "+ to_string(srcTrainer)+" "+ to_string(dstTrainer)+" ";
   if(getStatus()==COMPLETED)
       message+="Completed";
   else
       message+=getErrorMsg();
   return message;
}

// //////////////////////////////////////////// Close /////////////////////////////////////////////////

Close::Close(int id):trainerId(id){}

void Close:: act(Studio &studio){
    Trainer *my_trainer = studio.getTrainer(trainerId);
    if(my_trainer==0||!my_trainer->isOpen()){
        error("Trainer does not exist or is not open.");
    }else{
        int salary =my_trainer->getSalary();
        my_trainer->closeTrainer();
        string output = "Trainer "+ to_string(trainerId) + " is closed. Salary "+ to_string(salary)+"NIS";
        cout<< output<<endl;
        complete();
    }
}
string Close:: toString() const{
    string msg="close "+ to_string(trainerId)+" ";
    if(getStatus()==COMPLETED)
        msg+="Completed";
    else
        msg+=getErrorMsg();
    return msg;
}
// ////////////////////////////////////////// CloseAll /////////////////////////////////////////////////
CloseAll::CloseAll(){}

void CloseAll::act(Studio &studio) {
    for (int trainer_id = 0; trainer_id < studio.getNumOfTrainers(); trainer_id++) {
        if (studio.getTrainer(trainer_id)->isOpen()) {
            Close action(trainer_id);
            action.act(studio);
        }
        complete();
    }
}

string CloseAll::toString() const{
    string msg="closeall ";
    if(getStatus()==COMPLETED)
        msg+="Completed";
    else
        msg+=getErrorMsg();
    return msg;
}
// //////////////////////////////////////PrintWorkoutOptions /////////////////////////////////////////////////
PrintWorkoutOptions::PrintWorkoutOptions(){}

void PrintWorkoutOptions::act(Studio &studio){
    vector<Workout> workouts = studio.getWorkoutOptions();
    for(int i = 0 ; i< workouts.size(); i++){
        Workout my_workout = workouts[i];
        string name =my_workout.getName();
        int type = my_workout.getType();
        string type_output;
        if(type == 0 ) {
            type_output = "Anaerobic";
        }
        else if(type == 1 ) {
            type_output = "Mixed";
        }
        else {
            type_output = "Cardio";
        }
        string price = to_string(my_workout.getPrice());
        cout<<name<< ", " <<type_output<< ", " << price<<endl;
        complete();
    }
}
string PrintWorkoutOptions::toString() const{
    string msg="workout_options ";
    if(getStatus()==COMPLETED)
        msg+="Completed";
    else
        msg+=getErrorMsg();
    return msg;
}
// //////////////////////////////////////PrintTrainerStatus/////////////////////////////////////////////

PrintTrainerStatus::PrintTrainerStatus(int id): trainerId(id){}

void PrintTrainerStatus::act(Studio &studio){
    Trainer *my_trainer = studio.getTrainer(trainerId);
    if(!my_trainer->isOpen()){
        cout<<"Trainer "<<to_string(trainerId)<<" Status: closed"<<endl;
    }else{
        cout<<"Trainer "<<to_string(trainerId)<<" Status: open"<<endl;
        cout<<"Customers:"<<endl;
        vector<Customer*> customers = (my_trainer->getCustomers());
        for(int i = 0 ;i < customers.size() ; i++){
            if(customers[i] != nullptr) {
                Customer *c = my_trainer->getCustomer(customers[i]->getId());
                cout << c->getId() << " " << c->getName() << endl;
            }
        }
        cout<<"Orders:"<<endl;
        vector<OrderPair> orders = my_trainer->getOrders();
        for(int i = 0 ;i < orders.size() ; i++){
            cout<<orders[i].second.getName()+" "+ to_string(orders[i].second.getPrice())+"NIS "+ to_string(orders[i].first)<<endl;
        }
        cout<<"Current Trainer's Salary: "+ to_string(my_trainer->getSalary())+"NIS"<<endl;
    }
    complete();
}
string PrintTrainerStatus::toString() const {
    string msg="status "+ to_string(trainerId);
    if(getStatus()==COMPLETED)
        msg+=" Completed";
    else
        msg+=getErrorMsg();
    return msg;
}

// //////////////////////////////////////PrintActionsLog/////////////////////////////////////////////
PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog::act(Studio &studio){
    for(int i=0;i<studio.getActionsLog().size();i++)
        cout<<studio.getActionsLog()[i]->toString()<<endl;
    complete();
}
string PrintActionsLog::toString() const {
    string msg="log ";
    if(getStatus()==COMPLETED)
        msg+="Completed";
    else
        msg+=getErrorMsg();
    return msg;
}
// //////////////////////////////////////BackupStudio/////////////////////////////////////////////
BackupStudio::BackupStudio() {}
    void BackupStudio::act(Studio &studio) {
        if (backup != nullptr)
            delete (backup);
        backup = new Studio(studio); //hard copies current studio to backup
        complete();
    }
string BackupStudio::toString() const{
    string message="backup ";
    if(getStatus()==COMPLETED)
        message+="Completed";
    else
        message+=getErrorMsg();
    return message;
}
// /////////////////////////////////////RestoreStudio/////////////////////////////////////////////
RestoreStudio::RestoreStudio() {}
void RestoreStudio::act(Studio &studio) {
    if(backup== nullptr)
        error("No backup available");
    else{
        studio=*backup;
        complete();
    }
}
string RestoreStudio::toString() const{
    string message="restore ";
    if(getStatus()==COMPLETED)
        message+="Completed";
    else
        message+=getErrorMsg();
    return message;
}
