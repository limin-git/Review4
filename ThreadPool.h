#pragma once
#include "QueueProcessor.h"


struct ThreadPool
{
    typedef boost::function<void()> Callable;

public:

    ThreadPool( size_t initial_thread_number = 2 )
    {
        while( initial_thread_number--)
        {
            m_processors.push_back( new QueueProcessor );
        }
    }

    ~ThreadPool()
    {
        BOOST_FOREACH( QueueProcessor* p, m_processors )
        {
            p->terminate();
        }

        BOOST_FOREACH( QueueProcessor* p, m_processors )
        {
            delete p;
        }
    }

    void queue_item( const Callable& item )
    {
        BOOST_FOREACH( QueueProcessor* p, m_processors )
        {
            if ( ! p->busy() )
            {
                p->queue_item( item );
                return;
            }
        }

        boost::thread t( item );
    }

    template<typename U> void queue_items( const U& items )
    {
        BOOST_FOREACH( const Callable& item, items )
        {
            queue_item( item );
        }
    }

private:

    std::vector<QueueProcessor*> m_processors;
};
