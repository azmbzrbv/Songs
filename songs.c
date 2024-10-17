#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Length
{
    int minutes;
    int seconds;
}Length;

typedef struct Songs// Songs structure is created to store the information related to the song!
{
  char Title[30];
  char Performer[30];
  char Album[30];
  int ReleaseYear;
  char Genre[30];
  Length lenght;
}Songs;                             

void DisplayMenu()
{
    printf("1. Add new song entry to the Database\n"
           "2. Display songs of selected artist\n"
           "3. Display songs of selected album\n"
           "4. List songs released in selected year\n"
           "5. List songs of selected genre\n"
           "6. Exit\n"
           "7. Show \"Menu\" again\n"
           "8. Delete a song from Database\n"
           "9. Save data to file\n"
           "10. Create collections\n");

}

//This function is used to remove any unwanted character left in the input buffer, which can lead to the crash of the program.
// while loop reads the data from the buffer and just ignores it, which effectively discards the character
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
// This function is created to save the collections of the songs to the file. 
void createCollections(Songs array[], int size, char *filename)
{
    int choice; 
    printf("Do you want to create new collections or add to older one?\n"
           "1. New\n"
           "2. Add\n");
           scanf("%d", &choice);
           clearInputBuffer();
    
    printf("Write the title of the songs you want to save in the collections\n"
            "When you done, write \"Exit\"!\n");
    char line[30]; 
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")]='\0';
    
    int index[100];
    int j=0;
    int error=0;// Created to check if selected name exists or not 
    while(strcmp(line, "Exit")!=0)
    {
         for(int i=0; i<size; i++)
         {
            if(strcmp (array[i].Title, line)==0)
            {
                index[j]=i;
                j++;
            }
         }
    if(j==error)
       printf("Invalid selection. Please try again!\n");
    else
       error++;

      fgets(line, sizeof(line), stdin);
      line[strcspn(line, "\n")]='\0';
    }
    if(choice==1){
    FILE *file=fopen(filename, "w");
    if(file==NULL)
    {
        perror("Erorr! ");
        return;
    }
     // Writing headers to the file in a tabular format
    fprintf(file, "%-30s %-20s %-20s %-15s %-10s %s\n",
        "Title", "Performer", "Album", "Release Year", "Genre", "Length");

    // Writing the selected songs to the file
    for (int i = 0; i < j; i++) {
        int idx = index[i];
        fprintf(file, "%-30s %-20s %-20s %-15d %-10s %d:%02d\n",// %-30s was used to make file more readable
                array[idx].Title,
                array[idx].Performer,
                array[idx].Album,
                array[idx].ReleaseYear,
                array[idx].Genre,
                array[idx].lenght.minutes,
                array[idx].lenght.seconds);
    }

    // Closing the file
    fclose(file);
    printf("Collection saved successfully in '%s'.\n", filename);
    }
    if(choice==2){
      FILE *file=fopen(filename, "a");
    if(file==NULL)
    {
        perror("Erorr! ");
        return;
    }  
    for (int i = 0; i < j; i++) {
        int idx = index[i];
        fprintf(file, "%-30s %-20s %-20s %-15d %-10s %d:%02d\n",// %-30s was used to make file more readable
                array[idx].Title,
                array[idx].Performer,
                array[idx].Album,
                array[idx].ReleaseYear,
                array[idx].Genre,
                array[idx].lenght.minutes,
                array[idx].lenght.seconds);
    }

    // Closing the file
    fclose(file);
    printf("Collection saved successfully in '%s'.\n", filename);
    }

     DisplayMenu();
}


//It is possible to load the existing database from file, by calling this function. it works by using fgets and sscanf function.
void LoadDatabase(Songs *array[], int *size, char *filename)
{
    FILE *file=fopen(filename, "r");
    if(file==NULL){
        perror("Error!");
        return;
    }

    char line[100];
    Songs temparr; //temporarry array is created!

    while(fgets(line, sizeof(line), file))
    {
        // sscanf function will search for "Title:" and read the following string after it, until the end of the line.
        // If it was successful, then it will return 1, which will lead to the code in "if" statement work.
        if(sscanf(line, "Title: %[^\n]", temparr.Title)==1)
        {
            fgets(line, sizeof(line), file);
            sscanf(line,"Performer: %[^\n]", temparr.Performer);
            fgets(line,sizeof(line), file);
            sscanf(line, "Album: %[^\n]", temparr.Album);
            fgets(line, sizeof(line), file);
            sscanf(line, "Release year: %d", &temparr.ReleaseYear);
            fgets(line, sizeof(line), file);
            sscanf(line, "Genre: %[^\n]", temparr.Genre);
            fgets(line, sizeof(line), file);
            sscanf(line, "Length: %d:%d", &temparr.lenght.minutes, &temparr.lenght.seconds);

            (*array)[*size] = temparr; // copy  the temporary array to original array
            *size=*size+1;
            *array=realloc(*array, (*size+1)*sizeof(Songs));
             if (*array == NULL) {
                perror("Error reallocating memory");
                fclose(file);
                return;
            }
        }
    }
    printf("Database loaded to the memory successfully!\n\n");


     DisplayMenu();

}



// Following function is used to save the data from the memory into file.
void SaveToFile(Songs array[], int size, char* filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error!");
        return;
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "\nTitle: %s\nPerformer: %s\nAlbum: %s\nRelease year: %d\nGenre: %s\nLength: %d:%d\n",
            array[i].Title, array[i].Performer, array[i].Album,
            array[i].ReleaseYear, array[i].Genre,
            array[i].lenght.minutes, array[i].lenght.seconds);
    }

    fclose(file);
    printf("Songs are loaded to the file successfully!\n");

     DisplayMenu();
}


// Following function is able to store the data to memory from the standard input (from the user input)
// it is handled by the help of the fgets function.
void Addnew (Songs *array[], int *size)
{

    printf("Title of the song: ");
    fgets((*array)[*size].Title, sizeof((*array)[*size].Title), stdin);
    (*array)[*size].Title[strcspn((*array)[*size].Title, "\n")] = '\0'; // Remove newline character and replace it with termination character


    printf("Performer: ");
    fgets((*array)[*size].Performer, sizeof((*array)[*size].Performer), stdin);
    (*array)[*size].Performer[strcspn((*array)[*size].Performer, "\n")] = '\0';// This is important for formatting of the program.


    printf("Title of the album: ");
    fgets((*array)[*size].Album, sizeof((*array)[*size].Album), stdin);
    (*array)[*size].Album[strcspn((*array)[*size].Album, "\n")] ='\0';// Also crucial for the comparison of the two strings.

    printf("Year of release: ");
    scanf("%d", &(*array)[*size].ReleaseYear);
    clearInputBuffer(); // Clear buffer after using scanf function, to ensure that program will work in correct way.

    printf("Genre of the music: ");
    fgets((*array)[*size].Genre, sizeof((*array)[*size].Genre), stdin);
    (*array)[*size].Genre[strcspn((*array)[*size].Genre, "\n")] = '\0';

    printf("Length of the song:\n     Minutes: ");
    scanf("%d", &(*array)[*size].lenght.minutes);
    clearInputBuffer();

    printf("     Seconds: ");
    scanf("%d", &(*array)[*size].lenght.seconds);
    clearInputBuffer();

    *size = *size + 1;
    *array = realloc((*array), (*size + 1) * sizeof(Songs));// size of an array is increased

     DisplayMenu();
}



//The function is used to display the songs by the specific artist, typed by an user.
void DisplaybyArtist(Songs array[], int size)
{
    char artist[30];
    printf("Enter the name of the artist: ");
    fgets(artist, 30, stdin);
    artist[strcspn(artist, "\n")]='\0'; // "strcspn" function will find the index in artist string where "\n" is located;

    int j=0;
    for(int i=0; i<size; i++)
    {
        if(strcmp(array[i].Performer, artist)==0){
        printf("\nTitle:%s\n"
               "Title of the Album:%s\n"
               "Year of release:%d\n"
               "Genre:%s\n"
               "Length:%d:%d\n", array[i].Title, array[i].Album, array[i].ReleaseYear, array[i].Genre, array[i].lenght.minutes, array[i].lenght.seconds);
               j++;
        }
    }
    if(j==0)  // if j is still zero(which means that songs are not found), following text will be printed!
        printf("\nThere isn't any song with this artist!\n");
    DisplayMenu();
}



// The following function is identical to the previous function
// It  will display the songs in specific album, typed by an user.
void Displaybyalbum(Songs array[], int size)
{
    char album[30];
    printf("Enter the name of the album: ");
    fgets(album, 30, stdin);
    album[strcspn(album, "\n")]='\0';
    int j=0;
    for(int i=0; i<size; i++)
    {
        if(strcmp(array[i].Album, album)==0)// check if  there is any identical string in the memory!
        //If yes, do following
        {
        printf("\nTitle:%s\n"
               "Performer:%s\n"
               "Year of release:%d\n"
               "Genre:%s\n"
               "Length:%d:%d\n", array[i].Title, array[i].Performer, array[i].ReleaseYear, array[i].Genre, array[i].lenght.minutes, array[i].lenght.seconds);
               j++;
        }
    }
    if(j==0) // if j is still zero(which means that songs are not found), following text will be printed!
        printf("There is no such album!\n");

    DisplayMenu();
}


// This function will list the songs released in specific year, typed by the user.
void listbyYear(Songs array[], int size)
{
    int year;
    int j=1;
    printf("Enter the year: ");
    scanf("%d", &year);
    clearInputBuffer();// clear input buffer after scanf function, to ensure that program will work in correct way

    printf("Songs which were released in %d\n", year);
    for(int i=0; i<size; i++)
    {
        if(year==array[i].ReleaseYear)
        {
            printf("\n%d.Name: %s\n"
                   "Performer: %s\n"
                   "Album: %s\n"
                   "Genre: %s\n"
                   "Length: %d:%d\n", j, array[i].Title, array[i].Performer, array[i].Album, array[i].Genre, array[i].lenght.minutes, array[i].lenght.seconds);
            j++;
        }
    }
    if(j==1)// If j is 1 after loop, there are no songs released on specific year.
        printf("\nThere isn't any song released on this year!\n");

    DisplayMenu();     
}

// Follwing line will list the songs of specific genre, typed by an user
void listbyGenre(Songs array[], int size)
{
    char genre[30];
    int j=1;
    printf("Enter the genre of the song: ");
    fgets(genre, 30, stdin);
    genre[strcspn(genre, "\n")]='\0';// newline character is removed to ensure that it will with other string in the memory.

    printf("%s songs", genre);
    for(int i=0; i<size; i++)
    {
        if(strcmp(array[i].Genre, genre)==0)
        {
            printf("\n%d.Name: %s\n"
                   "Performer: %s\n"
                   "Album: %s\n"
                   "Year of release: %d\n"
                   "Length: %d:%d\n", j, array[i].Title, array[i].Performer, array[i].Album, array[i].ReleaseYear, array[i].lenght.minutes, array[i].lenght.seconds);
            j++;
        }
    }
    if(j==1)// if the j is still 1, there are no songs in corresponding genre.
        printf("\nThere isn't any song in this genre!\n");


    DisplayMenu();
}

//Following function is used to delete the desired songs from the memory as well as from the file.
void Delete (Songs *array[], int *size, char *filename)
{
  char t[30];
  printf("Title of the song: ");
  fgets(t, 30, stdin);
  t[strcspn(t, "\n")]='\0';
  int index=-1;// index variable is created and set to -1.

  for(int i=0; i<*size; i++)
  {
      if(strcmp(t, (*array)[i].Title)==0)
      {
        index=i;// if there is any identical song that user wants to delete, the index of that particular song is saved
              break;
      }
  }

  if (index == -1) {// if index is still -1, then there is no matching song in the memory to delete
        printf("There is no matching song name!\n");
        return;
    }


// following loop is used to remove from the memory the song;
  for (int i = index; i < *size - 1; ++i) {
        (*array)[i] = (*array)[i + 1];// it will replace the song with the following song.
    }
    *size=*size-1;
    *array=realloc((*array), (*size)*sizeof(Songs)); // the array is reduced.

    FILE *file = fopen(filename, "w");// Open the file to delete the song also from there
    if (file == NULL) {// if opening was not successful print "Error!" and return exit from the function
        printf("Error!");
        return;
    }

    for (int i = 0; i < *size; i++) {// this loop will reprint the new memory to file.
        fprintf(file, "\nTitle: %s\nPerformer: %s\nAlbum: %s\nRelease year: %d\nGenre: %s\nLength: %d:%d\n",
            (*array)[i].Title, (*array)[i].Performer,(*array)[i].Album,
            (*array)[i].ReleaseYear,(*array)[i].Genre,
            (*array)[i].lenght.minutes, (*array)[i].lenght.seconds);
    }

    fclose(file);
    printf("The song is deleted successfully!\n"); // If everything was successful, then this line will be printed.

     DisplayMenu();
}



// Function is used to display Menu with all available options



int main ()
{
int size=0, option = 3;
Songs *array=(Songs*)malloc((size+1)*sizeof(Songs)); // new dynamic array is created with type of Songs.


int n;
char name[30];// string is created to  store the name of the existing file
printf("Do you have an already existing database?\n1.Yes\n2.No\n");
scanf("%d", &n);
clearInputBuffer();
if(n==1)
{
  printf("Type the detailed name of the file with database!\n");
  scanf("%s", name);
  // if user has the file with appropriate database, following function is called.
  LoadDatabase(&array, &size, name);
}
DisplayMenu();

while(option!=6)// the program will do the proper tasks until user will type '6'
{

  scanf("%d", &option);
  clearInputBuffer();// clear input buffer from unwanted characters.
    switch (option)// this will do corresponding tasks to the typed number by an user.
    {
     case 1: Addnew(&array, &size); break;
     case 2: DisplaybyArtist(array, size); break;
     case 3: Displaybyalbum(array, size); break;
     case 4: listbyYear(array, size); break;
     case 5: listbyGenre(array, size); break;
     case 7: DisplayMenu(); break;
     case 8: Delete(&array, &size, "songsDB/db.txt"); break;
     case 9: SaveToFile(array ,size, "songsDB/db.txt"); break;
     case 10: createCollections(array, size, "Collections.txt");
     default: break;
    }

}
free(array);// free the dynamically allocated memory
return 0;
}