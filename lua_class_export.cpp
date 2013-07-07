#include <stdio.h>
#include <string.h>

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
		printf("%s", m_szName);
	}

private:
	char m_szName[32];
};

// lua function
void lua_print_name(lua_State *L)
{
}
// meta table
// create instance
// get instance
// call from lua
int main()
{
	Test test("hello, the world!\n");
	test.print_name();
	return 0;
}
