#include <string>
#include <vector>

#include "Echo.hpp"
#include "../Constants.hpp"

void Echo::SetFeature (const std::string& key, const std::string& value){
    if (key == "delay"){
        delay_ = std::stod(value);
    }
    else if (key == "decay"){
        decay_ = std::stod(value);
    }
}

void Echo::ApplyEffect(std::vector<double>& audio) {
    int d = delay_ * kSampleRate;
    for (int i = d; i < audio.size(); ++i)
        audio[i] += decay_ * audio[i - d];
}
