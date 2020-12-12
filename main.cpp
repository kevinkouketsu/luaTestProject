#include <iostream>
#include "luwra/luwra.hpp"

// Your function
int my_fun(int a, int b) 
{
    std::cout << a << " " << b << std::endl;
    return a + b;
}

int main(int, char**) 
{
    luwra::State* state = luaL_newstate();
    luaL_openlibs(state);

    lua_CFunction cfun = LUWRA_WRAP(my_fun);
    luwra::setGlobal(state, "my_fun", cfun);

    if (luaL_dofile(state, "teste.lua") == LUA_OK)
    {   
        std::cout << "Worked" << std::endl;
    }
    else
    {
        std::string errormsg = lua_tostring(state, -1);
        std::cout << errormsg << std::endl;
    }
}
