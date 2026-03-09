#include "converter.h"
#include <stdexcept>

double Converter::convert(Mode mode, double value, const QString& fromUnit, const QString& toUnit)
{
    if (fromUnit == toUnit) return value;

    switch (mode) {
    case Mode::Length: {
        const double m = lengthToMeters(value, fromUnit);
        return metersToLength(m, toUnit);
    }
    case Mode::Mass: {
        const double kg = massToKg(value, fromUnit);
        return kgToMass(kg, toUnit);
    }
    case Mode::Temperature: {
        const double k = tempToKelvin(value, fromUnit);
        return kelvinToTemp(k, toUnit);
    }
    }
    return value;
}

// ----- Length: base meter -----
double Converter::lengthToMeters(double v, const QString& u)
{
    if (u == "m")  return v;
    if (u == "km") return v * 1000.0;
    if (u == "in") return v * 0.0254;
    if (u == "ft") return v * 0.3048;
    if (u == "mi") return v * 1609.344;
    throw std::invalid_argument("Unknown length unit");
}

double Converter::metersToLength(double meters, const QString& u)
{
    if (u == "m")  return meters;
    if (u == "km") return meters / 1000.0;
    if (u == "in") return meters / 0.0254;
    if (u == "ft") return meters / 0.3048;
    if (u == "mi") return meters / 1609.344;
    throw std::invalid_argument("Unknown length unit");
}

// ----- Mass: base kilogram -----
double Converter::massToKg(double v, const QString& u)
{
    if (u == "kg") return v;
    if (u == "lb") return v * 0.45359237;
    if (u == "oz") return v * 0.028349523125;
    throw std::invalid_argument("Unknown mass unit");
}

double Converter::kgToMass(double kg, const QString& u)
{
    if (u == "kg") return kg;
    if (u == "lb") return kg / 0.45359237;
    if (u == "oz") return kg / 0.028349523125;
    throw std::invalid_argument("Unknown mass unit");
}

// ----- Temperature: base Kelvin -----
double Converter::tempToKelvin(double v, const QString& u)
{
    if (u == "K") return v;
    if (u == "C") return v + 273.15;
    if (u == "F") return (v - 32.0) * 5.0/9.0 + 273.15;
    throw std::invalid_argument("Unknown temperature unit");
}

double Converter::kelvinToTemp(double k, const QString& u)
{
    if (u == "K") return k;
    if (u == "C") return k - 273.15;
    if (u == "F") return (k - 273.15) * 9.0/5.0 + 32.0;
    throw std::invalid_argument("Unknown temperature unit");
}
