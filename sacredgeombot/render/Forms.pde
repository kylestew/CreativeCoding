public class Forms {
  
  public boolean darkBack = true;
  
  public PImage back;
  public int backFilter = -1;
  public float backFilterAmt = -1;
  
  public PImage post;
  public float postTintAlpha = 0.5;
  public ArrayList<Form> forms = new ArrayList<Form>();

  public void draw() {
    // background
    if (back != null) {
      // draw
      image(back, 0, 0, width, height);
      
      // filter
      if (backFilter > -1) {
        if (backFilterAmt > -1)
          filter(backFilter, backFilterAmt);
        else
          filter(backFilter);
      }
      
      // knock up / down image so overlay will show better
      PImage knock;
      if (darkBack) {
        // darken further
        knock = loadImage("down.png");
      } else {
        // brighten further
        knock = loadImage("up.png");
      }
      image(knock, 0, 0, width, height);
    } else {
      background(0);
    }
    
    // draw forms
    for (Form form : forms) {
      blendMode(form.blendmode);
      image(form.draw(darkBack), 0, 0);
    }
  }
}