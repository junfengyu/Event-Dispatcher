#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H

#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <unordered_map>
#include <vector>
#include <memory>

// Event types
enum class EventType {
    EVENT_TYPE_A,
    EVENT_TYPE_B,
    EVENT_TYPE_C
    // Add new event types as needed
};

// Base class for event data
struct BaseEventData {
    virtual ~BaseEventData() = default;
    virtual void extractData() const = 0; // Pure virtual method to extract data
};

// Abstract base class for event handler
class EventHandler {
public:
    virtual ~EventHandler() = default;
    virtual void handleEvent(const std::shared_ptr<BaseEventData>& data) = 0;
};

// Derived classes for different types of event data
struct EventDataA : BaseEventData {
    int data;
    EventDataA(int d);
    void extractData() const override;
};

struct EventDataB : BaseEventData {
    std::string data;
    EventDataB(const std::string& d);
    void extractData() const override;
};

struct EventDataC : BaseEventData {
    double data;
    EventDataC(double d);
    void extractData() const override;
};

// Event structure using std::shared_ptr<BaseEventData> to hold any type of event data
struct Event {
    EventType type;
    std::shared_ptr<BaseEventData> data;
};

// EventDispatcher class
class EventDispatcher {
public:
    EventDispatcher();
    ~EventDispatcher();

    void start();
    void stop();
    void registerEventHandler(EventType type, std::shared_ptr<EventHandler> handler);
    void enqueueEvent(const Event& event);

    template<typename T>
    void addEvent(EventType type, T&& data) {
        auto event = Event{type, std::make_shared<std::decay_t<T>>(std::forward<T>(data))};
        enqueueEvent(event);
    }

private:
    void dispatcherThread();
    void eventWorker(EventType type);
    void dispatchEvent(const Event& event);

    std::unordered_map<EventType, std::queue<Event>> eventQueues;
    std::unordered_map<EventType, std::vector<std::shared_ptr<EventHandler>>> eventHandlers;
    std::unordered_map<EventType, std::mutex> queueMutexes;
    std::unordered_map<EventType, std::condition_variable> queueCVs;
    std::unordered_map<EventType, std::thread> workerThreads;

    std::mutex handlersMutex;
    std::thread dispatcherThreadHandle;
    bool stopFlag;
};

#endif // EVENT_DISPATCHER_H
