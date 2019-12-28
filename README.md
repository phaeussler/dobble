# Dobble
Dobble is a simple pattern recognition game in which players try to find an word shown on two cards. Each card in Dobble features ten different words in disorder. Any two cards have exactly one word in common.


The fist time that you want to play, make sure to do make
```console
make
```

If you wanna play:
For each time that you want to play the server shoud be running.
```console
./server -p 3000 -l -i 127.0.0.1;
```
And for each player run.
```console
./client -p 3000 -l -i 127.0.0.1;
```

In `src/server/globlas.c` you can change te number of players and the time to wait until the game will start.

Enjoy the game 
