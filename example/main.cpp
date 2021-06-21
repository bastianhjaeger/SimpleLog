#include <SimpleLog/log.h>

int main(int argc, char** argv) {
  log_init("log_example", DEBUG, true, true, true);

  int16_t i {42};
  
  LOG(ERROR, "Hello World " << i++)
  LOG(WARN,  "Hello World " << i++)
  LOG(INFO,  "Hello World " << i++)
  LOG(DEBUG, "Hello World " << i++)
  
  LOG(DEBUG2, "Hello World 1 " << i++) 
  log_verbosity(DEBUG2);
  LOG(DEBUG2, "Hello World 2 " << i++)

  return 0;
}

