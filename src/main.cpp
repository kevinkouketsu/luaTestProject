#include <iostream>
#include "luwra/luwra.hpp"
#include "ScriptInterface.hpp"

// Your function
int my_fun(int a, int b) 
{
    std::cout << a << " " << b << std::endl;
    return a + b;
}

class Timer{
    public:
    Timer()
    {
        last = std::chrono::steady_clock::now();
    }
    ~Timer()
    {
        std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - last).count() << std::endl;
    }
private:
    std::chrono::time_point<std::chrono::steady_clock> last;
};

struct CPosition 
{
    uint16_t X;
    uint16_t Y;
};

class ScriptManager
{
public:
    ScriptManager(luwra::StateWrapper state):
        m_LuaState{state}
    {
    }
    bool readScripts()
    {  
        using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
        for (const auto& dirEntry : recursive_directory_iterator("npcs"))
        {
            std::cout << "Trying to read " << dirEntry.path() << std::endl;
            if(m_LuaState.runFile(dirEntry.path().c_str()) == LUA_OK)
            {
                if(m_LuaState.get<std::function<void()>>("onTalk"))
                {
                    if(m_LuaState.get<std::function<int()>>("getNpcId"))
                    {
                        auto i = m_LuaState.get<std::function<int()>>("getNpcId")();
                        scriptsList[i] = std::make_shared<ScriptInterface>(m_LuaState, dirEntry.path());
                        std::cout << "Added NPC id " << i << std::endl;
                    }
                }
            }
            else
            {
                std::string errormsg = lua_tostring(m_LuaState, -1);
                std::cout << errormsg << std::endl;
            }
        }
    }

    void execute(int npcId, int b)
    {
        if(scriptsList.find(npcId) == std::end(scriptsList))
        {
            std::cout << "NpcID " << npcId << " not found";
            return;
        }
        std::cout << "Executing npcId " << npcId << std::endl; 
        scriptsList[npcId]->execute(b);
    }
private:
    std::map<uint32_t, std::shared_ptr<ScriptInterface>> scriptsList;
    luwra::StateWrapper m_LuaState;
};

struct CMob
{
    std::string Name;
    CPosition Position;

    std::string toString()
    {
        using namespace std::string_literals;
        return "my name is "s + Name + "\n";
    }
};

std::vector<CMob> mobs;

CMob* getPlayerById(int id)
{
    if (id >= mobs.size())
        return nullptr;

    std::cout << "Returning id " << id << std::endl;
    return &mobs[id];
}

int main(int, char**) 
{
    luwra::StateWrapper state;
	state.loadStandardLibrary();
    state["my_fun"] = LUWRA_WRAP(my_fun);
    state["getPlayerById"] = LUWRA_WRAP(getPlayerById);
    
    for(int i = 0; i < 1000; ++i)
        mobs.emplace_back();
    luwra::registerUserType<CPosition()>(
        state,
        "CPosition",
        {
            LUWRA_MEMBER(CPosition, X),
            LUWRA_MEMBER(CPosition, Y)
        }
    );
    luwra::registerUserType<CMob()>(
        state, 
        "CMob",
        {
            LUWRA_MEMBER(CMob, Name),
            LUWRA_MEMBER(CMob, Position),
            LUWRA_MEMBER(CMob, toString)
        }
    );

    ScriptManager scriptManager{state};
    scriptManager.readScripts();

    while(true)
    {
        int a;
        int b;
        std::cout << "Please, enter the NPCID you want to execute and the clientId" << std::endl;
        std::cin >> a >> b;

        scriptManager.execute(a, b);
    }
}
