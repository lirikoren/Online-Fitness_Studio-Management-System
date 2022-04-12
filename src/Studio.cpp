#include "Studio.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

Studio::Studio():  //default constructor
open(true)
{
    vector<Trainer*> _trainers;
    trainers=_trainers;
    vector<Workout> _workout_options;
    workout_options=_workout_options;
    vector<BaseAction*> _actionsLog;
    actionsLog=_actionsLog;
}
Studio::Studio(const string &configFilePath) {  //constructor
    ifstream myfile(configFilePath);
    string str;
    vector<string> fullString;
    if (myfile.is_open()) {
        while (getline(myfile, str)) {    //parses given text to vector of strings
            if (not str.empty() && str.substr(0, 1) != "#") {
                fullString.push_back(str);
            }
        }
        myfile.close();
        int trainersNumDigits = fullString[0].find(' '); //number of trainers
        istringstream line2(fullString[1]);
        string capacity;
        while (getline(line2, capacity, ',')) {  //parses capacities of trainers
            Trainer *trainer = new Trainer(stoi(capacity)); //Adds trainers according to their capacity
            trainers.push_back(trainer);
        }
            for(int i=2;i<fullString.size();i++){    //Parsing workout types
                    vector<string>lineVector;
                    string word;
                    istringstream ss(fullString[i]);
                    while(getline(ss,word,',')){
                        lineVector.push_back(word);       //creates vector of size 3 containing name, type and price of workout
                    }
                    WorkoutType type;
                    if(lineVector[1]==" Anaerobic")
                        type=WorkoutType::ANAEROBIC;
                    else if(lineVector[1]==" Cardio")
                        type=WorkoutType::CARDIO;
                    else
                        type=WorkoutType::MIXED;
                    workout_options.push_back(Workout(i-2,lineVector[0],stoi(lineVector[2]),type)); //(id,name,price,type)
    }
}
    else cout<<"error opening file";
}
Studio::Studio(const Studio &other){ //copy constructor
    open = other.open;
    for(Workout workout : other.workout_options){  //hard copies all objects
        workout_options.push_back(Workout(workout.getId(),workout.getName(),workout.getPrice(),workout.getType()));
    }
    trainers = copyTrainers(other.trainers);
    actionsLog = copyActionsLog(other.actionsLog);
}
Studio::~Studio() { //destructor
    deleteStudio();
}

Studio::Studio(Studio &&other){ //move constructor
    open=other.open;
    trainers=other.trainers;
    workout_options=other.workout_options;
    actionsLog=other.actionsLog;
    other.trainers.clear();
    other.workout_options.clear();
    other.actionsLog.clear();
}
Studio &Studio::operator=(const Studio &other) { //copy assignment operator
    if(this==&other){
        return *this;
    }
    deleteStudio();
    open=other.open;
    trainers= copyTrainers(other.trainers);
    actionsLog= copyActionsLog(other.actionsLog);
    for(Workout workout : other.workout_options){
        workout_options.push_back(Workout(workout.getId(),workout.getName(),workout.getPrice(),workout.getType()));
    }
    return *this;
}
Studio &Studio::operator=(Studio &&other) { //move assignment operator
    if(this==&other){
        return *this;
    }
    else{
        deleteStudio();
        open=other.open;
        workout_options=other.workout_options;
        trainers = other.trainers;
        actionsLog = other.actionsLog;
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();
    }
    return *this;
}


vector <Trainer*> Studio::copyTrainers(vector<Trainer*> _trainers){ //returns hard copy of given trainers
    vector<Trainer*> copy;
    for (Trainer *trainer: _trainers) {
        copy.push_back(new Trainer(*trainer));
    }
    return copy;
}
vector<BaseAction*> Studio::copyActionsLog(vector<BaseAction*> actions){ //returns hard copy of given actions
    vector<BaseAction*> copy;
    for(BaseAction *generalAction : actions){
        BaseAction *specificAction = generalAction->clone();
        copy.push_back(specificAction);
    }
    return copy;
}
void Studio::deleteStudio() {
    for (Trainer *trainer: trainers) { //hard deletes trainers
        if (trainer) {
            delete trainer;
            trainer = nullptr;
        }
    }
    for (BaseAction *action: actionsLog){ //hard deletes actionLog
        if (action){
            delete action;
            action = nullptr;
        }
    }
    trainers.clear();
    workout_options.clear();
    actionsLog.clear();
}

void Studio::start(){
    cout<<"Studio is now open!"<<endl;
    int idCounter=0;
    string input;
    getline(cin,input);
    while(input!="closeall"){
        vector<string> wordsVector;
        istringstream ss(input);
        string word;
        while(getline(ss,word,' ')){
                wordsVector.push_back(word);       //creates vector containing all words, seperated with spaces
        }
        string command=wordsVector[0];
        BaseAction*action;
        if(command=="open") {
            int id = stoi(wordsVector[1]);   //extracts id from string
            vector<Customer *> customersList;
            string temp;
            for (int i = 2; i < wordsVector.size(); i++) {  //creates Customer object for each pair of (name,type)
                string name= wordsVector[i].substr(0, wordsVector[i].find(","));
                string type=wordsVector[i].substr(wordsVector[i].find(',')+1,wordsVector[i].find(',')+4);
                Customer*customer;
                if(type=="swt"){
                    customer=new SweatyCustomer(name,idCounter);
                }
                else if(type=="chp"){
                    customer=new CheapCustomer(name,idCounter);
                }
                else if(type=="mcl"){
                    customer=new HeavyMuscleCustomer(name,idCounter);
                }
                else
                    customer=new FullBodyCustomer(name,idCounter);
                idCounter++;
                customersList.push_back(customer);
            }
            action=new OpenTrainer(id, customersList);
        }
        else if(command=="order"){
            int id = stoi(wordsVector[1]);   //extracts id from string
            action=new Order(id);
        }
        else if(command=="move"){
            action= new MoveCustomer(stoi(wordsVector[1]),stoi(wordsVector[2]),stoi(wordsVector[3]));
        }
        else if(command=="close"){
            int id=stoi(wordsVector[1]);
            action=new Close(id);
        }
        else if(command=="workout_options"){
            action=new PrintWorkoutOptions;
        }
        else if(command=="status"){
            int id=stoi(wordsVector[1]);
            action=new PrintTrainerStatus(id);
        }
        else if(command=="log"){
            action=new PrintActionsLog();
        }
        else if(command=="backup"){
            action=new BackupStudio;
        }
        else if(command=="restore"){
            action=new RestoreStudio;
        }
        action->act(*this);
        actionsLog.push_back(action);
        getline(cin,input);
    }
    BaseAction* closeAll=new CloseAll;
    closeAll->act(*this);
    actionsLog.push_back(closeAll);
}

int Studio::getNumOfTrainers() const{
    return trainers.size();
}

Trainer* Studio::getTrainer(int tid){
    if(tid>=trainers.size())
        return nullptr;
    else
        return trainers[tid];
}

const std::vector<BaseAction*>& Studio::getActionsLog() const // Return a reference to the history of actions
{
    return actionsLog;
}
vector<Workout>& Studio::getWorkoutOptions(){
    return workout_options;
}


