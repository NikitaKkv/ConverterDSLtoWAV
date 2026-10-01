#pragma once

#include <string>
#include <vector>

#include "Event.hpp"

struct Context;

class Note final : public Event{
public:
    Note (const std::vector<std::string>& str);
    Note () = default;

    Context Request() override;

    std::string GetInstr() const;
    double GetPitch() const;
    int GetDuration() const;
    int GetVelocity() const;


private:
    std::string instr_;
    double pitch_;
    int duration_;
    int velocity_;
};

struct Context{
    Context (const std::string& name, int start) : add_pattern_(name), relat_start_units(start) {is_note_ = false;}
    Context (const Note& note, int start) : note_(note), relat_start_units(start) { is_note_ = true;}
    Note note_;
    std::string add_pattern_;
    bool is_note_;
    int relat_start_units;
};