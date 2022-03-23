int GlobalFeature = 1;

void init(int f) {
  GlobalFeature = f;
}

int getFeature() {
  return GlobalFeature;
}

int main() {
  init(42);
  return getFeature();
}
