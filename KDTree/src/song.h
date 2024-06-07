#ifndef SONG_H
#define SONG_H

#include <string>
#include <vector>
#include <map>

class Song
{
private:
    const std::vector<std::string> keys = {
        "valence",
        "year",
        "acousticness",
        "danceability",
        "duration_ms",
        "energy",
        "explicit",
        "instrumentalness",
        "key",
        "liveness",
        "loudness",
        "mode",
        "popularity",
        "speechiness",
        "tempo"};

    std::map<std::string, double> numeric_data;

    std::string id;
    std::string name;
    std::vector<std::string> artists;
    std::string release_date;

public:
    // Destructor
    ~Song() {}
    // Constructor
    Song() {}
    Song(std::map<std::string, double> numeric_data,
         std::string id,
         std::string name,
         std::vector<std::string> artists,
         std::string release_date);

    // All operators
    bool operator<(const Song &song) const;
    bool operator==(const Song &song) const;
    bool operator!=(const Song &song) const;
    bool operator>(const Song &song) const;

    // Assigmnet operator
    Song &operator=(const Song &song);

    // Coordinate getter
    std::vector<double> get_coordinates() const;
};

#endif
