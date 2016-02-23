
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


  // Pentigree
  // lSystem = new LSystem({
  //   axiom: "F-F-F-F",
  //   rules: {
  //     "F": "F-F+F+FF-F-F+F"
  //   },
  //   theta: radians(90)
  // })
  // lSystem.iterate(3);

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

var lSystem;
var step, skip;

function setup() {
  createCanvas(600, 600);
  frameRate(30);
  stroke(255);
  strokeWeight(1);
  noFill();

  lSystem = LSystem.KOCH_0();
  lSystem.iterate(4);
  size = lSystem.determineSizeInfo();

  console.log(size);

  background(0);
}

var size;
function draw() {
  $('#generation-text').text("generation: " + lSystem.generation + " :: points: " + size.points);

  // default scale by width
  var _scale = width / size.width;
  if (size.height*_scale > height) {
    // if height is bounding, scale by that instead
    _scale = height / size.height;
  }
  _scale *= 0.8; // don't draw to the edge

  // center canvas
  translate((width-size.width*_scale)/2, (height-size.height*_scale)/2);
  scale(_scale, _scale);
  translate(-size.x, -size.x);
  translate(-100, -12);

  strokeWeight(1/_scale);
  if (lSystem.renderComplete)
    noLoop();
  lSystem.renderStep(lSystem.generation * 32);
}

// LSystem.KOCH_ISLAND = function() {
//   return new LSystem({
//     axiom: "F",
//     rules: {
//       "A": "-F+AA++A+F--F-A",
//       "F": "F+A++A-F--FF-A+"
//     },
//     theta: radians(60)
//   });
// }


var LSystem = function LSystem(props) {
  this.axiom = props.axiom;
  this.rules = props.rules;
  this.theta = props.theta;

  this.production = this.axiom;
  this.generation = 0;

  this.turtle = createVector(0, 0);
  this.index = 0;
  this.angle = 0;
}

LSystem.KOCH_ISLAND = function() {
  return new LSystem({
    axiom: "F-F-F-F",
    rules: {
      "F": "F-F+F+FF-F-F+F",
    },
    theta: radians(90)
  });
}
LSystem.KOCH_ISLAND_2 = function() {
  return new LSystem({
    axiom: "-F",
    rules: {
      "F": "F+F-F-F+F",
    },
    theta: radians(90)
  });
}
LSystem.KOCH_LAKES = function() {
  return new LSystem({
    axiom: "F+F+F+F",
    rules: {
      "F": "F+f-FF+F+FF+Ff+FF-f+FF-F-FF-Ff-FFF",
      "f": "ffffff"
    },
    theta: radians(90)
  });
}
LSystem.KOCH_0 = function() {
  return new LSystem({
    axiom: "F-F-F-F",
    rules: {
      "F": "FF-F-F-F-F-F+F",
    },
    theta: radians(90)
  });
}

LSystem.prototype.iterate = function(iterations) {
  iterations = typeof iterations !== 'undefined' ? iterations : 1;
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
LSystem.prototype.determineSizeInfo = function() {
  var turtle = createVector(0, 0);
  var x0, y0, x1, y1, points;
  x0 = y0 = x1 = y1 = points = 0;
  for (var i = 0; i < this.production.length; i++) {
    switch(this.production.charAt(i)) {
      case '+':
        this.angle += this.theta;
        break;
      case '-':
        this.angle -= this.theta;
        break;
      default:
        points++;
        turtle.x += cos(this.angle);
        turtle.y += sin(this.angle);
        if (turtle.x < x0) x0 = turtle.x;
        if (turtle.y < y0) y0 = turtle.y;
        if (turtle.x > x1) x1 = turtle.x;
        if (turtle.y > y1) y1 = turtle.y;
        break;
    }
  }

  return {
    x: x0,
    y: y0,
    width: x1-x0,
    height: y1-y0,
    points: points
  }
};
LSystem.prototype.renderStep = function(steps) {
  for (; this.index < this.production.length && steps > 0; this.index++) {
    steps--;
    switch(this.production.charAt(this.index)) {
      case 'f': // skip
        this.turtle.x += cos(this.angle);
        this.turtle.y += sin(this.angle);
        break;
      case '+':
        this.angle += this.theta;
        break;
      case '-':
        this.angle -= this.theta;
        break;
      default:
        var x = this.turtle.x + cos(this.angle);
        var y = this.turtle.y + sin(this.angle);
        line(this.turtle.x, this.turtle.y, x, y);
        this.turtle.x = x;
        this.turtle.y = y;
        break;
    }
  }
};
