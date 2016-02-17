
// koch
// var axiom = "F-F-F-F";
// var rules = {
//   "F" : "F-F+F+FF-F-F+F",
// };

// quadratic koch
// var axiom = "-F";
// var rules = {
//   "F" : "F+F-F-F+F",
// };

// ???
// var axiom = "F+F+F+F";
// var rules = {
//   "F" : "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF",
//   "f" : "ffffff"
// };

// ???
var axiom = "F-F-F-F";
var rules = {
  "F" : "FF-F-F-F-FF",
};

var step = 64;
var angleInc = 90;

var lString;

function setup() {
  createCanvas(640, 640);
  frameRate(1);
  stroke(255);
  noFill();

  lString = axiom;
}

var iterateLString = function(lString, rules) {
  return _.map(lString.split(""), function(symbol) {
    var replacement = _.find(rules, function(value, key) {
      return symbol == key;
    });
    return replacement || symbol;
  }).join("");
}

var parseLStringToVertices = function(lString, step, angleInc) {
  var pos = createVector(0, 0);
  var angle = 0;

  var vertices = [];
  vertices.push([pos.x, pos.y]);
  _.each(lString.split(""), function(command) {
    switch(command) {
      case 'F':
        pos.x += step*cos(radians(angle));
        pos.y += step*sin(radians(angle));
        vertices.push([pos.x, pos.y]);
        break;
      case 'f':
        pos.x += step*cos(radians(angle));
        pos.y += step*sin(radians(angle));
        break;
      case '+':
        angle += 90;
        break;
      case '-':
        angle -= 90;
        break;
    }
  });

  return vertices;
}

function draw() {
  background(0);

  lString = iterateLString(lString, rules);
  var vertices = parseLStringToVertices(lString, step/(9*(frameCount)), angleInc);

  // TODO: auto center based on vertices created
  translate(200, 450);

  beginShape();
  _.each(vertices, function(v) {
    vertex(v[0], v[1]);
  });
  endShape(CLOSE);

  if (frameCount > 5)
    noLoop();
}
