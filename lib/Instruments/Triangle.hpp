#pragma once

#include <string>

#include "Instrument.hpp"

class Triangle final : public Instrument{
public:
    Triangle(const std::string& name);
    virtual void RenderNote (int start_samples, int duration_samples, double pitch, int velocity) override;
};