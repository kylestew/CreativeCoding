
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

// CONTROLS
var clearGenerations = true;
var clearColor = 16;

var lSystem;
var renderSize;

function setup() {
  createCanvas(600, 600);
  frameRate(30);
  strokeWeight(1);
  noFill();
  background(clearColor);

  lSystem = LSystem.KOCH_ISLAND();
}

function draw() {

  // slowly fade old
  if (!clearGenerations) {
    fill(0,0,0,1);
    rect(0,0,width,height);
    noFill();
  }

  if (!renderSize) {
    renderSize = lSystem.determineSizeInfo();
  }
  $('#generation-text').text("generation: " + lSystem.generation + " :: points: " + renderSize.points);

  // default scale by width
  var _scale = width / renderSize.width;
  if (renderSize.height*_scale > height) {
    // if height is bounding, scale by that instead
    _scale = height / renderSize.height;
  }

  // center scale by 80% to not draw on edge
  translate(width/2.0, height/2.0);
  scale(0.8, 0.8);
  translate(-width/2.0, -height/2.0);
  // scale to draw l-system in view
  scale(_scale, _scale);
  // translate to starting point
  translate(-renderSize.x, -renderSize.y);
  // strokes scale on p5 for some reason
  strokeWeight(2/_scale);
  // stroke(255, 255, 255, 255);
  stroke(255, 128, 255);

  lSystem.renderStep(renderSize.points/(1*30));

  // move to next generation?
  if (lSystem.renderComplete) {
    noLoop();
    if (lSystem.generation >= 5) {
      return; // too many points to handle
    }

    $('#generation-text').text("generating generation " + (lSystem.generation + 1));

    lSystem.iterate(1);
    renderSize = lSystem.determineSizeInfo();
    setTimeout(function() {
      if (clearGenerations)
        background(clearColor);
      loop(); // start again
    }, 1200);

    return;
  }
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
  this.renderComplete = false;
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

  // reset render commands
  this.turtle = createVector(0, 0);
  this.index = 0;
  this.angle = 0;
  this.renderComplete = false;
};
LSystem.prototype.determineSizeInfo = function() {
  var turtle = createVector(0, 0);
  var angle = 0;
  var x0, y0, x1, y1, points;
  x0 = y0 = x1 = y1 = points = 0;
  for (var i = 0; i < this.production.length; i++) {
    switch(this.production.charAt(i)) {
      case '+':
        angle += this.theta;
        break;
      case '-':
        angle -= this.theta;
        break;
      default:
        points++;
        turtle.x += cos(angle);
        turtle.y += sin(angle);
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
  if (this.index == this.production.length)
    this.renderComplete = true;
};
