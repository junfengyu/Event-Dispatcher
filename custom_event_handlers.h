#include "event_dispatcher.h"

class EventHandlerA : public EventHandler {
public:
    void handleEvent(const std::shared_ptr<BaseEventData>& data) override {
        auto eventData = std::dynamic_pointer_cast<EventDataA>(data);
        if (eventData) {
            eventData->extractData();
        }
    }
};

class EventHandlerB : public EventHandler {
public:
    void handleEvent(const std::shared_ptr<BaseEventData>& data) override {
        auto eventData = std::dynamic_pointer_cast<EventDataB>(data);
        if (eventData) {
            eventData->extractData();
        }
    }
};

class EventHandlerC : public EventHandler {
public:
    void handleEvent(const std::shared_ptr<BaseEventData>& data) override {
        auto eventData = std::dynamic_pointer_cast<EventDataC>(data);
        if (eventData) {
            eventData->extractData();
        }
    }
};

