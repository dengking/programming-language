/* File : example.h */

#include <functional>
#include <iostream>

struct ParamStruct
{
    int a_;
    int b_;
};

using Callback = std::function<void(const ParamStruct &)>;

class Caller
{
private:
    Callback callback_;

public:
    Caller() {}
    ~Caller() { delCallback(); }
    void delCallback()
    {
    }
    void setCallback(const Callback &cb)
    {
        callback_ = cb;
    }
    void resetCallback() {}
    void call()
    {
        if (callback_)
            callback_(ParamStruct());
    }
};
