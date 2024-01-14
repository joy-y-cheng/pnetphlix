#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
}

UserDatabase::~UserDatabase()
{
    // Delete all dynamically allocated users
    vector<User*>::iterator p = m_users.begin();
    int n = 0;
    while (p != m_users.end())
    {
        delete *p;
        p = m_users.erase(p);
        n++;
    }
}

bool UserDatabase::load(const string& filename)
{
    // If function has already been called on file
    if (m_users.size() != 0)
        return false;
    
    ifstream infile(filename);
    
    // If file does not exist
    if(infile.fail()){
        return false;
    }
    
    for ( ; ; )
    {
        // Read in name
        string full_name;
        getline(infile, full_name);
        
        // Check that we have not reached end of file
        if (!infile)
            break;
        
        // Read in email
        string email;
        getline(infile, email);
        
        // Read in count
        int count;
        infile >> count;
        infile.ignore(10000, '\n');
        
        // Add movie IDs to vector
        vector<string> watch_history;
        for (int i = 0; i < count; i++)
        {
            string movie;
            getline(infile, movie);
            watch_history.push_back(movie);
        }
        
        // Allocate new user in vector
        User* u = new User(full_name, email, watch_history);
        m_users.push_back(u);
        
        // Add user to email tree
        m_emailTree.insert(email, u);
        
        // Ignore blank line
        infile.ignore(10000, '\n');
    }
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string, User*>::Iterator it = m_emailTree.find(email);
    if (!it.is_valid())
        return nullptr;
    return it.get_value();
}
