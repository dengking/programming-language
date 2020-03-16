package com.sample.observer.timer;
import java.util.Calendar;
public class Client { 
    public static void main(String[] args) throws InterruptedException { 
        Timer timer = new Timer(); // subject
        // Creating a clock (observer) and registering it on timer (subject).
        Clock clock = new Clock(timer);
        final Calendar calendar = Calendar.getInstance();
        for (int i = 0; i < 3; i++) { 
              Thread.sleep(1000); // one second
              calendar.setTimeInMillis(System.currentTimeMillis());
              int h = calendar.get(Calendar.HOUR_OF_DAY);
              int m = calendar.get(Calendar.MINUTE);
              int s = calendar.get(Calendar.SECOND);
              // Changing timer's state every second.
              timer.tick(h, m, s);
          } 
      } 
  }

package com.sample.observer.timer;
import java.util.ArrayList;
import java.util.List;
public abstract class Subject { 
    private List<Observer> observers = new ArrayList<Observer>();
    // Registration interface.
    public void attach(Observer o) { 
        observers.add(o);
    } 
      // Notification interface.
      public void notifyObservers() { 
          for (Observer o : observers)
              o.update(this);
      } 
  } 

package com.sample.observer.timer;
public class Timer extends Subject { 
    private int hour = 0;
    private int minute = 0;
    private int second = 0;
    public int getHour() { 
        return hour;
    } 
    public int getMinute() { 
          return minute;
      } 
      public int getSecond() { 
          return second;
      } 
      // Changing time of day and notifying observers.
      public void tick(int hour, int minute, int second) { 
          System.out.printf(
              "Timer  : Time of day changed to  :  %02d:%02d:%02d %n",
              hour, minute, second);
          this.hour = hour;
          this.minute = minute;
          this.second = second;
          // Notifying observers that time has changed.
          notifyObservers();
      } 
  } 

package com.sample.observer.timer;
public abstract class Observer { 
    public abstract void update(Subject s);
} 

package com.sample.observer.timer;
public class Clock extends Observer { 
    private Timer subject;
    public Clock(Timer subject) { 
        this.subject = subject;
        // Registering this clock on subject.
        subject.attach(this);
    } 
    public void update(Subject s) { 
          if (this.subject == s) { 
              System.out.printf(
                  "Clock  : Updated/Synchronized to :  %02d:%02d:%02d %n",
                  subject.getHour(), subject.getMinute(), subject.getSecond());
          } 
      } 
  } 