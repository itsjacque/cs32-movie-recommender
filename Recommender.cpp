#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database): udb(user_database), mdb(movie_database)
{
//    udb = user_database;
//    mdb = movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{


    vector<string> watchHistory;
    unordered_map<Movie*, int> map;
    User* user1 = udb.get_user_from_email(user_email);
    watchHistory = user1->get_watch_history();
    
    vector<Movie*> moviesWithDirectors;
    vector<Movie*> moviesWithActors;
    vector<Movie*> moviesWithGenre;
    
    
    
    //for movies watched
    unordered_set<Movie*> moviesWatched;
    
    // director
    unordered_set<Movie*> directors;
    
    //stores id
    unordered_set<string> movThatMeetReq; // movies that meet at least one req.
    
    for(int i = 0; i < watchHistory.size(); i++)
    {
        string movieID = watchHistory[i];
        Movie* movie = mdb.get_movie_from_id(movieID);
        moviesWatched.insert(movie);
        
        vector<string> directorVector = movie->get_directors();
        for(auto blt: directorVector)
            moviesWithDirectors = mdb.get_movies_with_director(blt);
        
        //Insert movies with the same director to the movies that meet at least one requirement set
        for(auto it: moviesWithDirectors){
            string a = it->get_id();
            movThatMeetReq.insert(a);
        }
//
        vector<string> actorVector = movie->get_actors();
        for(auto blt: actorVector)
            moviesWithActors = mdb.get_movies_with_actor(blt);

        for(auto it: moviesWithActors)
                movThatMeetReq.insert(it->get_id());
//
        vector<string> genreVector = movie->get_genres();
        for(auto blt: genreVector)
            moviesWithGenre = mdb.get_movies_with_genre(blt);

        for(auto it: moviesWithGenre)
                movThatMeetReq.insert(it->get_id());
        
    }
    
    cerr << movThatMeetReq.size();
    
    // deleting the movies already watched
    
    
    for (auto it: movThatMeetReq){
        for(auto jt: moviesWatched) {
                movThatMeetReq.erase(jt->get_id());
        }
    }
    

    
    cerr << movThatMeetReq.size();
    //assume we already deleted the movies user watched
    vector<pair<Movie*, int>> scores;

    
    //it contains movie pointers
    unordered_set<string>:: iterator it;
    for(it = movThatMeetReq.begin(); it != movThatMeetReq.end(); it++){
        int score = 0;
      
        for(unordered_set<Movie*>::iterator bo = moviesWatched.begin(); bo !=  moviesWatched.end(); bo++){
            Movie* getMovptr = mdb.get_movie_from_id(*it);
                for(auto ji: (*bo)->get_directors())
                {
                    for(auto b: getMovptr->get_directors()){
                        if(ji == b)
                            score += 20;
                    }
                
                }
            
            for(auto ji: (*bo)->get_actors())
            {
                for(auto b: getMovptr->get_actors()){
                    if(ji == b)
                        score += 30;
                }
            
            }
            
            for(auto ji: (*bo)->get_genres())
            {
                for(auto b: getMovptr->get_genres()){
                    if(ji == b)
                        score += 1;
                }
            
            }
        }
        scores.push_back(make_pair(mdb.get_movie_from_id(*it), score));

    }
   
    sort (scores.begin(), scores.end(), [](const pair<Movie*, int>& a, const pair<Movie*, int>& b) {
        if(a.second != b.second) {
            return a.second > b.second;
        } else if (a.first->get_rating() != b.first->get_rating()) {
            return a.first->get_rating() > b.first->get_rating();
        }  else
              return a.first->get_title() < b.first->get_title();
    });
        
   // Replace this line with correct code.

    vector<MovieAndRank> movieRank;
    
    for(int k = 0; k<scores.size(); k++)
    {
        if(k > movie_count - 1)
            break;
        movieRank.push_back(MovieAndRank(scores[k].first->get_id(), scores[k].second));
    }
    
    return movieRank;
}
