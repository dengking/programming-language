using System;
namespace Outer
{
    public class OuterClass
    {

    }
    namespace Inner
    {
        public class InnerClass
        {
            private OuterClass memVar;
            public SetMem(OuterClass mem)
            {
                memVar = mem;
            }
        }
    }
}


public class MainClass
{
    public static void Main(String[] args)
    {
        Console.WriteLine("begin main");
        Outer.OuterClass o = new Outer.OuterClass();
        Outer.Inner.InnerClass i = new Outer.Inner.InnerClass();
        i.SetMem(o);
        Console.WriteLine("end main");
    }
}