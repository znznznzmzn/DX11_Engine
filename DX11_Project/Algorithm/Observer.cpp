#include "Framework.h"

Observer::Observer() {
}
Observer::~Observer() {
}

void Observer::AddEvent(string key, Event event)  { events[key].push_back(event); }
void Observer::AddObjectEvent(string key, ObjectEvent objectEvent) 
{ objectEvents[key].push_back(objectEvent); }

void Observer::ExcuteEvents(string key) {
    if (events.count(key) == 0) return;
    for (Event elem : events[key]) elem();
}
void Observer::ExcuteObjectEvents(string key, void* obj) {
    if (objectEvents.count(key) == 0) return;
    for (ObjectEvent elem : objectEvents[key]) elem(obj);
}
