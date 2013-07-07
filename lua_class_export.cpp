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
	if (n != 1 && lua_type(L, -1) != LUA_TTABLE)
	{
		lua_pushstring(L, "incorrect argument");
		lua_error(L);
	}
	// get this pointer
	lua_getfield(L, -1, "__id");
	Test* pTest = (Test*)lua_touserdata(L, -1);
	if (pTest == NULL)
	{
		lua_pushstring(L, "fail to get this pointer");
		lua_error(L);
	}
	pTest->print_name();
	return 0;
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
	lua_setfield(L, -2, "__id");

	// TODO: set metatable
	lua_newtable(L);
	lua_getfield(L, LUA_REGISTRYINDEX, "Test");
	lua_setfield(L, -2, "__index");

	lua_setmetatable(L, -2);
	// return table
	return 1;
}

// meta table
int registe_function(lua_State *L)
{
	lua_register(L, "get_instance", lua_get_instance);

	lua_newtable(L);
	lua_pushcfunction(L, lua_print_name);
	lua_setfield(L, -2, "print_name");
	lua_setfield(L, LUA_REGISTRYINDEX, "Test");
	return 0;
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
	int result = luaL_dofile(L, "class_export.lua");
	if (LUA_OK != result)
	{
		printf("%s, error code: %d\n", "do file failed: class_export.lua", result);
	}
	lua_close(L);

	delete pInstance;
	pInstance = NULL;
	return 0;
}
