
struct Interface
{
    virtual int callback(void *p) = 0;
    virtual ~Interface() {}
};
void run(Interface *i)
{
    int v = 0;
    i->callback(&v);
}