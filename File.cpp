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

/////// class end //////



void pars_token(std::string str, std::vector<std::string>& token)
{
    std::string current;
    for (int i = 0; i < str.size(); ++i)
    {
        if (str[i] != ' ' && str[i] != '/') {
            current += str[i];
        } else if (!current.empty()) {
            if (str[i] == '/') {
                current += str[i];
            }
            token.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) {
        token.push_back(current);
    }
}

bool haveFolder(Direc* dir, std::vector<std::string>& token)
{
    for (int i = 1; i < token.size(); ++i) {
        if (token[i] == "..") {
            if (i == (token.size() - 1)) {
                if ((dir->cdPrevious() != nullptr)) {
                    dir = dir->cdPrevious();
                } else {
                    return false;
                }
            } else {
                std::cout << "cd: " << token[i] << ": No such file or directory" << std::endl;
                return false;
            }
        } else if (token[i] == "../") {
            if ((dir->cdPrevious() != nullptr)) {
                dir = dir->cdPrevious();
            }
        } else if ((token[i][token[i].size() - 1]) != '/') {
            if (i == (token.size() - 1)) {
                if (dir->cd(token[i]) != nullptr) {
                    dir = dir->cd(token[i]);
                } else {
                    return false;
                }
            } else {
                std::cout << "cd: too many arguments" << std::endl;
                return false;
            }
        } else if ((token[i][token[i].size() - 1]) == '/') {
            std::string current = token[i].substr(0, token[i].size() -1);
            if (dir->cd(current) != nullptr) {
                dir = dir->cd(current);
            } else {
                return false;
            }
        } else {
            std::cout << "cd: too many arguments" << std::endl;
            return false;
        }
    }
    return true;
}

void helpCdAndLs(Direc*& dir, std::vector<std::string>& instrucToken)
{
    for (int i = 1; i < instrucToken.size(); ++i) {
        if (instrucToken[i] == "..") {
            if (i == (instrucToken.size() - 1)) {
                if ((dir->cdPrevious() != nullptr)) {
                    dir = dir->cdPrevious();
                }
            }
        } else if (instrucToken[i] == "../") {
            if ((dir->cdPrevious() != nullptr)) {
                dir = dir->cdPrevious();
            }
        } else if ((instrucToken[i][instrucToken[i].size() - 1]) != '/') {
            if (i == (instrucToken.size() - 1)) {
                if (dir->cd(instrucToken[i]) != nullptr) {
                    dir = dir->cd(instrucToken[i]);
                }
            }
        } else if ((instrucToken[i][instrucToken[i].size() - 1]) == '/') {
            std::string current = instrucToken[i].substr(0, instrucToken[i].size() -1);
            if (dir->cd(current) != nullptr) {
                dir = dir->cd(current);
            }
        }
    }
}

void fileWord(Direc*& dir) {

    std::vector<std::string> instrucToken;
    while(true) {
        std::cout << "> ";
        std::string instruction;
        std::getline(std::cin, instruction);
        instrucToken.clear();
        pars_token(instruction, instrucToken);
        if (instruction == "") {
        } else if (instrucToken[0] == "mkdir") {
            for (int i = 1; i < instrucToken.size(); ++i) {
                dir->mkdir(instrucToken[i]);
            }
        } else if(instrucToken[0] == "touch") {
            for (int i = 1; i < instrucToken.size(); ++i) {
                dir->touch(instrucToken[i]);
            }
        } else if(instrucToken[0] == "ls") {
            if (instrucToken.size() == 1) {
                dir->ls();
            } else {
                if(haveFolder(dir, instrucToken)) {
                    helpCdAndLs(dir, instrucToken);
                    dir->ls();
                }
            }
        } else if(instrucToken[0] == "rmdir") {
            for (int i = 1; i < instrucToken.size(); ++i) {
                dir->rmdir(instrucToken[i]);
            }
        } else if(instrucToken[0] == "rm") {
            for (int i = 1; i < instrucToken.size(); ++i) {
                dir->rm(instrucToken[i]);
            }
        } else if(instruction.find("pwd") == 0) {
            dir->pwd();
        } else if(instrucToken[0] == "cd") {
            if(haveFolder(dir, instrucToken)) {
                helpCdAndLs(dir, instrucToken);
            }
        } else {
            std::cout << instruction << ": command not found" << std::endl;
        }
    }
}


int main()
{
    Direc* dir (new Direc("root"));
    std::vector<std::string> path;
    fileWord(dir);
    delete dir;
    return 0;
}
