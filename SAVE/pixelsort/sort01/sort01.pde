/*== PIXEL SORTING == */
/*
 * Sort by highest RGB value
 * Melt ya face off!!!
 * https://github.com/jeffThompson/PixelSorting
 */
import java.util.Collections; 

PImage img;

void settings() {
  //img = loadImage("photo.jpg");
  img = loadImage("photo2.jpg");
  size(img.width, img.height);
  //smooth(8);
  //frameRate(30);
}

void setup() {
  color c;
  float r, g, b;
  ArrayList<Integer> rColor = new ArrayList<Integer>();
  ArrayList<Integer> rPos = new ArrayList<Integer>();
  ArrayList<Integer> gColor = new ArrayList<Integer>();
  ArrayList<Integer> gPos = new ArrayList<Integer>();
  ArrayList<Integer> bColor = new ArrayList<Integer>();
  ArrayList<Integer> bPos = new ArrayList<Integer>();

  image(img, 0, 0);
  
  loadPixels();
  for (int i = 0; i < pixels.length; i++) {
    // split color into component values
    c = pixels[i];
    r = c >> 16 & 0xFF;
    g = c >> 8 & 0xFF;
    b = c & 0xFF;
    
    // sort by highest into component arrays
    int highest = highestColor(r, g, b);
    switch (highest) {
    case 1:
      rColor.add(c);
      rPos.add(i);
      break;
    case 2:
      gColor.add(c);
      gPos.add(i);
      break;
    case 3:
      bColor.add(c);
      bPos.add(i);
      break;
    }
    
    if (i > 0 && i % (pixels.length/10) == 0) {
      println("  " + nfc(i) + "\t\t/ " + nfc(pixels.length));
    }
  }
  
  // let us know the statistics of our gathering!
  println("\nSTATISTICS:");
  println("  Red:   " + nfc(rColor.size()) + "px");
  println("  Green: " + nfc(gColor.size()) + "px");
  println("  Blue:  " + nfc(bColor.size()) + "px\n");
  
  // sort ArrayLists using Java Collections
  println("Sorting arrays (may take a while)...");
  print("  red...");
  Collections.sort(rColor);
  print(" green...");
  Collections.sort(gColor);
  print(" blue...\n");
  Collections.sort(bColor);
  
  
  // put pixels back in place in their new order
  for (int i = 0; i < rPos.size(); i++) {
    pixels[rPos.get(i)] = (color)rColor.get(i);
  }
  for (int i = 0; i < gPos.size(); i++) {
    pixels[gPos.get(i)] = (color)gColor.get(i);
  }
  for (int i = 0; i < bPos.size(); i++) {
    pixels[bPos.get(i)] = (color)bColor.get(i);
  }
  
  updatePixels();
  
}