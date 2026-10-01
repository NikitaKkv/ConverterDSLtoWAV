#pragma once

#include <cmath>
#include <cstdint>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Events/Event.hpp"
#include "Instruments/Instrument.hpp"
#include "Pattern.hpp"

struct StackEl{
    StackEl(const std::string name, double start_beat) : name_(name), start_beat_(start_beat) {}
    std::string name_;
    double start_beat_;
};

class Program final{
public:
    Program (const std::string& file) : file_out_(file) {}
    void AddPattern(std::unique_ptr<Pattern> pattern);
    void AddInstrument(std::unique_ptr<Instrument> pattern);
    void Write();

    void SetBpm(int bpm);
    void SetSizeAudio(double last_beat);

    void Run();
    void Apply(Context context, double start_beat, int resolution);

private:
    void WriteInt32(int val, std::ofstream& out);
    void WriteInt16(int16_t val, std::ofstream& out);

    std::vector<StackEl> st_;
    std::vector<double> res_audio_;
    std::map<std::string, std::unique_ptr<Pattern>> patterns_;
    std::map<std::string, std::unique_ptr<Instrument>> instruments_;
    int bpm_;
    std::string file_out_;
};




