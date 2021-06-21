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
2021-06-21T10:47:59.383 [INFO]  SimpleLog initialized with LOG_NAME='log_example' and VERBOSITY=1.
2021-06-21T10:47:59.383 [ERROR] Hello World 42
2021-06-21T10:47:59.383 [WARN]  Hello World 43
2021-06-21T10:47:59.383 [INFO]  Hello World 44
2021-06-21T10:47:59.383 [DEBUG] Hello World 45
2021-06-21T10:47:59.384 [DEBUG2] Hello World 2 46
```
