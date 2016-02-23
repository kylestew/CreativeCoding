
var NORTH = 0;
var NORTHEAST = 1;
var EAST = 2;
var SOUTHEAST = 3;
var SOUTH = 4;
var SOUTHWEST = 5;
var WEST = 6;
var NORTHWEST = 7;

// var steps = 64;
// var stepSize = 1;
// var diameter = 2;

var steps = 4;
var stepSize = 10;
var diameter = 8;
var counter = 0;

var direction;
var posX, posY;

function setup() {
  createCanvas(400, 400);

  colorMode(HSB, 360, 100, 100, 100);
  background(255);
  smooth();
  noStroke();

  posX = width/2;
  posY = height/2;
}

function draw() {
  for (var i = 0; i <= steps; i++) {
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
