#include <math.h>
#include <cmath>
#include <string>

#include "Instrument.hpp"
#include "Sine.hpp"
#include "../Constants.hpp"

Sine::Sine(const std::string& name) :
    Instrument(name) {}

void Sine::RenderNote (int start_samples, int duration_samples, double pitch, int velocity) {
    double increment = 2.0 * M_PI * pitch / kSampleRate; 
    double phase = 0.0;

    for (int i = 0; i < duration_samples; ++i) {
        double res = std::sin(phase);           
        res = SetVelocity(res, velocity);      
        res = SetRelAtt(res, attack_, release_, duration_samples, i); 
        audio_[i + start_samples] += res;     
        phase += increment;                     
        if (phase > 2.0 * M_PI) phase -= 2.0 * M_PI; 
    }
}