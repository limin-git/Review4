#pragma once
#include "QueueProcessor.h"


struct IThreadPool
{
    virtual ~IThreadPool() {}
    virtual void terminate() = 0;
    virtual void queue_item( const Function& item );
    virtual void queue_items( std::vector<Function>& items ) = 0;
    virtual void queue_items( std::list<Function>& items ) = 0;
};

template<size_t initial_thread_number = 0, size_t max_thread_number = -1, size_t max_queue_size = -1>
struct ThreadPool
{
public:

    ThreadPool()
    {
        for ( size_t i = 0; i < initial_thread_number; ++i)
        {
            m_processors.push_back( new QueueProcessor<max_queue_size> );
        }
    }

    ~ThreadPool()
    {
        terminate();

        BOOST_FOREACH( IQueueProcessor* p, m_processors )
        {
            delete p;
        }
    }

    virtual void terminate()
    {
        boost::lock_guard<boost::mutex> lock( m_mutex );

        BOOST_FOREACH( IQueueProcessor* p, m_processors )
        {
            p->terminate();
        }
    }

    void queue_item( const Function& item )
    {
        boost::lock_guard<boost::mutex> lock( m_mutex );

        BOOST_FOREACH( IQueueProcessor* p, m_processors )
        {
            if ( ! p->busy() )
            {
                p->queue_item( item );
                return;
            }
        }

        if ( m_processors.size() < max_thread_number )
        {
            IQueueProcessor* p = new QueueProcessor<max_queue_size>;
            p->queue_item( item );
            m_processors.push_back( p );
        }
        else
        {
            min_queue_processor().queue_item( item );
        }
    }

    virtual void queue_items( std::vector<Function>& items )
    {
        queue_items_impl( items );
    }

    virtual void queue_items( std::list<Function>& items )
    {
        queue_items_impl( items );
    }

private:

    template<typename U> void queue_items_impl( const U& items )
    {
        BOOST_FOREACH( const Function& item, items )
        {
            queue_item( item );
        }
    }

    IQueueProcessor& min_queue_processor()
    {
        struct Predicate
        {
            bool operator()( const IQueueProcessor* lhs, const IQueueProcessor* rhs ) const
            {
                return lhs->size() < rhs->size();
            }
        };

        static Predicate pred;
        std::vector<IQueueProcessor*> processors;
        {
            boost::lock_guard<boost::mutex> lock( m_mutex );
            processors = m_processors;
        }
        std::sort( processors.begin(), processors.end(), pred );
        return *processors.front();
    }

private:

    std::vector<IQueueProcessor*> m_processors;
    boost::mutex m_mutex;
};
