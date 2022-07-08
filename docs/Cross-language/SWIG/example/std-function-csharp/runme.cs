using System;

public class runme
{
  static void Main() 
  {
    Console.WriteLine("Adding and calling a normal C++ callback");
    Console.WriteLine("----------------------------------------");

    Caller caller = new Caller();
    CSharpCallbackImpl callbackImpl = new CSharpCallbackImpl();
    SwigCallback callback = new SwigCallback(callbackImpl);

      caller.setCallback(callback);
      caller.call();
      caller.resetCallback();

    Console.WriteLine();
    Console.WriteLine("C# exit");
    Console.ReadLine();
  }
}

public class CSharpCallbackImpl : SwigCallbackImpl
{
  public CSharpCallbackImpl()
    : base()
  {
  }

  protected override void run(ParamStruct arg0)
  {
    Console.WriteLine(arg0.a_);
    Console.WriteLine(arg0.b_);
  }
}

