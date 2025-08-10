#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <iomanip>
#include <future>
#include <csignal>
#include <atomic>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

using namespace mavsdk;
std::atomic<bool> g_stop{false};
void signal_handler(int signum) {
    g_stop = true;
}

static std::string flight_mode_to_str(Telemetry::FlightMode mode) {
    switch (mode) {
        case Telemetry::FlightMode::Unknown:        return "Unknown";
        case Telemetry::FlightMode::Ready:          return "Ready";
        case Telemetry::FlightMode::Takeoff:        return "Takeoff";
        case Telemetry::FlightMode::Hold:           return "Hold";
        case Telemetry::FlightMode::Mission:        return "Mission";
        case Telemetry::FlightMode::ReturnToLaunch: return "ReturnToLaunch";
        case Telemetry::FlightMode::Land:           return "Land";
        case Telemetry::FlightMode::Offboard:       return "Offboard";
        case Telemetry::FlightMode::FollowMe:       return "FollowMe";
        default:                                    return "Other";
    }
}

void setup_monitoring(Telemetry& telemetry) {
    telemetry.subscribe_flight_mode([](Telemetry::FlightMode flight_mode) {
        std::cout << "Flight Mode: " << flight_mode_to_str(flight_mode) << std::endl;
    });

    telemetry.subscribe_battery([](Telemetry::Battery battery) {
        std::cout << "Battery: " << std::fixed << std::setprecision(1) 
                  << battery.remaining_percent << " %" << std::endl;
    });
}

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
    Telemetry telemetry(system);
    setup_monitoring(telemetry);

    while (!g_stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\nReceived stop signal. Exiting." << std::endl;
    return 0;
}