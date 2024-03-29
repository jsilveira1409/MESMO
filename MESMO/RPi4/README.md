# RPi4 Application

This deployment was auto-generated by the F' utility tool.

## Building and Running the RPi4 Application

In order to build the RPi4 application, or any other F´ application, we first need to generate a build directory. This can be done with the following commands:

```
cd RPi4
fprime-util generate
```

The next step is to build the RPi4 application's code.
```
fprime-util build
```

## Running the application and F' GDS

The following command will spin up the F' GDS as well as run the application binary and the components necessary for the GDS and application to communicate.

```
cd RPi4
fprime-gds
```

To run the ground system without starting the RPi4 app:
```
cd RPi4
fprime-gds --no-app
```

The application binary may then be run independently from the created 'bin' directory.

```
cd RPi4/build-artifacts/<platform>/bin/
./RPi4 -a 127.0.0.1 -p 50000
```
