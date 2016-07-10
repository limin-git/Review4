#include "stdafx.h"
#include "TestSingleton.h"
#include "Singleton.h"


struct Test
{
    Test()
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    ~Test()
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    void foo()
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    static Test& instance()
    {
        if ( obj == NULL )
        {
            obj = new Test;
        }

        return *obj;
    }

    static void remove()
    {
        delete obj;
        obj = NULL;
    }

    static Test* obj;
};

Test* Test::obj = NULL;


TestSingleton::TestSingleton()
{
    test1();
}


void TestSingleton::test1()
{
    Singleton<Test> t, t2, t3;
    t->foo();
}
