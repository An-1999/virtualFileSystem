#include "File.h"

Direc::Direc(std::string name) : DirecOrFile (name) {
    pwdVec.push_back(name);
}

Direc::~Direc()
{
    for (int i = 0; i < dir.size(); ++i) {
        delete[] dir[i];
    }
}

std::string Direc::getName() const
{
    return DirecOrFile::m_name;
}

std::string File::getName() const
{
    return DirecOrFile::m_name;
}

void Direc::mkdir(std::string name)
{
    for (int i = 0; i < dir.size(); ++i) {
        if (name == dir[i]->getName()) {
            std::cout << "name repetition" << std::endl;
            return;
        }
    }
    dir.push_back(new Direc(name));
}

void Direc::touch(std::string name)
{
    for (int i = 0; i < dir.size(); ++i) {
        if (name == dir[i]->getName()) {
            std::cout << "name repetition" << std::endl;
            return;
        }
    }
    dir.push_back(new File(name));
}

Direc* Direc::cd(std::string name)
{
    for (int i = 0; i < dir.size(); ++i) {
        if (name == dir[i]->getName()) {
            Direc* d = dynamic_cast<Direc*>(dir[i]);
            if (d == nullptr) {
                std::cout << "cd: " << name << ": No such file or directory" << std::endl;
                return nullptr;
            }
            d->previous = this;
            return d;
        }
    }
    std::cout << "There is no directory by " << name << std::endl;
    return nullptr;
}

Direc* Direc::cdPrevious()
{
    return previous;
}

void Direc::rm(std::string name)
{
    for (int i = 0; i < dir.size(); ++i) {
        if (name == dir[i]->getName()) {
            File* f = dynamic_cast<File*>(dir[i]);
            if (f == nullptr) {
                std::cout << "cd: " << name << ": No such file or directory" << std::endl;
                return;
            }
            dir.erase(dir.begin() + i);
            return;
        }
    }
    std::cout << "There is no file by " << name << std::endl;
}

void Direc::rmdir(std::string name)
{
    for (int i = 0; i < dir.size(); ++i) {
        if (name == dir[i]->getName()) {
            Direc* d = dynamic_cast<Direc*>(dir[i]);
            if (d == nullptr) {
                std::cout << "cd: " << name << ": No such file or directory" << std::endl;
                return;
            }
            dir.erase(dir.begin() + i);
            return;
        }
    }
    std::cout << "There is no directory by " << name << std::endl;
}

void Direc::ls() const
{
    for (int i = 0; i < dir.size(); ++i) {
        std::cout << dir[i]->getName() << "   ";
    }
    std::cout << std::endl;
}

void Direc::pwd()
{
    Direc* currentDir = cdPrevious();
    if (currentDir){
        pwdVec.push_back(currentDir->getName());
    } else {
        std::cout << pwdVec[0] << std::endl;
        return;
    }
    while (currentDir->previous) {
        currentDir = currentDir->previous;
        pwdVec.push_back(currentDir->getName());
    }
    for (int i = pwdVec.size() - 1; i >= 0; --i) {
        std::cout << "/" << pwdVec[i];
    }
    std::cout << std::endl;
}

File::File (std::string name) : DirecOrFile(name) {}

