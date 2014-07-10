fbfs
====
The Facebook File System provides an interface to Facebook in the form of a FUSE
implementation. This allows for convenient access to Facebook through the
Graph API with traditional Unix utilities. This code is not yet production
ready (or even alpha). Read the top comment in `fbfs.c` to find out why.
Keep in mind that this was a hackathon project with no contributions past that.

# Building

You will need the `libcurl`, `json-c`, and `fuse` libraries to compile and run
`fbfs`. You will need ruby with sinatra to run the OAuth webserver.

    $ make

# Preparation

First, spin up the webserver and get the auth token.

    $ ruby webserver/app.rb
    # get the access token from console logs
    $ mkdir ~/.fbfs
    $ mkdir -p ~/fbfs/friends
    $ mkdir ./rootdir
    $ echo <ACCESS_TOKEN_STRING> > ~/.fbfs/auth_token

# Usage

Now mount fbfs on to ~/fbfs

    $ ./fbfs ./rootdir ~/fbfs

    $ ls ~/fbfs/friends
    # ...list of friends as directories...

    $ echo "test post" > ~/fbfs/post
    # check your facebook wall

# To-do
- Flesh this thing out
- Write an adaptive program to automatically generate C bindings for
  Facebook's (or anyone's) API.

