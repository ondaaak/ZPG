#pragma once
class Observer {
public:
    virtual ~Observer() {}
    virtual void onCameraChanged(const class Camera* camera) = 0;
};
