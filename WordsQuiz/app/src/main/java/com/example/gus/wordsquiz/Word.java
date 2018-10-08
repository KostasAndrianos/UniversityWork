package com.example.gus.wordsquiz;

import java.util.Random;

class Word {
    String word;
    boolean alreadyFound;
    int letterCount;

    Word(String word) {
        this.word = word;
        alreadyFound = false;
        letterCount = word.length();
    }

    private static String scramble(Random random, String inputString) {
        char a[] = inputString.toCharArray();
        for (int i = 0; i < a.length; i++) {
            int j = random.nextInt(a.length);
            char temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
        return new String(a);
    }

    String getScrambledWord() {
        Random r = new Random();
        return scramble(r, word);
    }
}
