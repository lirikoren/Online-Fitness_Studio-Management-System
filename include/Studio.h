#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{		
public:
	Studio();
    ~Studio(); //destructor
    Studio(const Studio &other); //copy constructor
    Studio(Studio &&other); //move constructor
    Studio &operator=(const Studio &studio); //copy assignment operator
    Studio &operator=(Studio &&studio); //move assignment operator
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid); // להחזיר 0 אם לא קיים
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    vector <Trainer*> copyTrainers(vector<Trainer*>);
    vector<BaseAction*> copyActionsLog(vector<BaseAction*>);
    void deleteStudio();
};

#endif