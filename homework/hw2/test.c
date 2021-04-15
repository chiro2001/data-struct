typedef struct {
  int a, b;
  char c;
} A;

typedef struct {
  int a, b;
  int d;
} B;

typedef union {
  A a;
  B b;
} Test;

int main() {
  A a;
  B b;
  Test t;
  t.a = a;
  t.b = b;
  // t.a.a == t.b.a
  B *bb = (B *)&a;
  // bb->a == a.a
}