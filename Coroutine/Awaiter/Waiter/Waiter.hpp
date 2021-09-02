#ifndef __WAITER_H__
#define __WAITER_H__

template<typename T>
class Waiter
{
public:
    Waiter(bool isReady = false)
    {
        this->isReady = isReady;
    }

    void SetStatus(bool isReady = false)
    {
        this->isReady = isReady;
    }

    bool GetStatus()
    {
        return isReady;
    }
    
protected:

    bool isReady;
    T value;
};

#endif // __WAITER_H__