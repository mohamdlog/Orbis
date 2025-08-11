#pragma once

namespace mavsdk {
    class Telemetry;
}

/**
 * @brief Sets up subscriptions to MAVSDK telemetry data.
 * @param telemetry A reference to the Telemetry plugin object.
 */
void setup_monitoring(mavsdk::Telemetry &telemetry);