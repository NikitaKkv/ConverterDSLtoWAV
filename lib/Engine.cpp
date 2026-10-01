#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "Constants.hpp"
#include "Engine.hpp"
#include "Pattern.hpp"
#include "Instruments/Instrument.hpp"
#include "Events/Event.hpp"

void Program::AddPattern(std::unique_ptr<Pattern> pattern){
    patterns_[pattern->Name()] = std::move(pattern);
}

void Program::AddInstrument(std::unique_ptr<Instrument> instr){
    instruments_[instr->Name()] = std::move(instr);
}

void Program::SetBpm(int bpm) {
    bpm_ = bpm;
}

int Pattern::GetResolution() const {
    return  resolution_;
}

void Program::SetSizeAudio(double last_beat){
    int samples = last_beat * static_cast<double>(kSecPerMin) / bpm_ * kSampleRate;
    res_audio_.resize(samples);
    for (auto& [name, instr] : instruments_){
        instr->SetSizeAudio(samples);
    }
}

std::pair<int, int> SamplesCalc(int relat_start_units, int resolution, double start_beat, int bpm, int unit_durat){

    double relative_beat = 1.0 * relat_start_units / resolution;
    double abs_beat = start_beat + relative_beat;
    double seconds = abs_beat * static_cast<double>(kSecPerMin) / bpm;
    int start_samples = seconds * kSampleRate;

    double duration_beats = 1.0 * unit_durat / resolution;
    double duration_seconds = duration_beats * static_cast<double>(kSecPerMin) / bpm;
    int duration_samples = duration_seconds * kSampleRate;
    
    return {start_samples, duration_samples};
}

void Program::Apply(Context context, double start_beat, int resolution){
    if (!context.is_note_){
        patterns_[context.add_pattern_]->SetIdxInstr(0);
        st_.push_back(StackEl(context.add_pattern_, 
                      start_beat + context.relat_start_units / resolution));
    }
    else{
        auto [start_samples, duration_samples] = 
            SamplesCalc(context.relat_start_units,
                        resolution,
                        start_beat,
                        bpm_,
                        context.note_.GetDuration());


        instruments_[context.note_.GetInstr()]->RenderNote(start_samples, 
                                                duration_samples,
                                                context.note_.GetPitch(), 
                                                context.note_.GetVelocity());
    }
}


void Program::Run(){
    st_.push_back(StackEl("main", 0.0f));

    while (!st_.empty()){
        auto action = patterns_[st_.back().name_]->NextInstruction();
        double start_beat = st_.back().start_beat_;
        int resolution = patterns_[st_.back().name_]->GetResolution();

        if (action.second){
            st_.pop_back();
        }
        Apply(action.first, start_beat, resolution);
    }

    for (auto& [name, instr] : instruments_){
        auto& res = instr->GetResult();
        for (int i = 0; i < res_audio_.size(); ++i){
            res_audio_[i] += res[i];
        }

    }
    Write();
}

void Program::WriteInt32(int val, std::ofstream& out){
    for (int i = 0; i < 4; ++i){
        out.put(static_cast<char>(val & 0xFF));
        val >>= 8;
    }
}

void Program::WriteInt16(int16_t val, std::ofstream& out){
    for (int i = 0; i < 2; ++i){
        out.put(static_cast<char>(val & 0xFF));
        val >>= 8;
    }
}

void Program::Write(){
    std::ofstream out(file_out_);
    out << "RIFF";
    WriteInt32(kHeaderSize + res_audio_.size() * kBytesEl, out);;
    out << "WAVE";
    out << "fmt ";
    WriteInt32(kSizeFmt, out);
    WriteInt16(kPCM, out);
    WriteInt16(kNumChannels, out);
    WriteInt32(kSampleRate, out);
    WriteInt32(kByteRate, out);
    WriteInt16(kBlockAlign, out);
    WriteInt16(kDepth, out);
    out << "data";
    WriteInt32(res_audio_.size() * kBytesEl, out);
    for (double sample : res_audio_){
        sample = std::max(-1.0, std::min(1.0, sample));
        WriteInt16(sample * std::numeric_limits<int16_t>::max(), out);
    }
}
