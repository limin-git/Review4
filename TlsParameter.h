#pragma once
#include <boost/thread/tss.hpp>

typedef std::map< std::wstring, boost::thread_specific_ptr<std::wstring>* > TlsMap;
#define TLS_PARAM_PlayEnglish       L"PlayEnglish"


struct TlsParameter
{
public:

    static boost::thread_specific_ptr<std::wstring>& get_param( const std::wstring& name );

private:
    
    static TlsMap m_params;
};
