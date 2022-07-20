using System;
class NimApiGlobalPINVOKE
{
    protected class SWIGExceptionHelper
    {
        static SWIGExceptionHelper()
        {
            Console.WriteLine("SWIGExceptionHelper static constructor");
        }
    }

    protected static SWIGExceptionHelper swigExceptionHelper = new SWIGExceptionHelper();

    static NimApiGlobalPINVOKE()
    {
        Console.WriteLine("NimApiGlobalPINVOKE static constructor");
    }
    public static void Test()
    {
        Console.WriteLine("test");
    }
}

public class MainClass
{
    public static void Main(String[] args)
    {
        Console.WriteLine("begin main");
        NimApiGlobalPINVOKE.Test();
        NimApiGlobalPINVOKE.Test();
        NimApiGlobalPINVOKE.Test();
        Console.WriteLine("end main");
    }
}