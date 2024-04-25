#pragma once
#include "MyParty.h"

void myParty::myParty()
{
    fileDir = "C:\\Pokemans";
    
}

void myParty::~myParty()
{
    //f
}

void myParty::Init()
{
    std::cout << "init";
}

void myParty::Update()
{

}

void myParty::CreateFiles()
{
    std::cout << "doing something";
}

void myParty::CreateFolder()
{
    std::cout << "doing something";
    ///CreateDirectory(directoryPath.c_str(), NULL);
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