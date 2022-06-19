// Matthew Delgado
// Assignment Chapter 3 - SongLinkedList
// ECE 2552 Software/Hardware Integration
// Dr. Smith
// 2-10-22

/* Program is supposed to mimic a playlist of songs but for ease of implementation, integers are used to represent song names. */

#include "SongLinkedList.hpp"

void SongPlaylistDemo();
void mainMenu();
std::string namePlaylist();
Playlist<int>& addTo(Playlist<int>& pl);

int main(){
    SongPlaylistDemo();
    return EXIT_SUCCESS;
}

void SongPlaylistDemo(){
    mainMenu();
    std::string playlistName = namePlaylist();
    Playlist<int> pl;
    
    pl.push_back(1);
    pl.push_back(2);
    std::cout << "Playing: " << playlistName << std::endl;
    pl.loopOnce();
    std::cout << std::endl << std::endl;
    
    mainMenu();
    std::string playlistName2 = namePlaylist();
    Playlist<int> pl2 = pl;
    
    pl2.remove(2);
    pl2.push_front(3);
    std::cout << "Playing: " << playlistName2 << std::endl;
    pl2.loopOnce();
    std::cout << std::endl << std::endl;
}

void mainMenu(){
    std::string userInput;
    while(userInput != "YES"){
        std::cout << "Create new playlist? (YES/NO)" << std::endl;
        std::cin >> userInput;
    }
    std::cout << std::endl;
}

std::string namePlaylist(){
    std::string playlistName = "Unnamed";
    std::cout << "Playlist Name: ";
    std::cin >> playlistName;
    return playlistName;
}
