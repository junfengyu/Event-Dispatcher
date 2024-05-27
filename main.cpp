#include "event_dispatcher.h"
#include "custom_event_handlers.h"

int main() {
    // Create an EventDispatcher instance
    EventDispatcher dispatcher;

    // Start the dispatcher in a separate thread
    dispatcher.start();

    // Register event handlers
    dispatcher.registerEventHandler(EventType::EVENT_TYPE_A, std::make_shared<EventHandlerA>());
    dispatcher.registerEventHandler(EventType::EVENT_TYPE_B, std::make_shared<EventHandlerB>());
    dispatcher.registerEventHandler(EventType::EVENT_TYPE_C, std::make_shared<EventHandlerC>());

    // Main thread: generate and add events using addEvent
    for (int i = 0; i < 10; ++i) {
        switch (i % 3) {
            case 0:
                dispatcher.addEvent(EventType::EVENT_TYPE_A, EventDataA{i}); // int data
                break;
            case 1:
                dispatcher.addEvent(EventType::EVENT_TYPE_B, EventDataB{std::to_string(i)}); // string data
                break;
            case 2:
                dispatcher.addEvent(EventType::EVENT_TYPE_C, EventDataC{i * 0.1}); // double data
                break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Stop the dispatcher
    dispatcher.stop();

    return 0;
}
