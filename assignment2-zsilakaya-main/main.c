#include <stdio.h>
#include <string.h>
#include <math.h>
void toLower(char *s);
int main(int argc, char *argv[]) {
	//task2
	FILE *wordFile;
	wordFile = fopen("wordList.txt", "r");
	char c;
	int wfLineCount = 0; //count the number of lines in wordList.txt file
	int charCountWList = 0; //count the number of characters in wordList.txt file
	while ((c = getc(wordFile)) != EOF) {
		charCountWList++;
		if (c == '\r') {
			charCountWList++;
			wfLineCount++;
		}
	}
	char wordsLine[charCountWList]; //array that contains all the words in wordList.txt file
	int negList[wfLineCount]; //array of negative review occurrence
	int posList[wfLineCount]; //array of positive review occurrence

	for (int i = 0; i < wfLineCount; i++) { //initializes the arrays.
		negList[i] = 0;
		posList[i] = 0;
	}

	rewind(wordFile); // moves file pointer position to the beginning of the file.
	char *wordsList[wfLineCount]; //string array of words in the wordList.txt
	fgets(wordsLine, sizeof(wordsLine), wordFile);
	char *words = strtok(wordsLine, "\r");
	int count = 0;
	while (words != NULL) {
		wordsList[count] = words;
		count++;
		words = strtok(NULL, "\r");
	}
	//task1
	int score = 0;
	int freq = 0;
	double avScore = 0; //score/freq
	printf("Enter a word:\n");
	char input[50];
	scanf("%s", &input);
	toLower(input);
	FILE *reviewFile;
	reviewFile = fopen("movieReviews.txt", "r");
	char line[1000];
	if (reviewFile == NULL) {
		return (-1);
	}
	while (fgets(line, sizeof(line), reviewFile) != NULL) {
		int size = sizeof(line);
		char *wordsArray[size];
		int wordNum = 0;
		char *words = strtok(line, " "); //split words by tokens and store in the array word by word.
		while (words != NULL) {
			wordsArray[wordNum] = words;
			wordNum++;
			words = strtok(NULL, " ");
		}
		//for task1
		for (int i = 0; i < wordNum; i++) {
			toLower(wordsArray[i]);
			if (strcmp(wordsArray[i], input) == 0) {
				score += (line[0] - 48);
				freq++;
			}
		}
		//for task2
		for (int i = 0; i < wordNum; i++) {
			toLower(wordsArray[i]);
			for (int j = 0; j < wfLineCount; j++) {
				if (strcmp(wordsArray[i], wordsList[j]) == 0) {
					int task2Score = (line[0] - 48);
					if (task2Score >= 3) { //if review score is bigger than 3
						posList[j]++;
					} else { //if review score is less than 3
						negList[j]++;
					}
				}
			}
		}
	}
	printf("%s appears %d times.\n", input, freq);
	avScore = (score * 1.0) / (freq); //multiplying with 1.0 to achieve double division
	if (score != 0 && freq != 0) {
		printf("The average score for reviews containing the word %s is %.4f\n",
				input, avScore);
	} else {
		printf("The average score for reviews containing the word %s is 0\n",
				input);
	}
	
	int maxPos = 0; //position of the most positive occured word in the wordList array
	int mosOccurPos = 0; //position of the most positive occured word in the posList array
	int maxNeg = 0; //position of the most negative occured word in the wordList array
	int mosOccurNeg = 0; //position of the most positive occured word in the negList array
	for (int i = 0; i < count; i++) {
		if (maxPos < posList[i]) {
			maxPos = posList[i];
			mosOccurPos = i;
		}
		if (maxNeg < negList[i]) {
			maxNeg = negList[i];
			mosOccurNeg = i;
		}
	}
	printf("The most positive reviewed word is %s with %d occurrence.\n",
			wordsList[mosOccurPos], maxPos);
	printf("The most negative reviewed word is %s with %d occurrence.\n",
			wordsList[mosOccurNeg], maxNeg);
	
	fclose(reviewFile);
	fclose(wordFile);
	return 0;
}
void toLower(char *s) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] = s[i] + 32;
		}
	}

}


