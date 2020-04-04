package com.sample.observer.basic;
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