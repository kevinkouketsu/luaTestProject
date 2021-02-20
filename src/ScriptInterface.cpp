#include "pch.h"
#include "ScriptInterface.hpp"

ScriptInterface::ScriptInterface(luwra::StateWrapper state, const std::string& fileName):
    m_FileName{ fileName },
    m_LuaState{ state }
{
}

void ScriptInterface::execute(int clientId)
{
    if (m_LuaState.runFile(m_FileName.c_str()) == LUA_OK)
    {   
        auto a = m_LuaState.get<std::function<void(int)>>("onTalk");
        if (a)
        {
            a(clientId);
        }
    }
}