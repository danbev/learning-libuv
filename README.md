## Learning libuv
The sole purpose of this project is to help understand how libuv works.

### Installation

#### Clone libuv:

    $ git clone git@github.com:libuv/libuv.git
    $ cd libuv

Next create a copy libuv HEAD:

    $ git archive --prefix="libuv/" HEAD | tar -xvfC /path/to/learning-libuv

### Building

    $ make

## webserver
This program is a simple server that will print out what the clients sends it.

    $ ./webserver

And in a different terminal:

    $ telnet localhost 7777


