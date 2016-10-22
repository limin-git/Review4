#include "stdafx.h"
#include "TlsParameter.h"


TlsMap TlsParameter::m_params;


boost::thread_specific_ptr<std::wstring>& TlsParameter::get_param( const std::wstring& name )
{
    TlsMap::iterator find = m_params.find( name );

    if ( find == m_params.end() )
    {
        find = m_params.insert( std::make_pair( name, new boost::thread_specific_ptr<std::wstring> ) ).first;
    }

    return *( find->second );
}
