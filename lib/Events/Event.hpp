#pragma once

struct Context;

class Event{
public:
    Event (int start) : start_(start) {}
    Event () = default;
    
    virtual Context Request() = 0;
    virtual ~Event() = default;

protected:
    int start_;
};