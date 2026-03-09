#pragma once
#include <QString>

class Converter
{
public:
    enum class Mode { Length, Mass, Temperature };

    // unit keys are the short ones: "m", "km", "in", "ft", "mi", "kg", "lb", "oz", "C", "F", "K"
    static double convert(Mode mode, double value, const QString& fromUnit, const QString& toUnit);

private:
    // Length (base: meter)
    static double lengthToMeters(double v, const QString& u);
    static double metersToLength(double meters, const QString& u);

    // Mass (base: kilogram)
    static double massToKg(double v, const QString& u);
    static double kgToMass(double kg, const QString& u);

    // Temperature (base: Kelvin)
    static double tempToKelvin(double v, const QString& u);
    static double kelvinToTemp(double k, const QString& u);
};
