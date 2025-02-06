# sensor_value_generator
Code Explanation :

1) First we need to create a timer which triggers every 1 sec. For this we used setitimer() which allows us to set a timer that generates SIGALRM signal after a specified time and repeats at fixed intervals.

2) Now we have to catch SIGALRM signal and handle it for our custom use. For this sigaction() is used, where we register our custom handler to this signal.

3) In handler function, first we generate random count of byes, then stored that bytes into a global buffer. Mutex lock is used as same buffer is used in main thread also.

4) In main thread, we continous check if required number of bytes are stored in buffer or not, if yes then we print the lastest bytes and delete it from buffer.
