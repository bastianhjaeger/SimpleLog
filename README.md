# SimpleLog

This is a logging header only "library" for you c++ usage. 

Features: 

- Singleton implementation for "global" usage
- Thread save.
- Colorized log to console
- Log to console and/or file
- No initialization necessary (for console logging only)
- Logging with comfortable stream syntax `LOG(ERROR, "Hello " << "World")`

## build example

```
cd example
mkdir build && cd build

cmake ..
make -j
```

## run example

```
./SimpleLogExample
```

Output should look like:

```
user@ubuntu:~/SimpleLog/example/build $ ./SimpleLogExample
2021-06-21 10:06:17 [INFO]  SimpleLog initialized with LOG_NAME='log_example' and VERBOSITY=1.
2021-06-21 10:06:17 [ERROR] Hello World
2021-06-21 10:06:17 [WARN]  Hello World
2021-06-21 10:06:17 [INFO]  Hello World
2021-06-21 10:06:17 [DEBUG] Hello World
2021-06-21 10:06:17 [DEBUG2] Hello World

```