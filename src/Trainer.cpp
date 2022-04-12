#include "Trainer.h"
#include <utility>
#include <string>
#include <iostream>
using namespace std;
//update_capacity = number of free places
//constractor,initelize update_capacity
Trainer::Trainer(int t_capacity):
        capacity(t_capacity),salary(0){
    update_capacity=capacity;
    open=false;
}
int Trainer::getCapacity() const{
    return update_capacity;
}

//adding customer if the trainer capacity is not full
void Trainer::addCustomer(Customer* customer){
    if(update_capacity > 0) {
        customersList.push_back(customer);
        update_capacity = update_capacity-1;
    }
}

//remove the customer and update update_capacity
void Trainer::removeCustomer(int id) {
    for(int i=0;i<customersList.size();i++){
        if(customersList[i]->getId()==id) {
            customersList.erase(customersList.begin()+(i));
            update_capacity=update_capacity+1;
        }
    }
}

Customer* Trainer::getCustomer(int id){

    for(int i=0;i<customersList.size();i++) {
        if (customersList[i]->getId()== id) {
            return customersList[i];
        }
    }
}

std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}

std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
void Trainer::addOrder(pair<int, Workout> order) {
    orderList.push_back(order);
    salary+=order.second.getPrice();
}
void Trainer::removeCustomerOrderHistory(int id) {
    for(int i=orderList.size()-1;i>=0;i--){
        if(orderList[i].first==id){
            salary-=orderList[i].second.getPrice();
            orderList.erase(orderList.begin()+i);
        }
    }
}
//add to the vector orderlist the order pairs
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    for(int i=0; i < workout_ids.size(); i++){
        orderList.push_back(OrderPair(customer_id,workout_options[workout_ids[i]]));
        salary = salary + (workout_options[workout_ids[i]].getPrice());
    }
}

void Trainer::openTrainer(){
    open= true;
}

//reset capacity to the original capacity and clean the vectors
void Trainer::closeTrainer(){
    open = false;
    update_capacity = capacity;
    for(Customer* customer:customersList){
    delete(customer);}
    customersList.clear();
    orderList.clear();
}

int Trainer::getSalary(){
    return salary;
}
void Trainer::setSalary(int newSalary) {
    salary=newSalary;
}
bool Trainer::isOpen(){
    return open;
}

//role of 5:
//destructor
Trainer::~Trainer() {
    for (Customer * c:customersList) {
        if (c) delete c;
    }
    customersList.clear();
    orderList.clear();
}

//copy constructor
Trainer::Trainer(const Trainer& other_trainer):capacity(other_trainer.capacity) , update_capacity(other_trainer.update_capacity) , open(other_trainer.open) , salary(other_trainer.salary){
    for (Customer *c : other_trainer.customersList){
        customersList.push_back(c->clone());
    }
    for (OrderPair pair : other_trainer.orderList) {
        orderList.push_back(make_pair(pair.first , pair.second));
    }
}

//copy assigment operator
Trainer& Trainer::operator=(Trainer& other_trainer){
    if(this != &other_trainer){
        for (int i = 0 ; i <customersList.size() ; i++) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
        customersList.clear();
        orderList.clear();
        update_capacity = other_trainer.update_capacity;
        open = other_trainer.open;
        salary = other_trainer.salary;
        for (Customer *c : other_trainer.customersList){
            customersList.push_back(c->clone());
        }
        for (OrderPair pair : other_trainer.orderList) {
            orderList.push_back(make_pair(pair.first , pair.second));
        }
    }
    return (*this);
}

//move constructor
Trainer::Trainer(Trainer&& other_trainer):capacity(other_trainer.capacity),update_capacity(other_trainer.update_capacity),open(other_trainer.open),salary(other_trainer.salary),customersList(other_trainer.customersList),orderList(other_trainer.orderList) {
    other_trainer.customersList.clear();
    other_trainer.orderList.clear();
}

//move assigment operator
Trainer& Trainer::operator=(Trainer &&other_trainer) {
    if(this != &other_trainer) {
        for (int i = 0 ; i <customersList.size() ; i++) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
        customersList.clear();
        orderList.clear();
        capacity = other_trainer.capacity;
        update_capacity = other_trainer.update_capacity;
        open = other_trainer.open;
        salary = other_trainer.salary;
        for (Customer *c: other_trainer.customersList) {
            customersList.push_back(c);
        }
        for (OrderPair pair: other_trainer.orderList) {
            orderList.push_back(pair);
        }
        other_trainer.customersList.clear();
        other_trainer.orderList.clear();
    }
    return *this;
}
