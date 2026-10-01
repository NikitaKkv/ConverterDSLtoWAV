#pragma once

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "Engine.hpp"
#include "Instruments/Instrument.hpp"
#include "Pattern.hpp"

class Parser final{
public:
    Parser (const std::string& file) : in_(file) {}
    double Parse (Program& program);
    std::unique_ptr<Pattern>  ParsePattern();
    std::unique_ptr<Instrument>  ParseInstrument();

private:
    void FindLastNote(const std::vector<std::string>& str, std::unique_ptr<Pattern>& pattern);
    void ParseEffect(std::unique_ptr<Instrument>& instrument, const std::vector<std::string>& all_effs);
    void ReadEffects(std::unique_ptr<Instrument>& instrument);
    std::unordered_map<std::string, double> last_note_;
    std::ifstream in_;
};