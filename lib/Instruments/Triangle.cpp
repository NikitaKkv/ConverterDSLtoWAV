#include <string>

#include "Instrument.hpp"
#include "Triangle.hpp"
#include "../Constants.hpp"

Triangle::Triangle(const std::string& name) :
    Instrument(name) {}

void Triangle::RenderNote(int start_samples, int duration_samples, double pitch, int velocity) {
    int period_samples = kSampleRate / pitch; 
    for (int i = 0; i < duration_samples; ++i) {
        int pos_in_period = i % period_samples;
        double res;
        if (pos_in_period < period_samples / 2) {
            res = -1.0 + 4.0 * pos_in_period / period_samples;
        } 
        else {
            res = 3.0 - 4.0 * pos_in_period / period_samples;
        }
        res = SetVelocity(res, velocity);                  
        res = SetRelAtt(res, attack_, release_, duration_samples, i); 
        audio_[i + start_samples] += res;             
    }
}
