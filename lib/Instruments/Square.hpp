#pragma once

#include <string>

#include "Instrument.hpp"

class Square final : public Instrument{
public:
    Square(const std::string& name);
    virtual void RenderNote (int start_samples, int duration_samples, double pitch, int velocity) override;
    void SetFeature (const std::string& key, const std::string& value) override;

private:
    int duty_;
};