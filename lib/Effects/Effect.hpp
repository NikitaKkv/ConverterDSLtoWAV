#pragma once

#include <vector>
#include <string>

class Effect{
public:
    virtual void SetFeature (const std::string& key, const std::string& value) = 0;
    virtual void ApplyEffect(std::vector<double>& audio) = 0;
    virtual ~Effect() = default;
};
