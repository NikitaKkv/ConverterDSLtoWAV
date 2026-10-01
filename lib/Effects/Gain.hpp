#pragma once

#include <string>
#include <vector>

#include "Effect.hpp"

class Gain final : public Effect{
public:
    void SetFeature(const std::string& key, const std::string& value) override;
    void ApplyEffect(std::vector<double>& audio) override;
private:
    double gain_;
};