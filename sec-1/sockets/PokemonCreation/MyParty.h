#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "string.h"
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include "Pokemon.h"
#include "Attacks.h"
#include <list>
#include <filesystem>
#include <iostream>
#include <fstream>

class myParty
{
public:
	myParty() {};
	~myParty() {};
    //dumb shit lmao
    void Init(std::string file)
    {
        std::cout << "init" << std::endl;
        fileDir = file;
    }
    //something in here idk
    void Update()
    {

    }


    //im crying
    bool CreateFolder()
    {
        namespace fs = std::filesystem;
        fs::path p(fileDir + "/Pokemans");
        if (!fs::exists(p)) {
            return fs::create_directory(p);
        }
        return true;
    }

    //im gonna die
    void updatePc()
    {
        getFileNames();
        for each (std::string file in fileNames)
        {
            Pokemon temp(fileDir);
            temp.readFiles(file);
            PC.push_back(temp);
        }
    }

    void createPokemon(std::string name, int hp/*, attacks atk[4]*/)
    {
        Pokemon temp(name,hp, fileDir);
        temp.CreateFiles();
    }

    //gets textfile names
    void getFileNames()
    {
        namespace fs = std::filesystem;
        fs::path path(fileDir + "/Pokemans");
        if (fs::exists(path) && fs::is_directory(path))
        {
            for (const auto& entry : fs::directory_iterator(path))
            {
                if (entry.is_regular_file())
                {
                    fileNames.push_back(entry.path().filename().string());
                }
            }
        }
    }

    void readPC()
    {
        for each (Pokemon poke in PC)
        {
            std::cout << "IN PC: " << poke.getName() << std::endl;
        }
    }

private:
	//Pokemon _myPokemon[3];
	std::list<Pokemon> PC;
    std::list<std::string> fileNames;
	std::string fileDir;
};


