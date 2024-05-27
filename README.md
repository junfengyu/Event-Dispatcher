### README.md

```markdown
# EventDispatcher

EventDispatcher is a C++ event dispatch system that uses a custom base class with inheritance to manage different types of event data. This project demonstrates how to handle events asynchronously using separate worker threads for each event type. It also runs the dispatcher in a separate thread from the main thread.

## Features

- Supports multiple event types
- Handles events with different data structures using inheritance
- Uses worker threads to process events asynchronously
- Thread-safe event addition
- Flexible event handling system
- Dispatcher runs in a separate thread

## Requirements

- C++17 compatible compiler
- CMake 3.10 or higher

## Getting Started

### Clone the Repository

```bash
git clone https://github.com/yourusername/EventDispatcher.git
cd EventDispatcher
```

### Build the Project

1. **Create a build directory**:

    ```bash
    mkdir build
    cd build
    ```

2. **Generate the build files**:

    ```bash
    cmake ..
    ```

3. **Build the project**:

    ```bash
    make
    ```

### Run the Example

```bash
./event_dispatcher
```

## Project Structure

```
EventDispatcher/
├── CMakeLists.txt
├── event_dispatcher.h
├── event_dispatcher.cpp
├── custom_event_handlers.h
├── custom_event_handlers.cpp
└── main.cpp
```

- `CMakeLists.txt`: CMake configuration file.
- `event_dispatcher.h`: Header file containing the class definitions.
- `event_dispatcher.cpp`: Source file containing the class implementations.
- `custom_event_handlers.h`: Header file for custom event handlers.
- `custom_event_handlers.cpp`: Source file for custom event handlers.
- `main.cpp`: Example usage of the `EventDispatcher` class.

## Usage

The `EventDispatcher` class allows you to register event handlers, enqueue events, and handle events asynchronously. Here is a brief overview of how to use the `EventDispatcher` class:

### Register Event Handlers

```cpp
dispatcher.registerEventHandler(EventType::EVENT_TYPE_A, std::make_shared<EventHandlerA>());
dispatcher.registerEventHandler(EventType::EVENT_TYPE_B, std::make_shared<EventHandlerB>());
dispatcher.registerEventHandler(EventType::EVENT_TYPE_C, std::make_shared<EventHandlerC>());
```

### Add Events

You can add events from any thread or function using the `addEvent` method:

```cpp
dispatcher.addEvent(EventType::EVENT_TYPE_A, EventDataA{42});
dispatcher.addEvent(EventType::EVENT_TYPE_B, EventDataB{"Hello, World!"});
dispatcher.addEvent(EventType::EVENT_TYPE_C, EventDataC{3.14});
```

### Event Handlers

Define event handlers to process the events:

```cpp
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
```

### Main Function

The main function demonstrates how to use the `EventDispatcher` class:

```cpp
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
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

This `README.md` provides an overview of the project, instructions for building and running the project, and a brief guide on how to use the `EventDispatcher` class. It also includes standard sections for license and contributing, making it suitable for a GitHub repository.
