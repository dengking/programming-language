package com.sample.observer.gui;
import java.awt.event.*;
import javax.swing.*;
public class GUIDemo2 extends JPanel
    implements ActionListener { 
    JButton button1;
    JTextArea textArea1;
    public GUIDemo2() { 
        button1 = new JButton("Button1");
          add(button1);
          textArea1 = new JTextArea("TextArea1", 10, 20);
          add(textArea1);
          // Registering this object/ActionListener on button1
          // (for being notified when an action event occurs).
          button1.addActionListener(this);
      } 
      // Implementing the ActionListener interface.
      public void actionPerformed(ActionEvent e) { 
          if (e.getSource() == button1) { 
              textArea1.append("\nNotification from Button1: \n  " +
                  "User clicked the Button1.");
          } 
      } 
      private static void createAndShowGUI() { 
          // Creating the GUI.
          JFrame frame = new JFrame("GUIDemo2");
          frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
          JComponent contentPane = new GUIDemo2();
          frame.setContentPane(contentPane);
          // Showing the GUI.
          frame.pack();
          frame.setVisible(true);
      } 
      public static void main(String[] args) { 
          // For thread safety, invoked from event-dispatching thread.
          javax.swing.SwingUtilities.invokeLater(new Runnable() { 
              public void run() { 
                  createAndShowGUI();
              } 
          } );
      } 
  } 


package com.sample.observer.gui;
import java.awt.event.*;
import javax.swing.*;
public class GUIDemo2 extends JPanel
    implements ActionListener { 
    JButton button1;
    JTextArea textArea1;
    public GUIDemo2() { 
        button1 = new JButton("Button1");
          add(button1);
          textArea1 = new JTextArea("TextArea1", 10, 20);
          add(textArea1);
          // Registering this object/ActionListener on button1
          // (for being notified when an action event occurs).
          button1.addActionListener(this);
      } 
      // Implementing the ActionListener interface.
      public void actionPerformed(ActionEvent e) { 
          if (e.getSource() == button1) { 
              textArea1.append("\nNotification from Button1: \n  " +
                  "User clicked the Button1.");
          } 
      } 
      private static void createAndShowGUI() { 
          // Creating the GUI.
          JFrame frame = new JFrame("GUIDemo2");
          frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
          JComponent contentPane = new GUIDemo2();
          frame.setContentPane(contentPane);
          // Showing the GUI.
          frame.pack();
          frame.setVisible(true);
      } 
      public static void main(String[] args) { 
          // For thread safety, invoked from event-dispatching thread.
          javax.swing.SwingUtilities.invokeLater(new Runnable() { 
              public void run() { 
                  createAndShowGUI();
              } 
          } );
      } 
  }