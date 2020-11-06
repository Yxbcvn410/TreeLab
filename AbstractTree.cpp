template<typename T>
class AbstractTree {
public:
    virtual void add(T value) = 0;

    virtual bool check(T value) = 0;

    virtual void remove(T value) = 0;
};