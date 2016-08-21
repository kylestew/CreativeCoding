
public class RandomForm extends Forms {

  Form generateForm() {
    Form f1 = new Form(width, height, width/2, height/2);
    // blur
    if (random(1) < 0.6)
      f1.blur = random(8);
    // base rotation
    f1.baseRotation = (int)random(8) * 45;
    // blend mode (sparse)
    if (random(2) < 0.15) {
      println("f1: subtract");
      f1.blendmode = SUBTRACT;
    } else if (random(2) < 0.15) {
      println("f1: exclusion");
      f1.blendmode = EXCLUSION;
    }
    // pick # generations
    f1.genStart = (int)random(2);
    f1.genEnd = f1.genStart + (int)random(2) + 1;
    println("f1:: genStart: " + f1.genStart + " // genEnd: " + f1.genEnd);
    // directed generations?
    if (random(1) < 0.2) {
      f1.directedGen = false;
      f1.showShapes = false; // don't show shapes - super noisy
      println("f1:: disable directed generations");
    }
    // scale generations
    f1.scaleGeneration = 0.5*random(1) + 0.8;
    // offset generations
    f1.offsetGeneration = 0.5*random(1) + 0.5;
    println("f1:: scaleGen: " + f1.scaleGeneration + " // offsetGen: " + f1.offsetGeneration);
    // shapes
    f1.shapeCount = (int)random(9)+3; // at least 3, at most 12
    // too many shapes?
    if (f1.shapeCount > 8 && f1.genEnd - f1.genStart > 1) {
      f1.genEnd--;
    }
    println("f1:: shapeCount: " + f1.shapeCount);
    if (random(1) < 0.4) {
      // drop circular shapes
      f1.shapeSides = (int)random(12);
    }
    // shape radius
    f1.shapeRadius = 0.5*random(1)+0.05;
    // shape offset
    f1.shapeOffset = 0.1*random(1)+0.1;
    println("f1:: rad: " + f1.shapeRadius + " // off: " + f1.shapeOffset);
    f1.shapeRotation = (int)random(8)*45;
    // randomly hide shapes
    if (random(1) < 0.25) {
      f1.showShapes = false;
      println("f1:: HIDE SHAPES");
    }
    f1.shapeWeight = 6.0*random(1)+0.5;
    f1.shapeOpacity = 0.5*random(1) + 0.5;
    // shape color
    f1.shapeColor = darkBack ? color(random(40)+240) : color(random(40));
    // shape fill
    f1.shapeFillColor = darkBack ? color(255) : color(0);
    f1.shapeFillOpacity = 0.2*random(1);
    if (f1.shapeCount >= 6 || f1.generationCount() >= 2) {
      // pull down opacity when too many shapes
      f1.shapeFillOpacity /= 2.0;
    }
    println("f1:: shapeWeight: " + f1.shapeWeight + " // opa: " + f1.shapeOpacity + " // fill opa: " + f1.shapeFillOpacity);
    // show centers?
    if (random(1) < 0.4) {
      // trackCenters - pick generations to show
      f1.trackCenters = new boolean[f1.genEnd+1];
      for (int i = 0; i < f1.trackCenters.length; i++) {
        f1.trackCenters[i] = random(1) < 0.6;
      }
      // centers style
      f1.showCenters = true;
      // invert color
      f1.centerStrokeColor = darkBack ? color(random(40)+240) : color(random(40));
      // randomly choose to match color
      if (random(1) < 0.2)
        f1.centerStrokeColor = f1.shapeColor;
      f1.centerStrokeWeight = random(12) + 8;
      f1.centerStrokeOpacity = 0.5*random(1) + 0.5;
      println("f1:: SHOW CENTERS weight: " + f1.centerStrokeWeight + " // opa: " + f1.centerStrokeOpacity);
    }
    // show network?
    if (random(1) < 0.4 || !f1.showShapes) {
      f1.showNetwork = true;
      f1.networkStrokeColor = darkBack ? color(random(40)+240) : color(random(40));
      f1.networkStrokeWeight = random(8) + 0.2;
      // thin out lines if close together
      f1.networkStrokeWeight *= f1.shapeRadius;
      f1.networkStrokeWeight = max(f1.networkStrokeWeight, 1.0);
      println("f1:: SHOW NETWORK weight: " + f1.networkStrokeWeight + " // opa: " + f1.networkStrokeOpacity);
    }

    return f1;
  }

  public RandomForm() {
    super();

    // 1: Decide dark/light background
    darkBack = random(1) < 0.5;
    println(darkBack ? "going dark" : "going light");

    // 2: Pick a random backdrop based on darkness choice
    PImage background;
    if (darkBack == true) {
      float rand = random(10);
      back = loadImage("dark_0" + (int)rand + ".jpg");
    } else {
      float rand = random(10);
      back = loadImage("light_0" + (int)rand + ".jpg");
    }

    // 3: Random filter on background
    if (random(1) < 0.25) {
      backFilter = INVERT;
      darkBack = !darkBack;
      println("invert: " + backFilter);
    } else if (random(1) < 0.05) {
      println("posterize");
      backFilter = POSTERIZE;
      backFilterAmt = 3;
    } else if (random(1) < 0.2) {
      println("blur");
      backFilter = BLUR;
      backFilterAmt = 4;
    } else if (random(1) < 0.05) {
      println("dilate");
      backFilter = DILATE;
    } else if (random(1) < 0.2) {
      println("gray");
      backFilter = GRAY;
    }

    // 4: Optional circle container
    boolean showingCircle = false;
    if (random(1) < 0.3) {
      Form form = new Form(width, height, width/2, height/2);
      form.genStart = 0;
      form.genEnd = 0;
      form.shapeCount = 1;
      form.offsetGeneration = 0;
      form.scaleGeneration = 1.04;
      form.shapeRadius = random(1)*0.5+0.3;
      form.shapeWeight = random(12.0)+2.0;
      form.shapeOpacity = 1.0;

      form.shapeColor = darkBack ? color(random(40)+240) : color(random(40));
      form.shapeFillOpacity = random(1)*0.3;
      form.shapeFillColor = !darkBack ? color(random(40)+240) : color(random(40));

      form.blur = random(4);
      forms.add(form);    
      println("SHOW CIRCLE CONTAINER");

      showingCircle = true;
    }

    // 5: Generate form until desired non-suckiness reached
    float suck = 1000;
    Form f1;
    do {
      f1 = generateForm();
      
      suck = showingCircle ? 1.5 : 0;
      suck += 30.*abs(f1.shapeRadius - f1.shapeOffset);
      suck += 20.*abs(f1.scaleGeneration - f1.offsetGeneration);
      suck += 0.5 * f1.shapeCount; // more shapes tend towards mess
      if (f1.showCenters && f1.showNetwork && f1.showShapes) {
        suck += f1.shapeCount;
      }
      if (f1.generationCount() <= 1)
        suck += 4.0;
      if (f1.shapeOffset > f1.shapeRadius) {
        // shapes don't overlap - simple design
        if (!f1.showNetwork)
          suck *= 2.0;
      }
      // directed generations multiply complexity like crazy
      if (!f1.directedGen) {
        if (f1.showNetwork)
          suck *= 1.4;
        else
          suck *= 2.8;
      }
      if (!f1.showShapes && (f1.genCentersCount() <= 1)) {
        println("fail");
        // count generational centers - if only 1 then form will be empty
        suck += 100.0; // fail
      }
      println("suck score: " + suck);
    } while(suck > 20);

    forms.add(f1);
  }
}