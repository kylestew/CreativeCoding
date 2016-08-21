/*== DAY 65 == [SUN MAR 13 2016] == */
/*
 * "Mandala 1"
 * ???
 */

int fCount = 230;

float pen0x = 0;
float pen0y = 0;
int pen0base = 6;
color pen0color = color(0);

float pen1x = 0;
float pen1y = 0;
int pen1base = 5;
color pen1color = color(255);

float speedy = 20;

void setup() {
  size(640, 640);
  frameRate(30);
  smooth();
  noFill();
  background(0, 100, 100);

  // draw image background
  PImage img = loadImage("backdrop.png");
  image(img, 0, 0);

  pen0x = width/2;
  pen0y = width/2;
  pen1x = width/2;
  pen1y = width/2;
}

void draw() {
  if (frameCount > fCount) {
    frameCount = 0;
    noLoop();
  }
  println(frameCount);

  pushMatrix();
  float x2 = pen1x + 2.2*sin(frameCount/(64.0-speedy));
  float y2 = pen1y + 1.5*cos(frameCount/(32.0-speedy));
  drawForm(pen1x, pen1y, x2, y2, pen1color, pen1base);
  pen1x = x2;
  pen1y = y2;
  popMatrix();

  pushMatrix();
  x2 = pen0x + 3.5*sin(frameCount/(48.0-speedy));
  y2 = pen0y + 2*cos(frameCount/(32.0-speedy));
  drawForm(pen0x, pen0y, x2, y2, pen0color, pen0base);
  pen0x = x2;
  pen0y = y2;
  popMatrix();

  speedy -= 0.1;




  // video
  saveFrame("output/frame########.png");
  // gif
  //if (frameCount % 1 == 0 && frameCount < 500) saveFrame("output/frame####.gif");
}

void drawForm(float x0, float y0, float x1, float y1, color col, int base) {
  stroke(col);

  lineavariable(x1, y1, x0, y0);
  lineavariable(((width/2) - x1) + width/2, y1, ((width/2) - x0) + width/2, y0);
  translate(width/2, height/2);
  for (int i = 1; i < base; i += 1) {
    rotate(TWO_PI/base);
    lineavariable(x1-(width/2), y1-(height/2), x0-(width/2), y0-(height/2));
    lineavariable(((width/2) - x1), y1-(height/2), ((width/2) - x0), y0-(height/2));
  }
}

void lineavariable(float x, float y, float px, float py) {
  float speed = abs(x-px) + abs(y-py);
  strokeWeight(speed*4);
  line(x, y, px, py);
}
