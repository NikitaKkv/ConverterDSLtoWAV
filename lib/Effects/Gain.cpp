#include <string>
#include <vector>

#include "Gain.hpp"

void Gain::SetFeature(const std::string& key, const std::string& value) {
    if (key == "gain"){
        gain_ = std::stod(value);
    }
}

void Gain::ApplyEffect(std::vector<double>& audio) {
    for (double& i : audio){
        i *= gain_;
    }
}