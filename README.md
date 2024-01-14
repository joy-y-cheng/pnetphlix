# PnetPhlix

PnetPhlix, a movie recommender based on Netflix, loads searchable databases of users and movies from text files and recommends movies to users based on their movie-watching histories. For a given user, PnetPhlix determines the attributes of the user's previously watched movies and finds new movies with those attributes, ranking the recommendations in order of number of attributes shared with previously watched movies.  

## Building and running the code

Run the following commands from the command line:
- `g++ *.cpp -o pnetphlix`
- `./pnetphlix`

## Acknowledgements

I created PnetPhlix as a project for my COM SCI 32 class at UCLA. Professors Carey Nachenberg and David Smallberg wrote and assigned the specification.
