#pragma once

// from https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
template <class T>
class Singleton
{
    protected:
    inline static T* _instance = nullptr;

    protected:
    Singleton() = default;
    virtual ~Singleton() = default;

    public:
    Singleton(const Singleton& other) = delete;
    Singleton &operator=(const Singleton other) = delete;

    public:
    static T &getInstance()
    {
        if(_instance == nullptr)
        {
            _instance = new T;
        }
        
        return *_instance;
    }
};