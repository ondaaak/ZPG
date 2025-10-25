#pragma once
class Subject;

class Observer {
public:
    virtual ~Observer() {}
    virtual void onSubjectChanged(const Subject* subject) = 0;
};
