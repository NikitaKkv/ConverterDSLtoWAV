#pragma once

#include <memory>
#include <string>
#include <vector>
#include <utility>

#include "Events/Event.hpp"
#include "Events/Note.hpp"

class Pattern final{
public:
    Pattern (const std::string& name) : name_(name) {}
    Pattern () = default;
    void AddEvent (std::unique_ptr<Event> event);
    std::pair<Context, bool> NextInstruction();

    std::string Name() const;
    int GetResolution() const;

    void SetResolution(int resolution);
    void SetIdxInstr(int idx);

private:
    int cur_instruct_ = 0;
    int resolution_;
    std::string name_;
    std::vector<std::unique_ptr<Event>> events_;
};