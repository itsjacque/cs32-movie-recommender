#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

UserDatabase::UserDatabase()
{
    // Replace this line with correct code.
}
UserDatabase::~UserDatabase()
{
}

bool UserDatabase::load(const string& filename)
{
    ifstream infile(filename);    // infile is a name of our choosing
    if (!infile)                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    while (infile){
        std::string name;
        getline(infile,name);
        
        std::string email;
        getline(infile,email);
        
        int movieNum;
        infile >> movieNum;
        infile.ignore(10000,'\n');
              
        vector<std::string> moviesWatched;
        std::string movie;
        for(int i = 0; i < movieNum; i++){
            getline(infile, movie);
            moviesWatched.push_back(movie);
        }
        
        userMap.insert(email, User(name, email, moviesWatched));
        
        //to ignore the blank space after the last time
        string temp;
        getline(infile, temp);
    }
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string,User>::Iterator it = userMap.find(email);
    if(it.is_valid())
       return &it.get_value();
    else
        return nullptr;

}
