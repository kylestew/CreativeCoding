import oscP5.*;
import netP5.*;

OscP5 oscP5;
NetAddress dest;

PFont f;

void setup() {
  f = createFont("Courier", 16);
  textFont(f);
  
  size(640, 480, P2D);
  noStroke();
  smooth();
  
  oscP5 = new OscP5(this, 9000);
  dest = new NetAddress("127.0.0.1", 12000);
}

void draw() {
  background(0);
  fill(255);
  ellipse(mouseX, mouseY, 10, 10);
  
  if (frameCount % 2 == 0) {
    sendOsc();
  }
  
  text("x=" + mouseX + ", y=" + mouseY, 10, 80);
}

void sendOsc() {
  OscMessage msg = new OscMessage("note");
  msg.add((int)48+mouseX/10);
  msg.add((float)mouseY/height);
  oscP5.send(msg, dest);
}