#include "event_dispatcher.h"
#include <sstream>

// EventDataA implementation
EventDataA::EventDataA(int d) : data(d) {}

void EventDataA::extractData() const {
    std::cout << "EventDataA: " << data << std::endl;
}

// EventDataB implementation
EventDataB::EventDataB(const std::string& d) : data(d) {}

void EventDataB::extractData() const {
    std::cout << "EventDataB: " << data << std::endl;
}

// EventDataC implementation
EventDataC::EventDataC(double d) : data(d) {}

void EventDataC::extractData() const {
    std::cout << "EventDataC: " << data << std::endl;
}

// EventDispatcher implementation
EventDispatcher::EventDispatcher() : stopFlag(false) {}

EventDispatcher::~EventDispatcher() {
    stop();
}

void EventDispatcher::start() {
    dispatcherThreadHandle = std::thread(&EventDispatcher::dispatcherThread, this);
}

void EventDispatcher::stop() {
    {
        std::lock_guard<std::mutex> lock(handlersMutex);
        stopFlag = true;
    }
    for (auto& [type, cv] : queueCVs) {
        cv.notify_all();
    }
    if (dispatcherThreadHandle.joinable()) {
        dispatcherThreadHandle.join();
    }
    for (auto& [type, thread] : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void EventDispatcher::dispatcherThread() {
    std::stringstream ss;
    ss << "Dispatcher thread started. Thread ID: " << std::this_thread::get_id() << "\n";
    std::cout << ss.str();
    // Initialize worker threads for each event type
    for (auto type : {EventType::EVENT_TYPE_A, EventType::EVENT_TYPE_B, EventType::EVENT_TYPE_C}) {
        workerThreads[type] = std::thread(&EventDispatcher::eventWorker, this, type);
    }
}

void EventDispatcher::registerEventHandler(EventType type, std::shared_ptr<EventHandler> handler) {
    std::lock_guard<std::mutex> lock(handlersMutex);
    eventHandlers[type].emplace_back(handler);
}

void EventDispatcher::enqueueEvent(const Event& event) {
    {
        std::lock_guard<std::mutex> lock(queueMutexes[event.type]);
        eventQueues[event.type].push(event);
    }
    queueCVs[event.type].notify_one();
}

void EventDispatcher::eventWorker(EventType type) {
    std::stringstream ss;
    ss << "Thread for event type " << static_cast<int>(type) << " started. Thread ID: " << std::this_thread::get_id() << "\n";
    std::cout << ss.str();

    while (true) {
        Event event;
        {
            std::unique_lock<std::mutex> lock(queueMutexes[type]);
            queueCVs[type].wait(lock, [&event, type, this] {
                return !eventQueues[type].empty() || stopFlag;
            });

            if (stopFlag && eventQueues[type].empty()) {
                return;
            }

            event = eventQueues[type].front();
            eventQueues[type].pop();
        }

        ss.str("");
        ss << "Thread ID: " << std::this_thread::get_id() << " processing event of type " << static_cast<int>(type) << "\n";
        std::cout << ss.str();

        dispatchEvent(event);
    }
}

void EventDispatcher::dispatchEvent(const Event& event) {
    std::lock_guard<std::mutex> lock(handlersMutex);
    if (eventHandlers.find(event.type) != eventHandlers.end()) {
        for (auto& handler : eventHandlers[event.type]) {
            handler->handleEvent(event.data);
        }
    }
}
