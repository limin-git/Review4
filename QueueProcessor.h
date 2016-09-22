#pragma once


typedef boost::function<void()> Function;

struct IQueueProcessor
{
    virtual ~IQueueProcessor() {}
    virtual void terminate() = 0;
    virtual void queue_item( const Function& item ) = 0;
    virtual void queue_items( std::vector<Function>& items ) = 0;
    virtual void queue_items( std::list<Function>& items ) = 0;
    virtual bool busy() const = 0;
    virtual size_t size() const = 0;
};

template<size_t max_queue_size = -1>
struct QueueProcessor : IQueueProcessor
{
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

    virtual void terminate()
    {
        m_busy = true;
        m_running = false;
        m_condition.notify_one();
        m_thread.join();
    }

    virtual void queue_item( const Function& item )
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        while ( max_queue_size <= m_queue.size() )
        {
            m_queue.pop();
        }
        m_queue.push( item );
        m_condition.notify_one();
        m_busy = true;
    }

    virtual void queue_items( std::vector<Function>& items )
    {
        queue_items_impl( items );
    }

    virtual void queue_items( std::list<Function>& items )
    {
        queue_items_impl( items );
    }

    virtual bool busy() const
    {
        return m_busy;
    }

    virtual size_t size() const
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        return m_queue.size();
    }

private:

    template<typename U>
    void queue_items_impl( const U& items )
    {
        if ( !items.empty() )
        {
            boost::unique_lock<boost::mutex> lock( m_mutex );

            BOOST_FOREACH( const Function& item, items )
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
                Function item = get_item();

                if ( m_running && item )
                {
                    item();
                    boost::unique_lock<boost::mutex> lock( m_mutex );
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

    Function get_item()
    {
        boost::unique_lock<boost::mutex> lock( m_mutex );
        Function item;

        while ( m_queue.empty() && m_running )
        {
            m_condition.wait( lock );
        }

        if ( ! m_running || m_queue.empty() )
        {
            return Function();
        }

        item = m_queue.front();
        m_queue.pop();
        return item;
    }

public:

    std::queue<Function> m_queue;
    boost::thread m_thread;
    volatile bool m_running;
    volatile bool m_busy;
    mutable boost::mutex m_mutex;
    boost::condition_variable m_condition;
};
