/******************
Name: Neta Rosenzweig
ID: 323885582
Assignment: ex.5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct that represents a song with the basic properties
typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;
// Struct that represents a playlist that can hold multiple songs
typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

int printMainMenu ();
int watchPlaylist(Playlist **playlists, int playlistNum);
void handlePlaylistMenu(Playlist *selectedPlaylist);
void removePlaylist(Playlist **playlists, int *playlistNum);
void addPlaylist(Playlist ***playlistsPointer, int *playlistsNum);
void freeAllMemory(Playlist **playlists, int playlistsNum);
void showPlaylist(Playlist *playlist);
void addSong(Playlist *playlist);
void deleteSong(Playlist *playlist);
void sortPlaylist(Playlist *playlist);
void sortByYear(Playlist *playlist);
void sortByStreamsAscending(Playlist *playlist);
void sortByStreamsDescending(Playlist *playlist);
void sortByAlphabetically(Playlist *playlist);
void playSong(Song *song);
void playPlaylist(Playlist *playlist);
Song *createSong(char *title, char *artist, int year, char *lyrics);
char *readString();
void freeSong(Song *song);
int stringLength(char *string);

int main() {
    // Initialize program variables
    // Array to store all playlists
    Playlist **playlists = NULL;
    // Counter for number of playlists
    int playlistsNum = 0;
    // User's menu choice
    int choice = 0;
    do {
        // Get user's choice from main menu
        choice = printMainMenu();
        switch (choice) {
            // Watch or interact with playlists
            case 1: {
                watchPlaylist(playlists, playlistsNum);
                    break;
                }
            // Create new playlist
            case 2: {
                addPlaylist(&playlists, &playlistsNum);
                break;
            }
            // Delete existing playlist
            case 3: {
                removePlaylist(playlists, &playlistsNum);
                break;
            }
            // Exit program
            case 4: {
                    freeAllMemory(playlists, playlistsNum);
                    printf("Goodbye!\n");
                break;
            }
        }
    }
    // Continue until user chooses to exit
    while (choice != 4);
    // Program completed successfully
    return 0;
}

// Print the playlist menu
int printMainMenu (){
    int choice;
    printf("Please Choose:\n\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");
    //Read the choice
    scanf("%d", &choice);
    return choice;
}
// Displays all playlists and handles user interaction with them
int watchPlaylist(Playlist **playlists, int playlistNum) {
    int choice;
    do {
        // If no playlists exist
        if (playlists == NULL || playlistNum == 0) {
            printf("Choose a playlist:\n1. Back to main menu\n");
            scanf("%d", &choice);
            // Return to main menu
            return 1;
        }
        // Show list of all playlists
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlistNum; i++) {
            printf("%d. %s\n", i + 1, playlists[i]->name);
        }
        printf("%d. Back to main menu\n", playlistNum + 1);
        scanf("%d", &choice);
        // If user chose to go back
        if (choice == playlistNum + 1) {
            return 1;
        }
        // If valid playlist was chosen, open its menu
        if (choice > 0 && choice <= playlistNum) {
            handlePlaylistMenu(playlists[choice - 1]);
        }
    } while (choice != playlistNum + 1);
    return 1;
}
// Handles all operations within a selected playlist menu
void handlePlaylistMenu(Playlist *selectedPlaylist) {
    int playlistChoice;
    // Display playlist name and menu
    printf("playlist %s:\n", selectedPlaylist->name);
    do {
        // Show all available options for playlist operations
        printf("\t1. Show Playlist\n\t2. Add Song\n\t3. Delete Song\n\t4. Sort\n\t5. Play\n\t6. exit\n");
        // Get user choice and clear input buffer
        scanf("%d", &playlistChoice);
        getchar();
        switch (playlistChoice) {
            // Show all songs in playlist
            case 1: {
                showPlaylist(selectedPlaylist);
                break;
            }
            // Add a new song
            case 2: {
                addSong(selectedPlaylist);
                break;
            }
            // Remove a song
            case 3: {
                deleteSong(selectedPlaylist);
                break;
            }
            // Sort songs in playlist
            case 4: {
                sortPlaylist(selectedPlaylist);
                break;
            }
            // Play all songs in playlist
            case 5: {
                playPlaylist(selectedPlaylist);
                break;
            }
            // Exit playlist menu
            case 6: {
                return;
            }
        }
    }
    // Keep showing menu until user chooses to exit
    while (playlistChoice != 6);
}
// Removes a playlist chosen from the list of playlists
void removePlaylist(Playlist **playlists, int *playlistNum) {
    int choice;
    Playlist *toDelete;
    // Show all available playlists
    printf("Choose a playlist:\n");
    for(int i = 0; i < *playlistNum; i++) {
        printf("\t%d. %s\n", i + 1, playlists[i]->name);
    }
    printf("\t%d. Back to main menu\n", *playlistNum + 1);
    // Read the user choice
    scanf("%d", &choice);
    // If user wants to go back
    if(choice == *playlistNum + 1) {
        return;
    }
    // If choice is valid, delete the chosen playlist
    if(choice > 0 && choice <= *playlistNum) {
        toDelete = playlists[choice - 1];
        // First free all songs in the playlist
        for(int i = 0; i < toDelete->songsNum; i++) {
            freeSong(toDelete->songs[i]);
        }
        // Free the playlist's arrays and name
        free(toDelete->songs);
        free(toDelete->name);
        free(toDelete);
        // Move all playlists after it one position back
        for(int i = choice - 1; i < *playlistNum - 1; i++) {
            playlists[i] = playlists[i + 1];
        }
        // Decrease total number of playlists and confirm deletion
        (*playlistNum)--;
        printf("Playlist deleted.\n");
    }
}
// Creates a new playlist and adds it to the list
void addPlaylist(Playlist ***playlistsPointer, int *playlistsNum) {
    // Get the actual playlist array from the pointer to pointer
    Playlist **playlists = *playlistsPointer;
    // Get playlist name from user
    printf("Enter playlist's name:\n");
    // Clear newline after valid input
    getchar();
    char* playlistName = readString();
    // If we couldn't read the name of the playlist
    if (playlistName == NULL) {
        // Exit without creating playlist
        return;
    }
    // Allocate memory for the new playlist structure
    Playlist* newPlaylist = malloc(sizeof(Playlist));
    // Check if allocation failed and free memory
    if (newPlaylist == NULL) {
        free(playlistName);
        return;
    }
    // Initialize the new playlist
    newPlaylist->name = playlistName;
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;
    // If this is the first playlist
    if (playlists == NULL) {
        // Create the initial array of playlist pointers
        playlists = malloc(sizeof(Playlist*));
        // Check if allocation failed and free all the memory
        if (playlists == NULL) {
            free(playlistName);
            free(newPlaylist);
            return;
        }
    }
    // If we already have some playlists
    else {
        // Try to make allocate memory for new playlist in playlists
        Playlist **temp = realloc(playlists, sizeof(Playlist*) * (*playlistsNum + 1));
        // Check if allocation failed and free all the memory
        if (temp == NULL) {
            free(playlistName);
            free(newPlaylist);
            return;
        }
        playlists = temp;
    }
    // Add new playlist and update pointers and counters
    // Put new playlist in the array
    playlists[*playlistsNum] = newPlaylist;
    // Update the original array pointer
    *playlistsPointer = playlists;
    // Increment playlist count
    (*playlistsNum)++;
}
// Clean all th memory used by the program
void freeAllMemory(Playlist **playlists, int playlistsNum) {
    // Check if there is memory allocation to free
    if (playlists == NULL) {
        return;
    }
    // Go through each playlist
    for (int i = 0; i < playlistsNum; i++) {
        if (playlists[i] != NULL) {
            // For each playlist, first free all its songs
            if (playlists[i]->songs != NULL) {
                for (int j = 0; j < playlists[i]->songsNum; j++) {
                    freeSong(playlists[i]->songs[j]);
                }
                // Free the array of song pointers
                free(playlists[i]->songs);
            }
            // Free the playlist name and the playlist itself
            if (playlists[i]->name != NULL) {
                free(playlists[i]->name);
            }
            free(playlists[i]);
        }
    }
    // Finally free the array of playlist pointers
    free(playlists);
}
// Shows all songs in playlist
void showPlaylist(Playlist *playlist) {
    int choice;
    // Only show options if playlist has songs
    if (playlist->songsNum > 0) {
        // Display all songs with their details
        for (int i = 0; i < playlist->songsNum; i++) {
            printf("%d. Title: %s\n", i + 1, playlist->songs[i]->title);
            printf("Artist: %s\n", playlist->songs[i]->artist);
            printf("Released: %d\n", playlist->songs[i]->year);
            printf("Streams: %d\n", playlist->songs[i]->streams);
        }
        // Let user play songs until choose to quit
        do {
            printf("choose a song to play, or 0 to quit:\n");
            // Check for invalid input
            if (scanf("%d", &choice) != 1) {
                // Clear invalid input
                while (getchar() != '\n');
                continue;
            }
            // Clear newline after valid number
            getchar();
            // Play chosen song if choice is valid
            if (choice != 0 && choice <= playlist->songsNum) {
                playSong(playlist->songs[choice - 1]);
            }
        }
        // Stop play songs and quit
        while (choice != 0);
    }
}
/*
Gets song details from user and adds the song to the playlist
If any memory allocation fails, the function cleans up and returns
 */
void addSong(Playlist *playlist) {
    char *title;
    char *artist;
    char *lyrics;
    int year;
    Song *newSong;
    printf("Enter song's details\n");
    printf("Title:\n");
    // Read the title from the user
    title = readString();
    printf("Artist:\n");
    // Read the artist from the user
    artist = readString();
    printf("Year of release:\n");
    // Read the year from the user until valid number is entered
    while (scanf("%d", &year) != 1) {
        // Clear invalid input
        while (getchar() != '\n');
    }
    // Clear newline after valid number
    getchar();
    printf("Lyrics:\n");
    // Read the lyrics from the user
    lyrics = readString();
    // Create new song with provided details
    newSong = createSong(title, artist, year, lyrics);
    // Free temp strings as they are now copied into the song
    free(title);
    free(artist);
    free(lyrics);
    // Try to make allocate memory for new song in playlist
    Song **temp = (Song**)realloc(playlist->songs, sizeof(Song*) * (playlist->songsNum +1));
    if (temp == NULL) {
        // Free if realloc fails
        freeSong(newSong);
        return;
    }
    // Add new song to playlist
    playlist->songs = temp;
    playlist->songs[playlist->songsNum] = newSong;
    playlist->songsNum++;
}
// Delete song from the playlist
void deleteSong(Playlist *playlist) {
    int choice;
    // If playlist has songs, display all songs information
    if (playlist->songsNum > 0) {
        //Loop for each song in the playlist
        for (int i = 0; i < playlist->songsNum; i++) {
            printf("%d. Title: %s\n", i + 1, playlist->songs[i]->title);
            printf("Artist: %s\n", playlist->songs[i]->artist);
            printf("Released: %d\n", playlist->songs[i]->year);
            printf("Streams: %d\n", playlist->songs[i]->streams);
            printf("\n");
        }
    }
    printf("choose a song to delete, or 0 to quit:\n");
    //Read the user choice
    scanf("%d", &choice);
    // If user chooses 0, exit without deleting
    if (choice == 0) {
        return;
    }
    // Check if choice is valid and delete the chosen song
    if(choice <= playlist->songsNum) {
        // Free the memory of chosen song
        freeSong(playlist->songs[choice-1]);
        // Move all songs after it one position back
        for(int i = choice - 1; i < playlist->songsNum - 1; i++) {
            playlist->songs[i] = playlist->songs[i + 1];
        }
        // Decrease total number of songs and confirm deletion
        playlist->songsNum--;
        printf("Song deleted successfully.\n");
    }
}
// Handles the sorting menu and user's sorting choice
void sortPlaylist(Playlist *playlist) {
    int choice;
    // Display sorting options to user
    printf("choose:\n");
    printf("1. sort by year\n");
    printf("2. sort by streams - ascending order\n");
    printf("3. sort by streams - descending order\n");
    printf("4. sort alphabetically\n");
    scanf("%d", &choice);
    // Default to alphabetical sort if choice is invalid
    if(choice < 1 || choice > 4) {
        choice = 4;
    }
    // Call appropriate sorting function according the user choice
    switch (choice) {
        case 1: {
            sortByYear(playlist);
            break;
        }
        case 2: {
            sortByStreamsAscending(playlist);
            break;
        }
        case 3: {
            sortByStreamsDescending(playlist);
            break;
        }
        case 4: {
            sortByAlphabetically(playlist);
            break;
        }
    }
    printf("sorted\n");
}
// Sorts the songs in a playlist by their release year (oldest to newest)
void sortByYear(Playlist *playlist) {
    // If there's 0 or 1 song, no need to sort
    if(playlist->songsNum <= 1) {
        return;
    }
    // Comparing each pair of songs
    for(int i = 0; i < playlist->songsNum - 1; i++) {
        // The biggest element place in the final position
        for(int j = 0; j < playlist->songsNum - i - 1; j++) {
            // Compare years of songs
            if(playlist->songs[j]->year > playlist->songs[j + 1]->year) {
                // Store the current song in temp
                Song *temp = playlist->songs[j];
                // Move the older song forward
                playlist->songs[j] = playlist->songs[j + 1];
                // Place the newer song after it
                playlist->songs[j + 1] = temp;
            }
        }
    }
}
// Sorts songs by number of streams from lowest to highest
void sortByStreamsAscending(Playlist *playlist) {
    // If there's 0 or 1 song, no need to sort
    if(playlist->songsNum <= 1) {
        return;
    }
    // Each iteration place one more song in its final position
    for(int i = 0; i < playlist->songsNum - 1; i++) {
        // The range gets smaller because the end is already sorted
        for(int j = 0; j < playlist->songsNum - i - 1; j++) {
            // If current song has more streams than the next one
            if(playlist->songs[j]->streams > playlist->songs[j + 1]->streams) {
                // Swap their positions to put less played song first
                Song *temp = playlist->songs[j];
                playlist->songs[j] = playlist->songs[j + 1];
                playlist->songs[j + 1] = temp;
            }
        }
    }
}
// Sorts songs by number of streams from highest to lowest
void sortByStreamsDescending(Playlist *playlist) {
    // If there's 0 or 1 song, no need to sort
    if(playlist->songsNum <= 1) {
        return;
    }
    // Each iteration place one more song in its final position
    for(int i = 0; i < playlist->songsNum - 1; i++) {
        // The range gets smaller because the end is already sorted
        for(int j = 0; j < playlist->songsNum - i - 1; j++) {
            // If current song has more streams than the next one
            if(playlist->songs[j]->streams < playlist->songs[j + 1]->streams) {
                // Swap their positions to put more played song first
                Song *temp = playlist->songs[j];
                playlist->songs[j] = playlist->songs[j + 1];
                playlist->songs[j + 1] = temp;
            }
        }
    }
}
// Sorts songs alphabetically (a to z) by their titles
void sortByAlphabetically(Playlist *playlist) {
    // If there's 0 or 1 song, no need to sort
    if(playlist->songsNum <= 1) {
        return;
    }
    // Each iteration place one more song in its final position
    for(int i = 0; i < playlist->songsNum - 1; i++) {
        // The range gets smaller because the end is already sorted
        for(int j = 0; j < playlist->songsNum - i - 1; j++) {
            // If first string comes after second alphabetically
            if(strcmp(playlist->songs[j]->title, playlist->songs[j + 1]->title) > 0) {
                // Swap songs if they're in wrong alphabetical order
                Song *temp = playlist->songs[j];
                playlist->songs[j] = playlist->songs[j + 1];
                playlist->songs[j + 1] = temp;
            }
        }
    }
}
// Plays a single song
void playSong(Song *song) {
    printf("Now playing %s:\n", song->title);
    // Print the lyrics of the song
    printf("$ %s $\n", song->lyrics);
    // Increases play count
    song->streams++;
    printf("\n");
}
// Plays all songs in a playlist by the order
void playPlaylist(Playlist *playlist) {
    // If the playlist is empty
    if(playlist->songsNum == 0) {
        // Do nothing and return
        return;
    }
    // Go through each song
    for(int i = 0; i < playlist->songsNum; i++) {
        // Play each song in order
        playSong(playlist->songs[i]);
    }
}
/*
Creates a new song by allocating memory and copying all the song details
Returns NULL if any memory allocation fails, otherwise returns the new song
*/
Song* createSong(char *title, char *artist, int year, char *lyrics) {
    // Allocate memory for the song structure itself
    Song *newSong = (Song*)malloc(sizeof(Song));
    // Check if allocation failed
    if (newSong == NULL) {
        return NULL;
    }
    // Allocate memory for the title string (plus 1 for null char)
    newSong->title = (char*)malloc(stringLength(title) + 1);
    if (newSong->title == NULL) {
        // Free what we allocated before failing
        free(newSong);
        return NULL;
    }
    // Allocate memory for the artist string (plus 1 for null char)
    newSong->artist = (char*)malloc(stringLength(artist) + 1);
    // Check if allocation failed
    if (newSong->artist == NULL) {
        // Free what we allocated before failing
        free(newSong->title);
        free(newSong);
        return NULL;
    }
    // Allocate memory for the lyrics string (plus 1 for null char)
    newSong->lyrics = (char*)malloc(stringLength(lyrics) + 1);
    // Check if allocation failed
    if (newSong->lyrics == NULL) {
        // Free what we allocated before failing
        free(newSong->title);
        free(newSong->artist);
        free(newSong);
        return NULL;
    }
    int i;
    // Copy the title char by char and add null char
    for(i = 0; title[i] != '\0'; i++) {
        newSong->title[i] = title[i];
    }
    newSong->title[i] = '\0';
    // Copy the artist char by char and add null char
    for(i = 0; artist[i] != '\0'; i++) {
        newSong->artist[i] = artist[i];
    }
    newSong->artist[i] = '\0';
    // Copy the lyrics char by char and add null char
    for(i = 0; lyrics[i] != '\0'; i++) {
        newSong->lyrics[i] = lyrics[i];
    }
    newSong->lyrics[i] = '\0';
    // Set the year from parameter
    newSong->year = year;
    // Initialize streams to 0
    newSong->streams = 0;
    // Return the initialized song
    return newSong;
}
// Read string and return pointer to the memory that keep the data
char* readString() {
    // Started size
    int length = 0;
    char c;
    // Create the first char
    char *str = (char *) malloc(sizeof(char));
    // If the allocation fails can't continue
    if (!str) {
        exit(1);
    }
    // Read first char
    scanf(" %c", &c);
    // Loop that collect chars until the user press enter
    while (c != '\n' && c != '\r') {
        // Try to make more space for the next char in the string
        char *temp = (char *) realloc(str, (length + 2) * sizeof(char));
        // If realloc fails, free and exit
        if (!temp) {
            free(str);
            exit(1);
        }
        str = temp;
        // Add the new char to the string
        str[length] = c;
        length++;
        // Read next char
        scanf("%c", &c);
    }
    // Add the null char for the compiler to know it arrived to the end of the string
    str[length] = '\0';
    return str;
}
// Make sure that there isn't any data in memory leaks
void freeSong(Song *song) {
    // Check if the song actually exists
    if(song != NULL) {
        // Free all the strings inside
        free(song->title);
        free(song->artist);
        free(song->lyrics);
        // Free the song itself
        free(song);
    }
}
// Calculate how many characters are in a string
int stringLength(char *string) {
    int length = 0;
    // Loop that keep counting chars until the end char
    while(string[length] != '\0') {
        length++;
    }
    // Return the total number of characters that counted
    return length;
}



