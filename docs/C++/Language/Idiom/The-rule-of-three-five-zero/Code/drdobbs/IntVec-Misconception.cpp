// This class contains a subtle error
class IntVec
{
public:
    IntVec(int n) : data(new int[n]) {}
    ~IntVec() { delete[] data; };
    int &operator[](int n)
    {
        return data[n];
    }
    const int &operator[](int n) const
    {
        return data[n];
    }

private:
    int *data;
};

int main()
{
    IntVec x(100);
    IntVec y = x; // Trouble!
    return 0;
}