
// return integer [1, 2, 3] for which color is the highest value
int highestColor(float r, float g, float b) {
  int highest = 0;
  
  float minVal = -1;
  if (r > minVal) {
    highest = 1;
    minVal = r;
  }
  if (g > minVal) {
    highest = 2;
    minVal = g;
  }
  if (b > minVal) {
    highest = 3;
    minVal = b;
  }
  
  return highest;
}