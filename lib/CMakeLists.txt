set(CMAKE_CXX_STANDARD 20)

add_library(itmoloops_lib 
    Instruments/Instrument.cpp  
    Instruments/Sampler.cpp  
    Instruments/Square.cpp  
    Instruments/Sine.cpp 
    Instruments/Triangle.cpp      
    Events/Note.cpp   
    Events/PatternRef.cpp   
    Effects/Gain.cpp  
    Effects/Tremolo.cpp  
    Effects/Echo.cpp  
    Engine.cpp
    Parser.cpp
    Pattern.cpp
)

target_include_directories(itmoloops_lib PUBLIC .)
