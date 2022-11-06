#include <gum-system.h>

int main(int argc, char** argv)
{
	Gum::Output::print("#");
	Gum::Output::print("# Filesystem Testing:");
	Gum::Output::print("#");

    std::string execDir = Gum::Filesystem::getExecutablePath();
	Gum::Output::print("Current Executable directory: " + execDir);

    std::string projDir = std::string(EXAMPLES_PATH) + "/..";
    Gum::Output::print("Project directory contents ("+projDir+"):");
    Gum::Filesystem::iterateThroughDirectory(projDir, [](Gum::Filesystem::File file) {
        Gum::Output::print("    " + file.name + " (" + Gum::Filesystem::filetypeToString(file.type) + ")");
    });

	return 0;
};
