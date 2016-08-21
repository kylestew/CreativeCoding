Forms forms;

void setup() {
  size(960, 960, P2D);
  smooth(8);
  colorMode(RGB, 255, 255, 255, 1);

  forms = new RandomForm();
  
  forms.draw();
  saveFrame("render.png");
  exit();
}