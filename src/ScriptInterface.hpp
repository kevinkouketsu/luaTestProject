#pragma once

class ScriptInterface
{
public:
    ScriptInterface(luwra::StateWrapper luaState, const std::string& fileName);

    void execute(int clientId);

private:
    luwra::StateWrapper m_LuaState;
    std::string m_FileName;    
};