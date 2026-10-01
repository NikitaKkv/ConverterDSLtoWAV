#include <memory>
#include <string>
#include <utility>

#include "Pattern.hpp"
#include "Events/Event.hpp"
#include "Events/Note.hpp"

std::string Pattern::Name() const {
    return name_;
}

void Pattern::SetResolution(int resol){
    resolution_ = resol;
}

void Pattern::AddEvent (std::unique_ptr<Event> event){
    events_.push_back(std::move(event));
}

std::pair<Context, bool> Pattern::NextInstruction() {
    return {events_[cur_instruct_++]->Request(), cur_instruct_ == events_.size()};
}

void Pattern::SetIdxInstr(int idx){
    cur_instruct_ = idx;
}