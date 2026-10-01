#include <algorithm>
#include <climits>
#include <cstring>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "Instrument.hpp"
#include "../Effects/Effect.hpp"
#include "../Constants.hpp"


Instrument::Instrument(const std::string& name) :
    name_(name), 
    attack_(0.0f), 
    release_(0.0f) {}


std::vector<double>& Instrument::GetResult(){
    for (auto& effect : effects_){
        effect->ApplyEffect(audio_);
    }
    return audio_;
}

void Instrument::SetFeature(const std::string& key, const std::string& value){
    if (key == "attack") {
        attack_ = std::stod(value);
    }
    else if (key == "release") {
        release_ = std::stod(value);
    }
}

void Instrument::AddEffect(std::unique_ptr<Effect> effect){
    effects_.push_back(std::move(effect));
}

std::string Instrument::Name() const {
    return name_;
}

void Instrument::SetSizeAudio(int sz){
    audio_.resize(sz);
}


double SetVelocity(double sample, int velocity){
    return sample * velocity / 100.0;
}

double SetAttack(double sample, double attack, int duration_samples, int i){
    if (attack == 0) {
        return  sample;
    }
    int attack_samples = attack * kSampleRate;
    attack_samples = std::min(attack_samples, duration_samples);
    return i < attack_samples ? (sample * i) / attack_samples : sample;
}

double SetRelease(double sample, double release, int duration_samples, int i){
    if (release == 0) {
        return sample;
    }
    int release_samples = release * kSampleRate;
    release_samples = std::min(release_samples, duration_samples);
    return i + release_samples > duration_samples ? (sample * (duration_samples - i)) / release_samples : sample;   
}

double SetRelAtt(double res, double attack_, double release_, int duration_samples, int i){
    res = SetAttack(res, attack_, duration_samples, i);
    res = SetRelease(res, release_, duration_samples, i);
    return res;
}

//builder pattern