#include "Utils.h"

double roundToDecimals(double value, int decimals) {
    double factor = pow(10, decimals);
    return round(value * factor) / factor;
}
