#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** hash_map;
int tableSize = 0;
int noElements = 0;
float loadFactor = 0;
//Finds the ascii values of all of the characters in the key, adds them up and mods the result with the size
//of the hash map
int hash_function(const char *key) {
    int ascii = 0;
    int val;
    int c;
    for (int i=0; i<strlen(key); i++){
        c = key[i];
        val = c;
        if (val >=65 && val <= 90) {
            ascii += val;
        }
    }
    return ascii % tableSize;
}
//Finds the size of the text file so I can allocate memory accordingly
int sizeOfFile(const char* filename){
    FILE *inp = fopen(filename, "r");
    char c;
    int i = 0;
    if (inp) {
        c = fgetc(inp);
        while (!feof(inp)){
            //putchar(c);
            i++;
            c = fgetc(inp);
        }
        fclose(inp);
    }
    return i;
}
//Reads the text file of names and returns a list with all of the names
char *readFile(const char *filename, char* list){
    FILE *inp = fopen(filename, "r");
    char c;
    int i = 0;
    if (inp) {
        c = fgetc(inp);
        while (!feof(inp)){
            if(c != '"' && c != ',') {
                list[i] = c;
                i++;
            }else if(c == ','){
                list[i] = '\n';
                i++;
            }
            c = fgetc(inp);
        }
        fclose(inp);
    }
    return list;
}
//Makes the map bigger to accommodate more items if the load factor is above 0.7
//It then takes all of the items from the original map and rehashes them into the new, bigger map
void resize_map(int new_size) {
    char **new_map;
    new_map = malloc(new_size*sizeof(hash_map));
    for (int j=0; j<new_size; j++){
        new_map[j] = NULL;
    }
    for (int i=0; i<tableSize; i++){
        if (hash_map[i] != NULL && strcmp(hash_map[i], "H") != 0){
            int index = hash_function(hash_map[i]);
            if (new_map[index] == NULL || strcmp(new_map[index], "H") == 0) {
                new_map[index] = hash_map[i];
            } else {
                while (new_map[index] != NULL || index + 1 != new_size) {
                    if (new_map[index] == NULL || strcmp(new_map[index], "H") == 0) {
                        new_map[index] = hash_map[i];
                        break;
                    }
                    index++;
                }
            }

        }
    }
    free(hash_map);
    hash_map = new_map;
    tableSize = new_size;
}
//Uses the hash function and linear probing to find the desired item in the hash map
int search_map(const char *name) {
    int index = hash_function(name);
    while (hash_map[index] != NULL && index <= tableSize){
      //  printf("%s", hash_map[index]);
        if (hash_map[index] == name){
            return 1;
        }else{
            index++;
        }
    }
    return 0;
}
//Adds the desired name to the map using the hash function.
//If that space is occupied, I use linear probing to find an empty space.
void add_to_map(const char *name) {
    if (!search_map(name)) {
        int index = hash_function(name);
        if (hash_map[index] == NULL) {
            hash_map[index] = (char*) name;
            noElements+=1;
            printf("%s", hash_map[index]);
            printf("%s\n", "Added to the hash table");
        } else if (strcmp(hash_map[index], "H") == 0) {
            hash_map[index] = (char*) name;
           // printf("%s", hash_map[index]);
            noElements+=1;
            printf("%s", hash_map[index]);
            printf("%s\n", "Added to the hash table");
        }else {
                while (index + 1 < tableSize) {
                    if (hash_map[index] == NULL) {
                        hash_map[index] = (char*) name;
                        noElements+=1;
                        printf("%s", hash_map[index]);
                        printf("%s\n", "Added to the hash table");
                        break;
                    }
                    index++;
                }

        }
        loadFactor = (float) noElements/(float) tableSize;
        if (loadFactor > 0.7) {
            resize_map(noElements * 2);
        }
    }else{
        printf("%s\n", "That is already in the map.");
    }
}
//Removes the desired item and replaces it with 'H' so searches don't go wrong
int remove_from_map(const char *name) {
    int index = hash_function(name);
    if (hash_map[index] == name){
        hash_map[index] = "H";
        return 1;
    }else{
        while (hash_map[index] != NULL){
            if (index == tableSize && hash_map[index] != name){
                return 0;
            }
            else if (hash_map[index] == name){
                hash_map[index] = "H";
                return 1;
            }
            index++;
        }
    }
    return 0;
}
//Prints the map item with the corresponding index.
//If it is empty, it displays ---
void print_map() {
    for (int i=0; i<tableSize; i++){
        if (hash_map[i] != NULL){
            printf("%d %s\n",i, hash_map[i]);
        }else{
            printf("%d    ---\n", i);
        }
    }
}

int main(int argc, char *argv[]) {
	//Initialise the map to resize later
	hash_map = malloc(5);
	const char *filename = "./names.txt";
	int fileSize = sizeOfFile(filename);
    char *list = malloc(fileSize);
    //Initialises this to change later
    readFile(filename, list);
    resize_map(6);
    int index = 0;
    int increment = 0;
    int buffer = 0;
    char *nameToAdd;
    //This loop adds all of the names from the text list into the hash map.
    for (int i=0; i<strlen(list);i++){
        if (list[i] != '\n'){
            index++;
        }else{
            //Adds the characters of each name to nameToAdd
            nameToAdd = malloc(index + 1);
            for (int j=buffer; j<i; j++){
                nameToAdd[increment] = list[j];
                increment++;
            }
            //Made nameToAdd a string to make it easier to read in.
            nameToAdd[increment+1] = '\0';
            add_to_map(nameToAdd);
            index = 0;
            buffer = i + 1;
            increment = 0;
        }
    }

    print_map();
   int ret = search_map("OMER");
    if(ret)
        printf("Found %s!\n", "OMER");
    
	return EXIT_SUCCESS;
}
