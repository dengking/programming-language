package com.sample.observer.basic;
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