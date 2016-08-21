
var NORTH = 0;
var EAST = 1;
var SOUTH = 2;
var WEST = 3;

var posX, posY;
var posXcross, posYcross;

var direction = SOUTH;
var angleCount = 7
var angle = getRandomAngle(direction);
var steps = 4;
var stepSize = 3;
var minLength = 10;

function setup() {
  createCanvas(600, 600);
  colorMode(HSB, 360, 100, 100, 100);
  smooth();
  background(360);

  posX = int(random(0, width));
  posY = 5;
  posXcross = posX;
  posXcross = posY;
}

function draw() {
  for (var i = 0; i <= steps; i++) {


    posX += cos(radians(angle)) * stepSize;
    posY += sin(radians(angle)) * stepSize;

    boolean reachedBorder = false;

    if (posY <= 5) {
      direction = SOUTH;
      reachedBorder = true;
    }


    direction = round(noise(i*frameCount)*8);

    if (direction == NORTH) {
      posY -= stepSize;
    } else if (direction == NORTHEAST) {
      posX += stepSize;
      posY -= stepSize;
    } else if (direction == EAST) {
      posX += stepSize;
    }
    else if (direction == SOUTHEAST) {
      posX += stepSize;
      posY += stepSize;
    }
    else if (direction == SOUTH) {
      posY += stepSize;
    }
    else if (direction == SOUTHWEST) {
      posX -= stepSize;
      posY += stepSize;
    }
    else if (direction == WEST) {
      posX -= stepSize;
    }
    else if (direction == NORTHWEST) {
      posX -= stepSize;
      posY -= stepSize;
    }

    if (posX > width) posX = 0;
    if (posX < 0) posX = width;
    if (posY < 0) posY = height;
    if (posY > height) posY = 0;

    if (counter == 128) {
      counter = 0;
      fill(192, 100, 64, 80);
      ellipse(posX + stepSize/2, posY + stepSize/2, diameter*2, diameter*2);

    }
    counter++;

    fill(0, random(10, 40));
    ellipse(posX + stepSize/2, posY + stepSize/2, diameter, diameter);
  }
}


// void draw(){
//   for (int i=0; i<=mouseX; i++) {
//
//     // ------ draw dot at current position ------
//     if (!recordPDF) {
//       strokeWeight(1);
//       stroke(180);
//       point(posX, posY);
//     }
//
//     // ------ make step ------
//     posX += cos(radians(angle)) * stepSize;
//     posY += sin(radians(angle)) * stepSize;
//
//
//     // ------ check if agent is near one of the display borders ------
//     boolean reachedBorder = false;
//
//     if (posY <= 5) {
//       direction = SOUTH;
//       reachedBorder = true;
//     }
//     else if (posX >= width-5) {
//       direction = WEST;
//       reachedBorder = true;
//     }
//     else if (posY >= height-5) {
//       direction = NORTH;
//       reachedBorder = true;
//     }
//     else if (posX <= 5) {
//       direction = EAST;
//       reachedBorder = true;
//     }
//
//     // ------ if agent is crossing his path or border was reached ------
//     int px = (int) posX;
//     int py = (int) posY;
//     if (get(px, py) != color(360) || reachedBorder) {
//       angle = getRandomAngle(direction);
//       float distance = dist(posX, posY, posXcross, posYcross);
//       if (distance >= minLength) {
//         strokeWeight(3);
//         stroke(0);
//         line(posX, posY, posXcross, posYcross);
//       }
//       posXcross = posX;
//       posYcross = posY;
//     }
//   }
// }
//
// float getRandomAngle(int theDirection) {
//   float a = (floor(random(-angleCount, angleCount)) + 0.5) * 90.0/angleCount;
//
//   if (theDirection == NORTH) return (a - 90);
//   if (theDirection == EAST) return (a);
//   if (theDirection == SOUTH) return (a + 90);
//   if (theDirection == WEST) return (a + 180);
//   return 0;
// }
