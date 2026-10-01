#include <string>
#include <vector>

#include "Event.hpp"
#include "Note.hpp"
#include "../Constants.hpp"

Note::Note (const std::vector<std::string>& str) : 
        Event(std::stoi(str[0])),
        instr_(str[1]),
        pitch_(notes[str[2]]),
        duration_(std::stoi(str[3])),
        velocity_(std::stoi(str[4])) {}


Context Note::Request() {
         return Context(*this, start_);
}


std::string Note::GetInstr() const{
        return instr_;
}

double Note::GetPitch() const{
        return pitch_;
}

int Note::GetDuration() const{
        return duration_;
}

int Note::GetVelocity() const{
        return velocity_;
}