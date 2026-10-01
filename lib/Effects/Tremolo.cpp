#include <cmath>
#include <math.h>
#include <string>
#include <vector>

#include "../Constants.hpp"
#include "Tremolo.hpp"

void Tremolo::SetFeature(const std::string& key, const std::string& value) {
    if (key == "freq"){
        freq_ = std::stoi(value);
    }
    else if (key == "depth"){
        depth_ = std::stod(value);
    }
}

void Tremolo::ApplyEffect(std::vector<double>& audio) {
    for (int i = 0; i < audio.size(); ++i) {
        double t = i / static_cast<double>(kSampleRate);
        audio[i] *= (1 - depth_ + depth_ * sin(2 * M_PI * freq_ * t));
    }
}