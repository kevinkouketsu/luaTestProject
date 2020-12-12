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
    luwra::StateWrapper state;
	state.loadStandardLibrary();
    state["my_fun"] = LUWRA_WRAP(my_fun);

    if (state.runFile("teste.lua") == LUA_OK)
    {   
        auto a = state.get<std::function<int (int, int)>>("sum");
        std::cout << a(15, 15) << std::endl;
        std::cout << a(15, 15) << std::endl;
    }
    else
    {
        std::string errormsg = lua_tostring(state, -1);
        std::cout << errormsg << std::endl;
    }
}
