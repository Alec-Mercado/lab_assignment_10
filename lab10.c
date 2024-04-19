#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET 26

struct TrieNode
{
    struct TrieNode * children[ALPHABET];
    int finalWord;
    int count;
};

// Trie structure
struct Trie
{
    struct TrieNode * root;
};

struct TrieNode *createTrieNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        node->finalWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET; i++)
            node->children[i] = NULL;
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct TrieNode *crawl = pTrie->root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!crawl->children[index])
            crawl->children[index] = createTrieNode();

        crawl = crawl->children[index];
    }
    crawl->finalWord = 1;
    crawl->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct TrieNode *crawl = pTrie->root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!crawl->children[index])
            return 0;

        crawl = crawl->children[index];
    }
    if (crawl != NULL && crawl->finalWord)
        return crawl->count;

    return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie == NULL)
        return NULL;
    
    struct TrieNode *node = pTrie->root;

    if(node == NULL)
        return NULL;
    
    for (int i = 0; i < ALPHABET; i++)
    {
        if (node->children[i] != NULL)
            deallocateTrie(node->children[i]);
    }

    free(node);
    free(pTrie);
    return NULL;

}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = createTrieNode();
    return pTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Error\n");
        return 0;
    }

    int words = 0;
    char word[100];

    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[words] = strdup(word);
        words++;
    }

    fclose(file);
    return words;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}