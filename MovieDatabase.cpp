#include "MovieDatabase.h"
#include "Movie.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
}

bool MovieDatabase::load(const string& filename)
{
    ifstream infile(filename);    // infile is a name of our choosing
    if (!infile)                // Did opening the file fail?
    {
        cerr << "Error: Cannot open data.txt!" << endl;
        return false;
    }
    
    while (infile) {
        
        string movieID;
        getline(infile, movieID);
        
        string movieName;
        getline(infile, movieName);
        
        string year;
        getline(infile, year);
        
        char chMovieDir = 'a';
        string movieDir = "";
        string entireLine = "";
        getline(infile, entireLine);
        std::vector<std::string> moviesDirV;
        
        for (int i = 0; i < entireLine.size(); i++) {
            if (entireLine[i] != ',') {
                chMovieDir = toupper(entireLine[i]);
                movieDir += chMovieDir;
            } else {
                moviesDirV.push_back(movieDir);
                movieDir = "";
            }
        }
        
        moviesDirV.push_back(movieDir);

        char chMovieActor = 'a';
        string movieActor = "";
        entireLine = "";
        getline(infile, entireLine);
        std::vector<std::string> moviesActorV;
        
        for (int i = 0; i < entireLine.size(); i++) {
            if (entireLine[i] != ',') {
                chMovieActor = toupper(entireLine[i]);
                movieActor += chMovieActor;
            } else {
                moviesActorV.push_back(movieActor);
                movieActor = "";
            }
        }
        
        moviesActorV.push_back(movieActor);
        

        char chGenre = 'a';
        string genre = "";
        entireLine = "";
        getline(infile, entireLine);
        std::vector<std::string> moviesGenreV;
        
        for (int i = 0; i < entireLine.size(); i++) {
            if (entireLine[i] != ','){
                chGenre = toupper(entireLine[i]);
                genre += chGenre;
            } else {
                moviesGenreV.push_back(genre);
                genre = "";
            }
        }
        
        moviesGenreV.push_back(genre);
        
        float rating;
        infile >> rating;
        infile.ignore(10000,'\n');
        
        
        movieMap.insert(movieID, Movie(movieID, movieName, year, moviesDirV, moviesActorV, moviesGenreV, rating));
         
        
        for(auto it: moviesDirV){
            moviesDir.insert(it, Movie(movieID, movieName, year, moviesDirV, moviesActorV, moviesGenreV, rating));
        }
        
        for(auto it: moviesActorV){
            moviesActor.insert(it, Movie(movieID, movieName, year, moviesDirV, moviesActorV, moviesGenreV, rating));
        }
        
        for(auto it: moviesGenreV){
            moviesGenre.insert(it, Movie(movieID, movieName, year, moviesDirV, moviesActorV, moviesGenreV, rating));
        }
        
        string temp;
        getline(infile, temp);
    }
    return true;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<std::string,Movie>::Iterator it = movieMap.find(id);
    if(it.is_valid())
       return &it.get_value();
    else
        return nullptr; // Replace this line with correct code.
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    string temp= "";
    for (int i = 0; i < director.size(); i++){
        temp += toupper(director[i]);
    }
    vector<Movie*> m;
    TreeMultimap<std::string,Movie>::Iterator it = moviesDir.find(temp);
    while (it.is_valid()){
        m.push_back(&it.get_value());
        it.advance();
    }
    
    return m;
////    i want to go into each node and access the vector inside of them
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    string temp= "";
    for (int i = 0; i < actor.size(); i++){
        temp += toupper(actor[i]);
    }
    vector<Movie*> a;
    TreeMultimap<std::string,Movie>::Iterator it = moviesActor.find(temp);
    while (it.is_valid()){
        a.push_back(&it.get_value());
        it.advance();
    }
    
    return a;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    string temp= "";
    for (int i = 0; i < genre.size(); i++){
        temp += toupper(genre[i]);
    }
    vector<Movie*> g;
    TreeMultimap<std::string,Movie>::Iterator it = moviesGenre.find(temp);
    while (it.is_valid()){
        g.push_back(&it.get_value());
        it.advance();
    }
    
    return g;
}

