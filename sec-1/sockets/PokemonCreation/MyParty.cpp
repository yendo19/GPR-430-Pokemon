#include "pokemonCreator.h"
#include <filesystem>
#include <experimental/filesystem>
void myParty::Init()
{
    CreateFolder();
}

void myParty::Update()
{

}

void myParty::CreateFiles()
{
    CreateFolder();
    std::string fileName = _name + ".txt";
    std::ofstream file(fs::path(folderName) / fileName);
}

void myParty::CreateFolder()
{
    std::string folderName = "pokemans";
    if (!fs::exists(folderName))
        fs::create_directory(folderName);
}

std::list<std::string> myParty::CheckForFiles()
{
    std::string folderName = "pokemans";
    std::list<std::string> holder = new std::list<std::string>();
    for (const auto& files : fs::directory_iterator(folderName)) {
        if (files.is_regular_file() && files.path().extension() == ".txt") {
            holder.push_back(files.path().filename().string());
        }
    }

    return holder;

}