#include <string>
#include <vector>

#include "Event.hpp"
#include "Note.hpp"
#include "PatternRef.hpp"


PatternRef::PatternRef (const std::vector<std::string>& str) : 
        Event(std::stoi(str[0])), 
        ref_(str[1].substr(1)) {}

Context PatternRef::Request() {
         return Context(ref_, start_);
}