#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "Engine.hpp"
#include "Parser.hpp"
#include "Pattern.hpp"

#include "Events/Note.hpp"
#include "Events/PatternRef.hpp"

#include "Instruments/Instrument.hpp"
#include "Instruments/Sampler.hpp"
#include "Instruments/Square.hpp"
#include "Instruments/Sine.hpp"
#include "Instruments/Triangle.hpp"

#include "Effects/Effect.hpp"
#include "Effects/Echo.hpp"
#include "Effects/Gain.hpp"
#include "Effects/Tremolo.hpp"

bool IsRef(const std::string& s){
    return s[0] == '@';
}

bool IsEff(const std::string& str){
    return str == "echo" || str == "tremolo" || str == "gain";
}

void Read(std::ifstream& in, std::string& word) {
    in >> word;
    while (word == "#"){
        std::getline(in, word);
        in >> word;
    }
}

std::unique_ptr<Instrument> MakeInst(const std::string& type, 
                                     const std::string& name){
    if (type == "sampler"){
        return std::make_unique<Sampler>(name);
    }
    else if (type == "square"){
        return std::make_unique<Square>(name);
    }
    else if (type == "sine"){
        return std::make_unique<Sine>(name);
    }
    else if (type == "triangle"){
        return std::make_unique<Triangle>(name);
    }
    return nullptr;
}

std::unique_ptr<Effect> MakeEffect(const std::string& type){
    if (type == "gain"){
        return std::make_unique<Gain>();
    }
    else if (type == "echo"){
        return std::make_unique<Echo>();
    }
    else if (type == "tremolo"){
        return std::make_unique<Tremolo>();
    }
    return nullptr;
}


double Parser::Parse(Program& program){
    std::string word;
    while (in_ >> word){
        if (word == "#"){
            std::getline(in_, word);
            continue;
        }

        if (word == "bpm"){
            in_ >> word;
            program.SetBpm(std::stoi(word));
        }

        if (word == "pattern"){
            program.AddPattern(ParsePattern());
        }

        if (word == "instrument"){
            program.AddInstrument(ParseInstrument());
        }
    }

    return last_note_["main"];
}

void Parser::FindLastNote(const std::vector<std::string>& str, std::unique_ptr<Pattern>& pattern) {
    if (str[1][0] == '@'){
        last_note_[pattern->Name()] = std::stod(str[0]) / 
                    pattern->GetResolution() + last_note_[str[1].substr(1)];
    }
    else{
        last_note_[pattern->Name()] = (std::stod(str[0]) + std::stod(str[3])) / pattern->GetResolution();
    }
}

void EnterNote(int idx, std::ifstream& in, std::string& word, std::vector<std::string>& str) {
    while (idx < 5){
        Read(in, word);
        str[idx++] = word;
    }
}

std::unique_ptr<Pattern> Parser::ParsePattern() {
    std::string word;
    Read(in_, word);
    std::unique_ptr<Pattern> pattern = std::make_unique<Pattern>(word);

    Read(in_, word);
    Read(in_, word);   
    pattern->SetResolution(std::stoi(word));
    
    std::vector<std::string> str(5);

    while (true){
        int idx = 0;
        Read(in_, word);
        if (word == "end"){
            FindLastNote(str, pattern);
            break;
        }
        str[idx++] = word;

        Read(in_, word);
        str[idx++] = word;

        if (IsRef(word)){
            pattern->AddEvent(std::make_unique<PatternRef>(str));
        }
        else{
            EnterNote(idx, in_, word, str);
            pattern->AddEvent(std::make_unique<Note>(str));
        }
    }
    return pattern;
}

void ParseParams(const std::vector<std::string>& params, std::unique_ptr<Instrument>& instrument) {
    for (auto& i : params){
        int eq_pos = i.find_first_of('=');
        std::string key = i.substr(0, eq_pos);
        std::string value = i.substr(eq_pos + 1);
        instrument->SetFeature(key, value);
        instrument->Instrument::SetFeature(key, value);
    }
}

std::unique_ptr<Instrument> Parser::ParseInstrument() {
    std::string type, name, word;
    Read(in_, name);
    Read(in_, type);

    std::vector<std::string> params;

    std::unique_ptr<Instrument> instrument = MakeInst(type, name);

    while (true) {
        Read(in_, word);
        if (word == "end"){
            break;
        }
        params.push_back(word);
        if (word == "effect") {
            ReadEffects(instrument);
            break;
        }
    }

    ParseParams(params, instrument);
    
    return instrument;
}

void Parser::ParseEffect(std::unique_ptr<Instrument>& instrument, const std::vector<std::string>& all_effs){
    std::unique_ptr<Effect> effect = nullptr;
    for (const auto& word : all_effs){
        if (word == "effect"){
            if (!effect) continue;
            instrument->AddEffect(std::move(effect));
        }
        else if (IsEff(word)){
            effect = MakeEffect(word);
        }
        else {
            int eq_pos = word.find_first_of('=');
            std::string key = word.substr(0, eq_pos);
            std::string value = word.substr(eq_pos + 1);
            effect->SetFeature(key, value);
        }
    }
}

void Parser::ReadEffects(std::unique_ptr<Instrument>& instrument) {
    std::string word;
    std::vector<std::string> all_effs (1, "effect");
    while (true) {
        Read(in_, word);
        if (word == "end"){
            break;
        }
        all_effs.push_back(word);
    }    
    ParseEffect(instrument, all_effs);
}

