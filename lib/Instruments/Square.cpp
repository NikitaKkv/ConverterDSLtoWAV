#include <string>

#include "../Constants.hpp"
#include "Instrument.hpp"
#include "Square.hpp"

Square::Square(const std::string& name) :
    Instrument(name) {}

void Square::SetFeature(const std::string& key, const std::string& value){
    if (key == "duty") {
        duty_ = std::stoi(value);
    }
}

void Square::RenderNote (int start_samples, int duration_samples, double pitch, int velocity) {
    int period_samples = kSampleRate / pitch;
    for (int i = 0; i < duration_samples; ++i){
        double res = (i % period_samples < period_samples * duty_ / 100.0 ? 1 : -1);
        res = SetVelocity(res, velocity);
        res = SetRelAtt(res, attack_, release_, duration_samples, i);
        audio_[i + start_samples] += res;
    }
}