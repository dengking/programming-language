
struct Interface
{
    Interface(){}
    virtual void callback(void *p) {}
    virtual ~Interface() {}
};
void run(Interface *i)
{
    int v = 0;
    i->callback(&v);
}