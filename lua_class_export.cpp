#include <stdio.h>
#include <string.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

// class
class Test
{
public:
	Test(const char* _szName)
	{
		strncpy(m_szName, _szName, sizeof(m_szName) - 1);
		m_szName[sizeof(m_szName)] = 0;
	}

	void print_name()
	{
		printf("%s\n", m_szName);
	}

private:
	char m_szName[32];
};

Test *pInstance = NULL;

// lua function
int lua_print_name(lua_State *L)
{
	int n = lua_gettop(L);
	if (n > 2)
	{
		lua_pushstring(L, "incorrect argument");
		lua_error(L);
	}
	// get this pointer
	// this->print_name();
}

// get instance & bind metatable
int lua_get_instance(lua_State *L)
{
	if (pInstance == NULL)
	{
		lua_pushstring(L, "can't find instance");
		lua_error(L);
	}
	lua_newtable(L);
	lua_pushlightuserdata(L, (void*)pInstance);
	lua_setfield(L, -1, "__id");
	// TODO: set metatable
	// return table
	return 1;
}

// meta table
int registe_function(lua_State *L)
{
	lua_newtable(L);
	lua_pushcfunction(L, lua_print_name);
	lua_setfield(L, -2, "print_name");
	lua_setfield(L, LUA_REGISTRYINDEX, "Test");

	lua_register(L, "get_instance", lua_get_instance);
}

// call from lua
int main()
{
	Test test("hello, the world!");
	test.print_name();

	// create instance
	pInstance = new Test("please call me from lua");

	lua_State* L = luaL_newstate();
	if (L == NULL)
	{
		printf("%s\n", "fail to create lua state");
		return 1;
	}

	registe_function(L);
	// TODO: call a lua script file
	luaL_dofile(L, "class_export.lua");
	lua_close(L);

	delete pInstance;
	pInstance = NULL;
	return 0;
}
