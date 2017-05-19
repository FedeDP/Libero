# Libero
In italy we got this beautiful and funny (even if it pretends to be serious...) newspaper called [Libero Quotidiano](http://www.liberoquotidiano.it/).  
Well, it is so funny that i had to make a small, stupid c program to download their page and check some words occurrences :D  

To build, it only needs libcurl and pkg-config (only needed at build time). Just run:

    $ make

To run:
    
    $ ./libero list of words to be searched

It will then print each words occurrences and store the result to libero.csv in current folder.
