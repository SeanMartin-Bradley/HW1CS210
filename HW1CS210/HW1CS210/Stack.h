#pragma once
template <typename Object>
class Stack
{
public:
    bool isEmpty() const
    {
        return theList.empty();
    }
    const Object& top() const
    {
        return theList.back();
    }
    void push(const Object& x)
    {
        theList.push_back(x);
    }
    Object& pop()
    {
        theList.pop_back();
        return *theList.begin();
    }
private:
    List<Object> theList;
};