package com.example.gus.wordsquiz;

import java.util.ArrayList;

public class DataUtils {
    static ArrayList<Word> fillHardWithWords() {
        ArrayList<Word> arrayList = new ArrayList<>();
        arrayList.add(new Word("lamborghini"));
        arrayList.add(new Word("girlfriend"));
        arrayList.add(new Word("programming"));
        arrayList.add(new Word("whatever"));
        arrayList.add(new Word("pineapple"));
        arrayList.add(new Word("transmission"));
        arrayList.add(new Word("developer"));
        arrayList.add(new Word("university"));
        arrayList.add(new Word("xylophone"));
        arrayList.add(new Word("guardian"));
        arrayList.add(new Word("superman"));
        arrayList.add(new Word("spiderman"));
        arrayList.add(new Word("catwoman"));
        return arrayList;
    }

    static ArrayList<Word> fillMediumWithWords() {
        ArrayList<Word> arrayList = new ArrayList<>();
        arrayList.add(new Word("apple"));
        arrayList.add(new Word("bicycle"));
        arrayList.add(new Word("ferrari"));
        arrayList.add(new Word("school"));
        arrayList.add(new Word("android"));
        arrayList.add(new Word("check"));
        arrayList.add(new Word("half"));
        arrayList.add(new Word("ball"));
        arrayList.add(new Word("bird"));
        arrayList.add(new Word("parrot"));
        arrayList.add(new Word("scream"));
        arrayList.add(new Word("ironman"));
        arrayList.add(new Word("batman"));
        return arrayList;
    }

    static ArrayList<Word> fillEasyWithWords() {
        ArrayList<Word> arrayList = new ArrayList<>();
        arrayList.add(new Word("car"));
        arrayList.add(new Word("dog"));
        arrayList.add(new Word("cat"));
        arrayList.add(new Word("hat"));
        arrayList.add(new Word("kit"));
        return arrayList;
    }
}
