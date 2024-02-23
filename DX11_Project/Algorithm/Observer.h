#pragma once

class Observer : public Singleton<Observer> {
public:
    Observer();
    ~Observer();

    void AddEvent(string key, Event event);
    void AddObjectEvent(string key, ObjectEvent objectEvent);

    void ExcuteEvents(string key);
    void ExcuteObjectEvents(string key, void* obj);

private:
    map<string, vector<Event>> events;
    map<string, vector<ObjectEvent>> objectEvents;
};