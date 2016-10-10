#pragma once
namespace po = boost::program_options;


struct IGlobalSignals
{
    virtual ~IGlobalSignals() {}
    virtual void signal_next_slide() = 0;
    virtual void wait_next_slide() = 0;
    static IGlobalSignals& instance();
};
