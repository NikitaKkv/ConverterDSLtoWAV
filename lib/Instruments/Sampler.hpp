#include <string>
#include <vector>
#include <utility>

#include "Instrument.hpp"

class Sampler final : public Instrument {
public:
    Sampler(const std::string& name);
    virtual void RenderNote (int start_samples, int duration_samples, double pitch, int velocity) override;
    void SetFeature (const std::string& key, const std::string& value) override;

private:
    void SetSamples(const std::string& value);
    void FillAudioSeg(std::vector<double>& tar, 
                     double relation, 
                     int duration_samples, 
                     int velocity, 
                     int start_samples,
                     int limits);
    void FillAudioCycle(std::vector<double>& tar, 
                     double relation, 
                     int duration_samples, 
                     int velocity, 
                     int start_samples,
                     int start_pos,
                     int limit);
    void Fill(double start, int limit, std::vector<double>& tar, double relation);
    std::vector<float> samples_;
    double root_;
    std::pair<int, int> loop_ = {-1, -1};
};