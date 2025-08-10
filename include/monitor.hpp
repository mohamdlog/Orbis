#pragma once

#include <string>
#include <atomic>

namespace mavsdk {
    class Telemetry;
}

/**
 * @brief A global flag to signal the application to stop.
 */
 extern std::atomic<bool> g_stop;

 /**
 * @brief Signal handler to gracefully stop the application.
 * @param signum The signal number received.
 */
void signal_handler(int signum);

/**
 * @brief Sets up subscriptions to MAVSDK telemetry data.
 * @param telemetry A reference to the Telemetry plugin object.
 */
void setup_monitoring(mavsdk::Telemetry& telemetry);