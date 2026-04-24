#include <cmath>
#include <iostream>
#include <string>

#include "Avion.h"
#include "Camion.h"

namespace {

struct TestStats {
    int passed = 0;
    int failed = 0;
};

bool nearlyEqual(const double a, const double b, const double epsilon = 1e-9) {
    return std::fabs(a - b) <= epsilon;
}

void check(const bool condition, const std::string& testName, TestStats& stats) {
    if (condition) {
        ++stats.passed;
        std::cout << "[PASS] " << testName << '\n';
    } else {
        ++stats.failed;
        std::cout << "[FAIL] " << testName << '\n';
    }
}

void runCamionTests(TestStats& stats) {
    Camion camion;

    check(camion.getName() == "Camion", "Camion.getName returns expected value", stats);

    const Colis colisLimite(1000.0, "Standard");
    const Colis colisTropLourd(1000.1, "Lourd");

    check(camion.canDeliver(colisLimite), "Camion.canDeliver accepts weight at max threshold", stats);
    check(!camion.canDeliver(colisTropLourd), "Camion.canDeliver rejects weight above threshold", stats);

    const Colis colis(500.0, "Standard");
    const double distance = 100.0;

    // Formula in Camion.cpp:
    // 25.0 + (0.90 * distance) + (0.002 * weight * distance)
    const double expectedCost = 25.0 + (0.90 * distance) + (0.002 * 500.0 * distance);
    const double actualCost = camion.computeCost(colis, distance);

    check(nearlyEqual(actualCost, expectedCost), "Camion.computeCost follows expected formula", stats);

    const Colis colisPoidsNegatif(-12.0, "Invalid");
    const double costSafe = camion.computeCost(colisPoidsNegatif, -40.0);
    check(nearlyEqual(costSafe, 25.0), "Camion.computeCost clamps negative weight/distance to zero", stats);
}

void runAvionTests(TestStats& stats) {
    Avion avion;

    check(avion.getName() == "Avion", "Avion.getName returns expected value", stats);

    const Colis colisTresLourd(3500.0, "Cargo");
    check(avion.canDeliver(colisTresLourd), "Avion.canDeliver accepts very heavy package", stats);

    const Colis colis(300.0, "Express");
    const double distance = 120.0;

    // Formula in Avion.cpp:
    // base + distanceCost + handling + heavyFreightSurcharge
    // 180 + (2.75 * d) + (0.12 * w) + max(0, w - 200) * 0.35
    const double expectedCost = 180.0 + (2.75 * distance) + (0.12 * 300.0) + ((300.0 - 200.0) * 0.35);
    const double actualCost = avion.computeCost(colis, distance);

    check(nearlyEqual(actualCost, expectedCost), "Avion.computeCost follows expected formula with heavy surcharge", stats);

    const Colis colisNegatif(-5.0, "Invalid");
    const double safeCost = avion.computeCost(colisNegatif, -10.0);
    check(nearlyEqual(safeCost, 180.0), "Avion.computeCost clamps negative inputs safely", stats);
}

} // namespace

int main() {
    TestStats stats;

    runCamionTests(stats);
    runAvionTests(stats);

    std::cout << "\n===== TEST SUMMARY =====\n";
    std::cout << "Passed: " << stats.passed << '\n';
    std::cout << "Failed: " << stats.failed << '\n';

    // Convention standard: 0 = success, non-zero = failure.
    return (stats.failed == 0) ? 0 : 1;
}
