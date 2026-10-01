#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <utility>

#include "../Constants.hpp"
#include "Sampler.hpp"
#include "Instrument.hpp"

Sampler::Sampler(const std::string& name) :
    Instrument(name) {}

void Sampler::SetSamples(const std::string& value) {
    std::ifstream in(value, std::iostream::binary);

    char buffer[4];
    in.seekg(12, std::ios::cur);
    while (in.read(buffer, 4)){
        uint32_t size = 0;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (std::strncmp(buffer, "data", 4) == 0){
            int16_t sample;
            for (int i = 0; i < size / 2; ++i) { 
                in.read(reinterpret_cast<char*>(&sample), sizeof(sample));
                samples_.push_back(1.0 * sample / std::numeric_limits<int16_t>::max());
            }
            break;
        }
        else{
            in.seekg(size, std::ios::cur);
        }
    }
}

void Sampler::SetFeature(const std::string& key, const std::string& value){
    if (key == "sample") {
        SetSamples(value);
    }
    else if (key == "root") {
        root_ = notes[value];
    }
    else if (key == "loop") {
        int sep = value.find_first_of(',');
        int start = std::stoi(value.substr(0, sep));
        int finish = std::stoi(value.substr(sep + 1));
        loop_ = std::make_pair(start, finish);
    }
}

void Sampler::Fill(double start, int limit, std::vector<double>& tar, double relation) {
    for (double i = start; true; ++i){
        double new_idx = i / relation;
        int int_idx = std::floor(new_idx);
        if (int_idx + 1 >= limit) {
            break;
        }
        double value = samples_[int_idx] + (new_idx - int_idx) * (samples_[int_idx + 1] - samples_[int_idx]); 
        tar.push_back(value);
    }
}

void Sampler::FillAudioSeg(std::vector<double>& tar, double relation, int duration_samples, 
                          int velocity, int start_samples, int limit) {
    Fill(0, limit, tar, relation);
    for (int i = 0; i < duration_samples && i < tar.size(); ++i){
        double res = SetVelocity(tar[i], velocity);
        audio_[i + start_samples] += SetRelAtt(res, attack_, release_, duration_samples, i);
    }
}

void Sampler::FillAudioCycle(std::vector<double>& tar, double relation, int duration_samples, 
                          int velocity, int start_samples, int start_pos, int limit) {
    double i = 0;
    while (i / relation < loop_.first) { ++i; }

    Fill(i, limit, tar, relation);
    for (int i = start_pos; i < duration_samples; ++i){
        double res = SetVelocity(tar[(i - start_pos) % tar.size()], velocity);
        audio_[i + start_samples] += SetRelAtt(res, attack_, release_, duration_samples, i);
    }
}

void Sampler::RenderNote (int start_samples, int duration_samples, double pitch, int velocity) {
    
    if (start_samples + duration_samples > audio_.size()){
        duration_samples = audio_.size() - start_samples;
    }

    double relation = root_ / pitch;

    std::vector<double> before_loop, loop, full;
    if (loop_.first == -1) {
        FillAudioSeg(full, relation, duration_samples, velocity, start_samples, samples_.size() - 1);
    }
    else {
        FillAudioSeg(before_loop, relation, duration_samples, velocity, start_samples, loop_.first);
        FillAudioCycle(loop, relation, duration_samples, velocity, start_samples,
                     before_loop.size(), loop_.second);
    }

}