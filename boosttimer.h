#ifndef BOOSTTIMER_H
#define BOOSTTIMER_H

#include <boost/signals2.hpp>
#include <boost/thread.hpp>

/***** SAMPLES OF USAGE
    timer timer;
    timer.set_interval(30);
    timer.connect(_test_timer_handler);
    timer.start();
***************************************/
    
class BTimer
{
    typedef boost::signals2::signal<void ()> timeout_slot;
public:
    typedef timeout_slot::slot_type timeout_slot_t;

public:
    timer() : _interval(0), _is_active(false) {};
    timer(int interval) : _interval(interval), _is_active(false) {};
    virtual ~timer() { stop(); };

    inline boost::signals2::connection connect(const timeout_slot_t& subscriber) { return _signalTimeout.connect(subscriber); };

    void start()
    {
        boost::lock_guard<boost::mutex> lock(_guard);

        if (is_active())
            return; // Already executed.
        if (_interval <= 0)
            return;

        _timer_thread.interrupt();
        _timer_thread.join();

        timer_worker job;
        _timer_thread = boost::thread(job, this);

        _is_active = true;
    };

    void stop()
    {
        boost::lock_guard<boost::mutex> lock(_guard);

        if (!is_active())
            return; // Already executed.

        _timer_thread.interrupt();
        _timer_thread.join();

        _is_active = false;
    };

    inline bool is_active() const { return _is_active; };

    inline int get_interval() const { return _interval; };

    void set_interval(const int msec)
    {
        if (msec <= 0 || _interval == msec)
            return;

        boost::lock_guard<boost::mutex> lock(_guard);
        // Keep timer activity status.
        bool was_active = is_active();

        if (was_active)
            stop();
        // Initialize timer with new interval.
        _interval = msec;

        if (was_active)
            start();
    };

protected:
    friend struct timer_worker;
    // The timer worker thread.
    struct timer_worker
    {
        void operator()(timer* t)
        {
            boost::posix_time::milliseconds duration(t->get_interval());

            try
            {
                while (1)
                {
                    boost::this_thread::sleep<boost::posix_time::milliseconds>(duration);
                    {
                        boost::this_thread::disable_interruption di;
                        {
                            t->_signalTimeout();
                        }
                    }
                }
            }
            catch (boost::thread_interrupted const& )
            {
                // Handle the thread interruption exception.
                // This exception raises on boots::this_thread::interrupt.
            }
        };
    };

protected:
    int             _interval;
    bool            _is_active;

    boost::mutex    _guard;
    boost::thread   _timer_thread;

    // Signal slots
    timeout_slot    _signalTimeout;
};

#endif // BOOSTTIMER_H

