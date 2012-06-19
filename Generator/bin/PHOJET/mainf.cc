
extern"C" {
  void phomain_();
}
#define phomain phomain_

int main(int argc, char **argv){
   phomain();
   return 0;
}
