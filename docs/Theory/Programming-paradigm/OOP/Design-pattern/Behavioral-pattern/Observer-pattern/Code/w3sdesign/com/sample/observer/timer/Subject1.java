package com.sample.observer.basic;
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