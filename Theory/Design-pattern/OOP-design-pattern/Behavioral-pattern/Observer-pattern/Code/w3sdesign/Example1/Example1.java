
import java.util.ArrayList;
import java.util.List;
public abstract class Subject { 
    private List<Observer> observers = new ArrayList<Observer>();
    // Registration interface.
    public void attach(Observer o) { 
        observers.add(o);
    } 
      // Notification interface.
      // notify() is already used by the Java Language (to wake up threads).
      public void notifyObservers() { 
          for (Observer o : observers)
              o.update();
      } 
  } 


public class Subject1 extends Subject { 
    private int state = 0;
    //
    public int getState() { 
        return state;
    } 
    void setState(int state) { 
        this.state = state;
          System.out.println(
              "Subject1 : State changed to : " + state + 
              "\n           Notifying observers ...");
          // Notifying observers that state has changed.
          notifyObservers();
      } 
  } 


public abstract class Observer { 
    // Synchronizing observer's state with subject's state.
    public abstract void update();
} 


public class Observer1 extends Observer { 
    private int state;
    private Subject1 subject;
    public Observer1(Subject1 subject) { 
        this.subject = subject;
        // Registering this observer on subject.
        subject.attach(this); 
    } 
      public void update() { 
          this.state = subject.getState();
          System.out.println(
              "Observer1: State updated to : " + this.state);
      } 
  } 


public class Observer2 extends Observer { 
    private int state;
    private Subject1 subject;
    public Observer2(Subject1 subject) { 
        this.subject = subject;
        // Registering this observer on subject.
        subject.attach(this); 
    } 
      public void update() { 
          this.state = subject.getState();
          System.out.println(
              "Observer2: State updated to : " + this.state);
      } 
  }

  public class MyApp { 
    public static void main(String[] args) { 
        Subject1 s1 = new Subject1();
        // Creating observers and registering them on subject1.
        Observer o1 = new Observer1(s1);
        Observer o2 = new Observer2(s1);
          
        System.out.println("Changing state of Subject1 ...");
          s1.setState(100);
      } 
  }