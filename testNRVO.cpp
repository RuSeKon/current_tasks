#include <iostream>

class foo
{
	public:
	foo() { std::cout << "foo::foo()\n";}
	foo(const foo&) { std::cout << "foo::foo(const foo& src)\n";}
	~foo() { std::cout << "foo::~foo\n";}
};

foo boo()
{
	foo tmp;
	return tmp;
}

int main()
{
	foo s = boo();
	return 0;
}
	
