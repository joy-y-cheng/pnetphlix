#include "Recommender.h"
#include "User.h"
#include "UserDatabase.h"
#include "Movie.h"
#include "MovieDatabase.h"
#include "treemm.h"

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
 : m_udb(user_database), m_mdb(movie_database)
{
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* u = m_udb.get_user_from_email(user_email);
    
    if (u == nullptr)
        return vector<MovieAndRank>();
    
    vector<string> watchedMovies = u->get_watch_history();
    
    // Iterate through vector of watched movies and add candidate movies to map
    map<string, int> candidateMovies;
    vector<string>::iterator it = watchedMovies.begin();
    while (it != watchedMovies.end())
    {
        Movie* m = m_mdb.get_movie_from_id(*it);
        
        // Get vector of directors and iterate through
        vector<string> directors = m->get_directors();
        vector<string>::iterator dit = directors.begin();
        while (dit != directors.end())
        {
            // Get movies that share director and iterate through
            vector<Movie*> dMovies = m_mdb.get_movies_with_director(*dit);
            vector<Movie*>::iterator dmit = dMovies.begin();
            while (dmit != dMovies.end())
            {
                // Add to map
                candidateMovies[(*dmit)->get_id()] += 20;
                dmit++;
            }
            dit++;
        }
        
        // Get vector of actors and iterate through
        vector<string> actors = m->get_actors();
        vector<string>::iterator ait = actors.begin();
        while (ait != actors.end())
        {
            // Get movies that share actor and iterate through
            vector<Movie*> aMovies = m_mdb.get_movies_with_actor(*ait);
            vector<Movie*>::iterator amit = aMovies.begin();
            while (amit != aMovies.end())
            {
                // Add to map
                candidateMovies[(*amit)->get_id()] += 30;
                amit++;
            }
            ait++;
        }
        
        // Get vector of genres and iterate through
        vector<string> genres = m->get_genres();
        vector<string>::iterator git = genres.begin();
        while (git != genres.end())
        {
            // Get movies that share genre and iterate through
            vector<Movie*> gMovies = m_mdb.get_movies_with_genre(*git);
            vector<Movie*>::iterator gmit = gMovies.begin();
            while (gmit != gMovies.end())
            {
                // Add to map
                candidateMovies[(*gmit)->get_id()] += 1;
                gmit++;
            }
            git++;
        }
        
        it++;
    }
    
    // Create set from vector of watched movies
    set<string> watchedMoviesSet(watchedMovies.begin(), watchedMovies.end());
    
    vector<MovieAndRank> recommendations;
    // Iterate through candidate movies vector
    for (map<string, int>::iterator mit = candidateMovies.begin(); mit != candidateMovies.end(); mit++)
    {
        // Check that candidate movie is NOT in set of watched movies
        if (watchedMoviesSet.find(mit->first) == watchedMoviesSet.end())
            // Add unwatched candidate movie to recommendations vector
            recommendations.push_back(MovieAndRank(mit->first, mit->second));
    }
    
    // Sort vector based on hasHigherRanking criteria
    quicksort(recommendations, 0, static_cast<int>(recommendations.size()) - 1);
    
    // Add up to movie_count items to orderedRecommendations vector
    vector<MovieAndRank> orderedRecommendations;
    int count = 0;
    while (count < movie_count && count < recommendations.size())
    {
        orderedRecommendations.push_back(recommendations[count]);
        count++;
    }
    return orderedRecommendations;
}

// Returns true if mr1 should be ranked higher than mr2
bool Recommender::hasHigherRanking(const MovieAndRank& mr1, const MovieAndRank& mr2) const
{
    // Compare compatibility scores
    if (mr1.compatibility_score > mr2.compatibility_score)
        return true;
    if (mr1.compatibility_score < mr2.compatibility_score)
        return false;
    
    Movie* m1 = m_mdb.get_movie_from_id(mr1.movie_id);
    Movie* m2 = m_mdb.get_movie_from_id(mr2.movie_id);
    
    // Compare ratings
    float rating1 = m1->get_rating();
    float rating2 = m2->get_rating();
    if (rating1 > rating2)
        return true;
    if (rating1 < rating2)
        return false;
    
    // Compare titles
    string title1 = m1->get_title();
    string title2 = m2->get_title();
    return title1 < title2;
}

int Recommender::partition(vector<MovieAndRank>& mr, int begin, int end) const
{
    // Make pivot first value in vector
    int pivot = begin;
    int i = begin + 1;
    int j = end;
    while (i < j)
    {
        if (hasHigherRanking(mr[i], mr[pivot]))
            i++;
        else if (hasHigherRanking(mr[pivot], mr[j]))
            j--;
        else
        {
            // Swap if mr[i] and mr[j] are both on respective wrong sides of vector
            swap(mr[i], mr[j]);
            i++;
            j--;
        }
    }
    // Swap pivot with last value less than pivot
    if (hasHigherRanking(mr[i], mr[pivot]))
    {
        swap(mr[i], mr[pivot]);
        return i;
    }
    else
    {
        swap(mr[i - 1], mr[pivot]);
        return i - 1;
    }
}

void Recommender::quicksort(vector<MovieAndRank>& mr, int begin, int end) const
{
    // Recursively sort using partition function
    if (begin < end)
    {
        int p = partition(mr, begin, end);
        quicksort(mr, begin, p - 1);
        quicksort(mr, p + 1, end);
    }
}
