#include <iostream>
#include <vector>

class DirecOrFile
{
    public:
        DirecOrFile(std::string name) : m_name {name} {}
        virtual std::string getName() const = 0;
    protected:
        std::string m_name;
};

class Direc : public DirecOrFile
{
    public:
        Direc(std::string);
        ~Direc();
        std::string getName() const override;
        void mkdir(std::string);
        void touch(std::string);
        Direc* cd(std::string);
        Direc* cdPrevious();
        void rm(std::string);
        void rmdir(std::string);
        void ls() const;
        void pwd();
    private:
        std::vector<DirecOrFile*> dir;
        std::vector<std::string> pwdVec;
        Direc* previous = nullptr;
};

class File : public DirecOrFile
{
    public:
        File (std::string);
        std::string getName() const override;
};


