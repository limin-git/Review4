#pragma once


template<typename T>
struct QueueProcessor
{
    typedef boost::function<void (const T&)> Callback;

    QueueProcessor( const Callback& callback = Callback() )
        : m_callback( callback )
    {
        m_running = true;
        m_thread = boost::thread( boost::bind( &QueueProcessor::run, this ) );
    }

    ~QueueProcessor()
    {
        terminate();
    }

    void set_callback( const Callback& callback )
    {
        m_callback = callback;
    }

    void terminate()
    {
        m_running = false;
        m_condition.notify_one();
        m_thread.join();
    }

    void queue_item( const T& item )
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        m_queue.push( item );
        m_condition.notify_one();
    }

    template<typename U>
    void queue_items( const U& items )
    {
        if ( !items.empty() )
        {
            boost::unique_lock<boost::mutex> lock( m_mutex );

            BOOST_FOREACH( const T& item, items )
            {
                m_queue.push( item );
            }

            m_condition.notify_one();
        }
    }

    void run()
    {
        while ( m_running )
        {
            try
            {
                T item = get_item();

                if ( m_running )
                {
                    m_callback( item );
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

    T get_item()
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        T item;

        while ( m_queue.empty() )
        {
            m_condition.wait( lock );
        }

        if ( m_queue.empty() )
        {
            return T();
        }

        item = m_queue.front();
        m_queue.pop();
        return item;
    }

public:

    std::queue<T> m_queue;
    boost::thread m_thread;
    volatile bool m_running;
    boost::mutex m_mutex;
    boost::condition_variable m_condition;
    boost::function<void (const T&)> m_callback;
};
