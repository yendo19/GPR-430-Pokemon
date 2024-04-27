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

#pragma region Serialize/Deserialize


#pragma endregion

#pragma endregion

#pragma region PC Functions
    //im gonna die
    void updatePc()
    {
        /*holder*/ bool i = false;
        getFileNames();
        for each (std::string file in fileNames)
        {
            if (!i)
                setParty(file);
            Pokemon temp(fileDir);
            temp.readFiles(file);
            PC.push_back(temp);
        }
    }


    void readPC()
    {
        for each (Pokemon poke in PC)
        {
           // poke.coutData();
            std::list<char> holder;
            holder = poke.serialize();
            for (char c : holder)
                std::cout << c;
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
        std::cout << " 4. Go Fight!\n";
        std::cout << " 5. Exit\n";
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
            return;
        case 5:
            exit(1);
        default:
            std::cin.clear();
            std::cin.ignore(9999, '\n');
            Update();
            break;
        }
        
    }


    void pokemonCreation()
    {
        system("cls");
        std::string name;
        std::string temp;
        int hp = 201, speed, k = 0;;
        bool possible = false;
        attacks atks[4];

        std::cout << "Enter Pokemon Name: ";
        std::cin >> name;
        std::cout << "\n         Pokemon Name:\n             " << name << std::endl << std::endl;


        std::cout << "Enter Pokemon HP (HP + Speed will equal 200. The more HP the Slower it will be!): ";
        std::cin >> hp;

        while (!possible)
        {
            if (hp <= 190 && hp >= 10)
            {
                possible = true;
                break;
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(9999, '\n');
                std::cout << "\nHP Cannot be more than 190 and less that 10!\n";
                std::cout << "Please re-enter valid HP: ";
                std::cin >> hp;
            }
        }

        speed = 200 - hp;
        std::cout << "\n         HP:    Speed:\n              " << hp << "      " << speed << std::endl << std::endl;

        for (int i = 0; i < 4; i++)
        {
            possible = false;
            std::cout << "\nEnter attack " << i+1 << " name: ";
            std::cin >> temp;
            atks[i].setName(temp);
            //NEED ATTACK DAMAGE LIMITER/BALANCE SOMEHOW
            std::cout << "Enter attack " << i+1 << " Damage (10-100): ";
            std::cin >> temp;
            while (!possible)
            {
                try
                {
                    std::stoi(temp);
                    if (std::stoi(temp) >= 10 && std::stoi(temp) <= 100)
                    {
                        possible = true;
                        atks[i].setDamage(std::stoi(temp));
                        std::cin.clear();
                        std::cin.ignore(9999, '\n');
                        break;
                    }
                    else
                    {
                        std::cin.clear();
                        std::cin.ignore(9999, '\n');
                        std::cout << "\nPlease Enter Valid Number: ";
                        std::cin >> temp;
                        break;
                    }
                }
                catch(...)
                {
                    std::cin.clear();
                    std::cin.ignore(9999, '\n');
                    std::cout << "\nPlease Enter Valid Number: ";
                    std::cin >> temp;
                }
            }
        }
        system("cls");
        std::cout << "*******************************************************\n";
        std::cout << "                  " << name << "               \n";
        std::cout << "          HP: " << hp << "          SPEED: " << speed << std::endl << std::endl;
        std::cout << "*******************************************************\n\n";
        

        for each (attacks atk in atks)
        {
            std::cout << "   Attack Name: " << atk.getName() << "     Attack Damage: " << atk.getDamage() << std::endl;
            k++;
        }


        std::cout << "\n\n\nCreate Pokemon? (Y/N): ";
        std::cin >> temp;
        possible = false;
        while (!possible)
        {
            if (temp == "y" || temp == "Y" || temp == "n" || temp == "N")
            {
                possible = true;
                break;
            }
            else
            {
                std::cin.clear();
                std::cin.ignore(9999, '\n');
                std::cout << "Please enter Y or N: ";
                std::cin >> temp;
            }
        }

        if ((temp == "y" || temp == "Y"))
        {
            Pokemon temp(name, hp, speed, atks, fileDir);
            temp.CreateFiles();
            std::cout << "\n\n POKEMON CREATED!" << std::endl;
            system("pause");
            Update();
        }
        else
        {
            std::cout << "\n\n POKEMON DELETED!" << std::endl;
            system("pause");
            Update();
        }

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

    /*Placeholder for now*/
    void setParty(std::string dir)
    {
        for (Pokemon pkm : _myPokemon)
        {
            pkm.readFiles(dir);
        }
    }


private:
	Pokemon _myPokemon[3];
	std::list<Pokemon> PC;
    std::list<std::string> fileNames;
	std::string fileDir;
};


