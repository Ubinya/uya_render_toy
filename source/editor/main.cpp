#include <filesystem>
#include <iostream>
#include <string>
#include <thread>

#include "engine/engine.h"

#include "editor/editor.h"

int main(int argc, char** argv)
{
    std::filesystem::path executable_path(argv[0]);
    std::filesystem::path config_file_path = executable_path.parent_path() / "Editor.ini";

    Uya::UyaEngine* engine = new Uya::UyaEngine();

    engine->startEngine(config_file_path.generic_string());
    engine->initialize();

    Uya::Editor* editor = new Uya::UyaEditor();
    editor->initialize(engine);

    editor->run();

    editor->clear();

    engine->clear();
    engine->shutdownEngine();

    return 0;
}