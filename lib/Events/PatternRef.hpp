#pragma once

#include <string>
#include <vector>

#include "Event.hpp"

class PatternRef final : public Event{

public:
    PatternRef (const std::vector<std::string>& str);
    Context Request() override;

private:
    std::string ref_;
};