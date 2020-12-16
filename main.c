#include <Keyboard.h>

int state[] = {0, 0, 0};
int key[] = {7, 8, 9};
int buffer[255] = {};
int buffer_i = 0;
int cur_value = 0;
int cur_i = 0;

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  
  for (int i = 0; i < 255; i++) {
    buffer[i] = 0;
  }
  for (int i = 0; i < 3; i++) {
    pinMode(key[i], INPUT_PULLUP);
  }
}

void add(int v) {
  cur_value = (cur_value << 1) | v;
  cur_i += 1;
  if (cur_i == 8) {
    if (buffer_i + 1 == 255) {
      for (int i = 0; i < 254; i++) {
        buffer[i] = buffer[i + 1];
      }
      buffer[buffer_i] = cur_value;
    } else {
      buffer[buffer_i] = cur_value;
      buffer[buffer_i + 1] = 0;
      buffer_i++;
    }
    
    cur_value = 0;
    cur_i = 0;
  }
}

void press_0() {
  add(0);
}

void press_1() {
  add(1);
}

void write() {
  int i = 0;
  while (buffer[i] && i < 255) {
    Keyboard.write(buffer[i]);
    i++;
  }
  if (cur_value) {
    Keyboard.write(cur_value);
  }
  for (int x = 0; x < i; x++) {
    buffer[x] = 0;
  }
  buffer_i = 0;
  cur_value = 0;
  cur_i = 0;
}

void loop() {
  for (int i = 0; i < 3; i++) {
    int bs = digitalRead(key[i]);
    if (bs == LOW) {
      if (state[i] != 1) {
        if (i == 0) {
          press_0();
        } else if (i == 1) {
          press_1();
        } else {
          write();
        }
      }
      state[i] = 1;
    } else {
      state[i] = 0;
    }
    delay(10);
  }
}