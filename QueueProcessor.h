#pragma once


struct QueueProcessor
{
    typedef boost::function<void()> Callable;

    QueueProcessor()
    {
        m_busy = false;
        m_running = true;
        m_thread = boost::thread( boost::bind( &QueueProcessor::run, this ) );
    }

    ~QueueProcessor()
    {
        terminate();
    }

    void terminate()
    {
        m_running = false;
        queue_item( Callable() );
        m_condition.notify_one();
        m_thread.join();
        m_busy = false;
    }

    void queue_item( const Callable& item )
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        m_queue.push( item );
        m_condition.notify_one();
        m_busy = true;
    }

    template<typename U>
    void queue_items( const U& items )
    {
        if ( !items.empty() )
        {
            boost::unique_lock<boost::mutex> lock( m_mutex );

            BOOST_FOREACH( const Callable& item, items )
            {
                m_queue.push( item );
            }

            m_condition.notify_one();
            m_busy = true;
        }
    }

    void run()
    {
        while ( m_running )
        {
            try
            {
                Callable item = get_item();

                if ( m_running )
                {
                    item();
                    m_busy = ( ! m_queue.empty() );
                }
            }
            catch ( std::exception& )
            {
            }
            catch ( ... )
            {
            }
        }
    }

    Callable get_item()
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        Callable item;

        while ( m_queue.empty() && m_running )
        {
            m_condition.wait( lock );
        }

        if ( ! m_running || m_queue.empty() )
        {
            return Callable();
        }

        item = m_queue.front();
        m_queue.pop();
        return item;
    }


    bool busy()
    {
        return m_busy;
    }

public:

    std::queue<Callable> m_queue;
    boost::thread m_thread;
    volatile bool m_running;
    volatile bool m_busy;
    boost::mutex m_mutex;
    boost::condition_variable m_condition;
};
