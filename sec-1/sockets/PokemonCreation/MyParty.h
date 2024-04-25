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
#include <cstdlib>

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


#pragma region File Creating / Accessing
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
#pragma endregion

#pragma region PC Functions
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


    void readPC()
    {
        for each (Pokemon poke in PC)
        {
            poke.coutData();
        }
    }
#pragma endregion

#pragma region console output
    void Update()
    {
        system("cls");
        int i;
        std::cout << "______     _                                _____                           _              \n";
        std::cout << "| ___ \\   | |                              |  __ \\                         | |             \n";
        std::cout << "| |_/ /__ | | _____ _ __ ___   ___  _ __   | |  \\/ ___ _ __   ___ _ __ __ _| |_ ___  _ __  \n";
        std::cout << "|  __/ _ \\| |/ / _ \\ '_ ` _ \\ / _ \\| '_ \\  | | __ / _ \\ '_ \\ / _ \\ '__/ _` | __/ _ \\| '__| \n";
        std::cout << "| | | (_) |   <  __/ | | | | | (_) | | | | | |_\\ \\  __/ | | |  __/ | | (_| | || (_) | |    \n";
        std::cout << " \\|  \\___/|_|\\_\\___|_| |_| |_|\\___/|_| |_|  \\____/\\___|_| |_|\\___|_|  \\__,_|\\__\\___/|_| \n";
        std::cout << " \n\n 1. Create new Pokemon\n";
        std::cout << " 2. Choose Party\n";
        std::cout << " 3. Check PC\n";
        std::cout << " 4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> i; 

        switch (i)
        {
        case 1:
            pokemonCreation();
            break;
        case 2:
            chooseParty();
            break;
        case 3:
            checkPC();
            break;
        case 4:
            exit(0);
            break;
        default:
            break;
        }
        
    }


    void pokemonCreation()
    {
        system("cls");

    }


    void chooseParty()
    {
        system("cls");
    }

    void checkPC()
    {
        system("cls");
    }


#pragma endregion

    void createPokemon(std::string name, int hp,int speed,attacks atk[4])
    {
        Pokemon temp(name,hp,speed,atk,fileDir);
        temp.CreateFiles();
    }


private:
	//Pokemon _myPokemon[3];
	std::list<Pokemon> PC;
    std::list<std::string> fileNames;
	std::string fileDir;
};


