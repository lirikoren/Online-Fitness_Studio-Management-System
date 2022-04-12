#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"
using namespace std;
typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void addOrder(pair<int,Workout>);
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    void setSalary(int newSalary);
    bool isOpen();
    void removeCustomerOrderHistory(int id);
    virtual ~Trainer();
    Trainer(const Trainer &other_trainer);
    Trainer &operator=(Trainer& other_trainer);
    Trainer(Trainer&& other_trainer);
    Trainer &operator=(Trainer &&other_trainer);


private:
    int update_capacity;
    int salary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    //methods
    void Copy(const Trainer &other_trainer);
    void Steal(Trainer &other_trainer);
};

#endif