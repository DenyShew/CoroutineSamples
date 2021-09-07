#ifndef __WAITER_H__
#define __WAITER_H__

template<typename T>
class Waiter
{
public:
    Waiter(bool isReady = false);
    void SetStatus(const bool& isReady);
    bool GetStatus()const;

public:
    void SetValue(T value)
    {
        this->value = value;
    }
    
protected:

    bool isReady;
    T value;
};

#endif // __WAITER_H__