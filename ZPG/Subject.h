#pragma once
#include <vector>
#include "Observer.h"

class Subject {
public:
    virtual ~Subject() {}
    virtual void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    virtual void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
protected:
    void notifyObservers(const Subject* subject) {
        for (auto* obs : observers) {
            obs->onSubjectChanged(subject);
        }
    }
    std::vector<Observer*> observers;
};
