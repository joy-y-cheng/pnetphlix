#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

MovieDatabase::MovieDatabase()
{
}

MovieDatabase::~MovieDatabase()
{
    // Delete all dynamically allocated movies
    vector<Movie*>::iterator p = m_movies.begin();
    int n = 0;
    while (p != m_movies.end())
    {
        delete *p;
        p = m_movies.erase(p);
        n++;
    }
}

bool MovieDatabase::load(const string& filename)
{
    // If function has already been called on file
    if (m_movies.size() != 0)
        return false;
    
    ifstream infile(filename);
    
    // If file does not exist
    if(infile.fail()){
        return false;
    }
    
    for ( ; ; )
    {
        // Read in id
        string id;
        getline(infile, id);
        
        // Check that we have not reached end of file
        if (!infile)
            break;
        
        // Read in title
        string title;
        getline(infile, title);
        
        // Read in release year
        string release_year;
        getline(infile, release_year);
        
        // Add directors to vector
        string allDirectors;
        getline(infile, allDirectors);
        vector<string> directors;
        string curDirector;
        for (int i = 0; i < allDirectors.size(); i++)
        {
            char c = allDirectors.at(i);
            if (c != ',')
                curDirector += c;
            else
            {
                directors.push_back(curDirector);
                curDirector = "";
            }
        }
        directors.push_back(curDirector);
        
        // Add actors to vector
        string allActors;
        getline(infile, allActors);
        vector<string> actors;
        string curActor;
        for (int i = 0; i < allActors.size(); i++)
        {
            char c = allActors.at(i);
            if (c != ',')
                curActor += c;
            else
            {
                actors.push_back(curActor);
                curActor = "";
            }
        }
        actors.push_back(curActor);
        
        // Add genres to vector
        string allGenres;
        getline(infile, allGenres);
        vector<string> genres;
        string curGenre;
        for (int i = 0; i < allGenres.size(); i++)
        {
            // Make lowercase
            char c = allGenres.at(i);
            if (c != ',')
                curGenre += c;
            else
            {
                genres.push_back(curGenre);
                curGenre = "";
            }
        }
        genres.push_back(curGenre);
        
        // Read in rating
        float rating;
        infile >> rating;
        infile.ignore(10000, '\n');
        
        // Allocate new movie in vector
        Movie* m = new Movie(id, title, release_year, directors, actors, genres, rating);
        m_movies.push_back(m);
        
        // Make id lowercase
        for (int i = 0; i < id.size(); i++)
            id.at(i) = tolower(id.at(i));
        // Add movie to ID tree
        m_idTree.insert(id, m);
        
        // For each director, add movie to director tree
        vector<string>::iterator d = directors.begin();
        while (d != directors.end())
        {
            // Make lowercase
            for (int i = 0; i < (*d).size(); i++)
                (*d).at(i) = tolower((*d).at(i));
            m_directorTree.insert(*d, m);
            d++;
        }
        
        // For each actor, add movie to actor tree
        vector<string>::iterator a = actors.begin();
        while (a != actors.end())
        {
            // Make lowercase
            for (int i = 0; i < (*a).size(); i++)
                (*a).at(i) = tolower((*a).at(i));
            m_actorTree.insert(*a, m);
            a++;
        }
        
        // For each genre, add movie to genre tree
        vector<string>::iterator g = genres.begin();
        while (g != genres.end())
        {
            // Make lowercase
            for (int i = 0; i < (*g).size(); i++)
                (*g).at(i) = tolower((*g).at(i));
            m_genreTree.insert(*g, m);
            g++;
        }
        
        // Ignore blank line
        infile.ignore(10000, '\n');
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    // Create lowercase string from id
    string id_lower;
    for (int i = 0; i < id.size(); i++)
        id_lower += tolower(id.at(i));
    TreeMultimap<std::string, Movie*>::Iterator it = m_idTree.find(id_lower);
    if (!it.is_valid())
        return nullptr;
    return it.get_value();
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    // Create lowercase string from director
    string d_lower;
    for (int i = 0; i < director.size(); i++)
        d_lower += tolower(director.at(i));
    TreeMultimap<std::string, Movie*>::Iterator it = m_directorTree.find(d_lower);
    if (!it.is_valid())
        return vector<Movie*>();
    vector<Movie*> movies;
    // Put all movies in tree vector into new vector
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    // Create lowercase string from actor
    string a_lower;
    for (int i = 0; i < actor.size(); i++)
        a_lower += tolower(actor.at(i));
    TreeMultimap<std::string, Movie*>::Iterator it = m_actorTree.find(a_lower);
    if (!it.is_valid())
        return vector<Movie*>();
    vector<Movie*> movies;
    // Put all movies in tree vector into new vector
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    // Create lowercase string from genre
    string g_lower;
    for (int i = 0; i < genre.size(); i++)
        g_lower += tolower(genre.at(i));
    TreeMultimap<std::string, Movie*>::Iterator it = m_genreTree.find(g_lower);
    if (!it.is_valid())
        return vector<Movie*>();
    vector<Movie*> movies;
    // Put all movies in tree vector into new vector
    while (it.is_valid())
    {
        movies.push_back(it.get_value());
        it.advance();
    }
    return movies;
}
