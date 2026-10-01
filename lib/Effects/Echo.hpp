#pragma once

#include <string>
#include <vector>

#include "Effect.hpp"

class Echo final : public Effect{
public:
    void SetFeature(const std::string& key, const std::string& value) override;
    void ApplyEffect(std::vector<double>& audio) override;
private:
    double delay_;
    double decay_;
};