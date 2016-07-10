#pragma once


template<typename T>
struct Singleton
{
    Singleton()
    {
        m_object = create();
    }

    ~Singleton()
    {
        destroy();
    }

    T* operator->()
    {
        return m_object;
    }

private:

    Singleton( const Singleton& rhs );
    Singleton& operator =( const Singleton& );

private:

    static T* create()
    {
        if ( NULL == object() )
        {
            object() = &T::instance();
        }

        reference_count()++;
        return object();
    }

    static void destroy()
    {
        if ( 0 == reference_count() )
        {
            throw;
        }

        reference_count()--;

        if ( 0 == reference_count() )
        {
            delete object();
            object() = NULL;
        }
    }

    static T*& object()
    {
        static T* t = NULL;
        return t;
    }

    static size_t& reference_count()
    {
        static size_t c = 0;
        return c;
    }

    T* m_object;
};
