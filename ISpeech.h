#pragma once


struct ISpeech
{
    virtual ~ISpeech() {};
    virtual void speak( const std::wstring& word ) = 0;
    virtual void speak( const std::vector<std::wstring>& words ) = 0;
    static ISpeech& instance();
    static void remove();
};
