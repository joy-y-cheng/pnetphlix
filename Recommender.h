#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include "UserDatabase.h"
#include "MovieDatabase.h"

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {}

    std::string movie_id;
    int compatibility_score;
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;

  private:
    const UserDatabase &m_udb;
    const MovieDatabase &m_mdb;
    
    // Helper functions for sorting vector of recommendations
    int partition(std::vector<MovieAndRank>& mr, int begin, int end) const;
    void quicksort(std::vector<MovieAndRank>& mr, int begin, int end) const;
    bool hasHigherRanking(const MovieAndRank& mr1, const MovieAndRank& mr2) const;
};

#endif // RECOMMENDER_INCLUDED
