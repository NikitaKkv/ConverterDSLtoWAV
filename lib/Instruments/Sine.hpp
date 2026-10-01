#pragma once

#include <string>

#include "Instrument.hpp"

class Sine final : public Instrument{
public:
    Sine(const std::string& name);
    virtual void RenderNote (int start_samples, int duration_samples, double pitch, int velocity) override;
};