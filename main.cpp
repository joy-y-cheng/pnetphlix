#include "UserDatabase.h"
#include "User.h"
#include "MovieDatabase.h"
#include "Movie.h"
#include "Recommender.h"
#include <iostream>
#include <string>
#include "treemm.h"
using namespace std;

const string USER_DATAFILE  = "users.txt";
const string MOVIE_DATAFILE = "movies.txt";

void findMatches(const Recommender& r,
                 const MovieDatabase& md,
                 const string& user_email,
                 int num_recommendations) {
    vector<MovieAndRank> recommendations =
    r.recommend_movies(user_email, num_recommendations);
    if (recommendations.empty())
        cout << "We found no movies to recommend :(.\n";
    else {
        for (int i = 0; i < recommendations.size(); i++) {
            const MovieAndRank& mr = recommendations[i];
            Movie* m = md.get_movie_from_id(mr.movie_id);
            cout << i + 1 << ".\n" << m->get_title() << " ("
            << m->get_release_year() << ")\nRating: "
            << m->get_rating() << "\nCompatibility Score: "
            << mr.compatibility_score << "\n\n";
        }
    }
}

int main()
{
    // USER DATABASE TESTS
    UserDatabase udb;
    if (!udb.load(USER_DATAFILE))
    {
        cout << "Failed to load user data file " << USER_DATAFILE << "!" << endl;
        return 1;
    }

    cout << "Loaded user database" << endl;

    // MOVIE DATABASE TESTS
    MovieDatabase mdb;
    if (!mdb.load(MOVIE_DATAFILE))
    {
        cout << "Failed to load movie data file " << MOVIE_DATAFILE << "!" << endl;
        return 1;
    }

    cout << "Loaded movie database" << endl;

    // RECOMMENDER TESTS
    Recommender r(udb, mdb);
    for (;;)
    {
        cout << "Enter user email address (or quit): ";
        string email;
        getline(cin, email);
        if (email == "quit")
            break;
        cout << "Enter number of movies to recommend: ";
        int numRecs;
        cin >> numRecs;
        cin.ignore(10000, '\n');
        findMatches(r, mdb, email, numRecs);
    }
}
