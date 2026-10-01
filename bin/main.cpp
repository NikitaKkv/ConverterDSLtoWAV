#include "../lib/Engine.hpp"
#include "../lib/Parser.hpp"

int main(int argc, char** argv) {
    Program program(argv[2]);
    Parser parser(argv[1]);
    program.SetSizeAudio(parser.Parse(program));
    program.Run();
}
