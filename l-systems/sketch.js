
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

var lSystem;
var step, skip;

function setup() {
  createCanvas(600, 600);
  frameRate(30);
  stroke(255);
  noFill();

  // ???
  // lSystem = new LSystem({
  //   axiom: "F",
  //   rules: {
  //     "F": "+F--F+"
  //   },
  //   theta: radians(45)
  // })
  // lSystem.iterate(12);

  // ???
  // lSystem = new LSystem({
  //   axiom: "A",
  //   rules: {
  //     "B": "A+B+A",
  //     "A": "B-A-B"
  //   },
  //   theta: radians(60)
  // })
  // lSystem.iterate(8);

  // ???
  lSystem = new LSystem({
    axiom: "F",
    rules: {
      "A": "-F+AA++A+F--F-A",
      "F": "F+A++A-F--FF-A+"
    },
    theta: radians(60)
  })
  lSystem.iterate(5);

  // Pentigree
  // lSystem = new LSystem({
  //   axiom: "F-F-F-F",
  //   rules: {
  //     "F": "F-F+F+FF-F-F+F"
  //   },
  //   theta: radians(90)
  // })
  // lSystem.iterate(3);

  // try to center
  background(1);
  translate(width/2, height/3);
  step = 3;
  skip = 256;
  $('#generation-text').text("generation: " + lSystem.generation);
}

function draw() {
  if (lSystem.renderComplete)
    noLoop();
  lSystem.renderStep(skip, step);
}

var LSystem = function LSystem(props) {
  this.axiom = props.axiom;
  this.rules = props.rules;
  this.theta = props.theta;

  this.production = this.axiom;
  this.generation = 0;

  this.turtle = createVector(0, 0);
  this.angle = 0;
  this.renderComplete = false;
}
LSystem.prototype.iterate = function(iterations) {
  iterations = iterations || 1;
  // console.time("iterate");

  for (var itr = 0; itr < iterations; itr++) {
    var curProd = this.production;
    var nextProd = "";
    for (var i = 0; i < curProd.length; i++) {
      var symbol = curProd.charAt(i);
      var replacement = this.rules[symbol];
      nextProd += replacement || symbol;
    }
    this.production = nextProd;
    this.generation++;
  }

  // console.log("production length: " + this.production.length);
  // console.timeEnd("iterate");
};
LSystem.prototype.renderStep = function(steps, length) {
  for (var i = 0; i < steps; i++) {
    switch(this.production.charAt(i)) {
      case 'f': // skip
        this.turtle.x += length*cos(this.angle);
        this.turtle.y += length*sin(this.angle);
        break;
      case '+':
        this.angle += this.theta;
        break;
      case '-':
        this.angle -= this.theta;
        break;
      default:
        var x = this.turtle.x + length*cos(this.angle);
        var y = this.turtle.y + length*sin(this.angle);
        line(this.turtle.x, this.turtle.y, x, y);
        this.turtle.x = x;
        this.turtle.y = y;
        break;
    }
  }

  this.production = this.production.substring(steps);
  if (this.production == "")
    this.renderComplete = true;
};
