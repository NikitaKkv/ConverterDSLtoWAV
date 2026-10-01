#pragma once

#include <memory>
#include <string>
#include <vector>


#include "../Effects/Effect.hpp"

class Instrument{
public:
    Instrument(const std::string& name);
    
    virtual void RenderNote (int start_samples, int duration_samples, double pitch, int velocity) = 0;
    void AddEffect(std::unique_ptr<Effect> effect);

    virtual void SetFeature (const std::string& key, const std::string& value);
    void SetSizeAudio(int sz);

    std::string Name() const;
    std::vector<double>& GetResult();

    virtual ~Instrument() = default;

protected:
    std::vector<double> audio_;
    std::vector<std::unique_ptr<Effect>> effects_;
    std::string name_;
    double attack_ = 0.0f;
    double release_ = 0.0f;
};


double SetVelocity(double sample, int velocity);

double SetAttack(double sample, double attack, int duration_samples, int i);

double SetRelease(double sample, double release, int duration_samples, int i);

double SetRelAtt(double res, double attack_, double release_, int duration_samples, int i);