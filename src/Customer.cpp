#include <string>
#include <iostream>
#include "Customer.h"
#include <algorithm>
using namespace std;

Customer::Customer(std::string c_name, int c_id): //constructor
name(c_name),id(c_id){};

std::string Customer::getName() const{
    return name;
}
int Customer::getId() const{
    return id;
};

SweatyCustomer::SweatyCustomer(std::string name, int id):
        Customer(name,id){};
vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){
    vector<int> orderList;
    for(int i=0;i<workout_options.size();i++){
        if(workout_options[i].getType()==WorkoutType::CARDIO){  //creates vector of all cardio options
            orderList.push_back(workout_options[i].getId());
        }
    }
    return orderList;
};
std::string SweatyCustomer::toString() const{
    return getName()+",swt";
}

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name, id){};
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){
    const Workout* cheapest= &workout_options[0];
    for(int i=1;i<workout_options.size();i++){   //finds the cheapest workout
        if(workout_options[i].getPrice()==cheapest->getPrice()){  //if prices are equal, picks one with smalled id
            if(workout_options[i].getId()<cheapest->getId())
                cheapest=&workout_options[i];
        }
        if(workout_options[i].getPrice()<cheapest->getPrice())
            cheapest=&workout_options[i];
    }
    vector<int> orders;
    orders.push_back(cheapest->getId());
    return orders;
}
std::string CheapCustomer::toString() const {
    return getName()+",chp";
}


HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id){};

bool compare(pair<int,int> p1,pair<int,int> p2){
    return p1.second>p2.second;
}
std::vector<int> HeavyMuscleCustomer::order(const vector<Workout> &workout_options){
     vector<pair<int,int>> copy;
     for(int i=0;i<workout_options.size();i++){
         if(workout_options[i].getType()==ANAEROBIC)  //hard copies vector containing info of all anaerobic workouts
             copy.push_back(make_pair(workout_options[i].getId(),workout_options[i].getPrice()));
     }
     sort(copy.begin(),copy.end(), compare);  //sorts the anaerobics, from most expensive to cheapest
     vector<int>orders;
     for(int i=0;i<copy.size();i++)      //converts the paired vector to an id only vector
         orders.push_back(copy[i].first);
     return orders;
}
std::string HeavyMuscleCustomer::toString() const {
    return getName()+",mcl";
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name, id){};
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){
    vector<int>orders;
    int cheapestCardioId=-1,cheapestCardioPrice=-1;
    int mostExpensiveMixId=-1,mostExpensiveMixPrice=-1;
    int cheapestAnaerobicId=-1,cheapestAnaerobicPrice=-1;
    for(int i=0;i<workout_options.size();i++) {
        if(workout_options[i].getType()==CARDIO) {
            if (cheapestCardioId == -1 || workout_options[i].getPrice() < cheapestCardioPrice //if found cheaper cardio workout
            || (workout_options[i].getPrice() == cheapestCardioPrice && //if prices are equal, picks one with smaller id
                workout_options[i].getId() < cheapestCardioId)) {
                cheapestCardioId = workout_options[i].getId();
                cheapestCardioPrice = workout_options[i].getPrice();
            }
        }
        else if(workout_options[i].getType()==MIXED){
            if(mostExpensiveMixId==-1 || workout_options[i].getPrice()>mostExpensiveMixPrice //if found more expensive mixed workout
            ||(workout_options[i].getPrice()==mostExpensiveMixPrice && //if prices are equal, picks one with smaller id
            workout_options[i].getId()<mostExpensiveMixId)){
                mostExpensiveMixId=workout_options[i].getId();
                mostExpensiveMixPrice=workout_options[i].getPrice();
            }
        }
        else{
                if(cheapestAnaerobicId==-1 || workout_options[i].getPrice()<cheapestAnaerobicPrice //if found cheaper anaerobic workout
                || (workout_options[i].getPrice()<cheapestAnaerobicPrice && //if prices are equal, picks one with smaller id
                workout_options[i].getId()<cheapestCardioId)) {
                    cheapestAnaerobicId = workout_options[i].getId();
                    cheapestAnaerobicPrice = workout_options[i].getPrice();
                }
        }
    }
    orders.push_back(cheapestCardioId);
    orders.push_back(mostExpensiveMixId);
    orders.push_back(cheapestAnaerobicId);
    return orders;
}
std::string FullBodyCustomer::toString() const {
    return getName()+",fbd";
}



