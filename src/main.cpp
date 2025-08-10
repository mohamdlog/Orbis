#include "monitor.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <memory>
#include <future>

#include <mavsdk/mavsdk.h>
#include <mavsdk/system.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;

int main() {
    std::signal(SIGINT, signal_handler);

    Mavsdk sdk{Mavsdk::Configuration{ComponentType::CompanionComputer}};
    const std::string connection_url = "serial:///dev/ttyACM0:57600";

    ConnectionResult connection_result = sdk.add_any_connection(connection_url);
    if (connection_result != ConnectionResult::Success) {
        std::cerr << "Connection failed: " << connection_result << std::endl;
        return -1;
    }

    std::cout << "Waiting for a drone to connect..." << std::endl;
    auto prom{std::make_shared<std::promise<std::shared_ptr<System>>>()};
    auto fut{prom->get_future()};

    sdk.subscribe_on_new_system([&sdk, prom]() {
        std::shared_ptr<System> system = sdk.systems().at(0);
        std::cout << "Drone discovered!" << std::endl;
        prom->set_value(system);
    });

    std::shared_ptr<System> system{fut.get()};

    std::cout 
        << std::string(50, '_') << "\n" 
        << std::string(20, ' ') << "Main Menu\n" 
        << std::string(50, '_')
        << "\nWelcome to the main menu. Enter your selection:\n\n"
        << " 1. Read instructions\n"
        << " 2. Start telemetry monitoring\n"
        << " 3. Run control tests\n"
        << std::string(50, '_') << "\n\n";
    
    short choice;
    std::cin >> choice;
    std::cin.ignore(1);

    switch (choice) {
        case 1: {
            std::cout << "Not yet implemented.\n";
            break;
        }
        case 2: {
            Telemetry telemetry(system);
            setup_monitoring(telemetry);
            break;
        }
        case 3: {
            std::cout << "Not yet implemented.\n";
            break;
        }
    }

    while (!g_stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\nReceived stop signal. Exiting." << std::endl;
    return 0;
}