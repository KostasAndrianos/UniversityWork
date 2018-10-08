package com.example.gus.wordsquiz;

import android.annotation.SuppressLint;
import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;


import java.util.ArrayList;
import java.util.concurrent.ThreadLocalRandom;

import static com.example.gus.wordsquiz.MainActivity.easyWords;
import static com.example.gus.wordsquiz.MainActivity.mediumWords;
import static com.example.gus.wordsquiz.MainActivity.hardWords;
import static java.lang.System.currentTimeMillis;

public class GameActivity extends Activity {
    Button submitBt;
    TextView scrambledWordTv;
    EditText answerEt;
    static long start;
    ArrayList<Word> words;
    static private long lastClickTime = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        ActionBar actionBar = getActionBar();
        actionBar.setDisplayHomeAsUpEnabled(true);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);
        String difficulty = getIntent().getStringExtra("difficulty");
        int minimumLetters = 0;
        int maximumLetters = 0;
        if (difficulty.trim().equals("Medium")) {
            words = mediumWords;
            minimumLetters = 4;
            maximumLetters = 7;
        } else if (difficulty.trim().equals("Easy")) {
            words = easyWords;
            minimumLetters = 0;
            maximumLetters = 3;
        } else if (difficulty.trim().equals("Hard")) {
            words = hardWords;
            minimumLetters = 8;
            maximumLetters = 50;
        }
        submitBt = findViewById(R.id.bt_submit);
        scrambledWordTv = findViewById(R.id.tv_scrambled_word);
        answerEt = findViewById(R.id.et_answer);
        start = currentTimeMillis();
        final Word word = getUnusedWord(minimumLetters, maximumLetters);
        scrambledWordTv.setText(word.getScrambledWord());
        submitBt.setOnClickListener(new Button.OnClickListener() {

            @Override
            public void onClick(View v) {
                preventDoubleClick();
                String answer = answerEt.getText().toString();
                if (answer.toLowerCase().equals(word.word.toLowerCase())) {
                    DialogFragment gameEndDialog = new GameEndedDialogFragment();
                    gameEndDialog.show(getFragmentManager(), "Congratulations");
                } else {
                    Toast.makeText(getApplicationContext(), "Try again", Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    static void preventDoubleClick() {
        if (SystemClock.elapsedRealtime() - lastClickTime < 1000) {
            return;
        }
        lastClickTime = SystemClock.elapsedRealtime();
    }

    @Override
    public void onBackPressed() {
        finish();
        super.onBackPressed();
    }

    @Override
    public void recreate() {
        answerEt.setText("");
        super.recreate();
    }

    Word getUnusedWord(int minLetterCount, int maxLetterCount) {
        //Get random item from list
        Word word = getUnusedWordWithinLetterCountRange(minLetterCount, maxLetterCount);
        //If all words have been used before.
        if (word != null && word.alreadyFound || word.letterCount < minLetterCount || word.letterCount > maxLetterCount) {
            //Make words unused again
            makeAllWordsInRangeUnused(minLetterCount, maxLetterCount);
            //Get a random word
            word = getUnusedWordWithinLetterCountRange(minLetterCount, maxLetterCount);
        }
        if (word != null) {
            word.alreadyFound = true;
        }
        return word;
    }

    void makeAllWordsInRangeUnused(int minLetterCount, int maxLetterCount) {
        for (int i = 0; i < words.size(); i++) {
            if (words.get(i).letterCount >= minLetterCount && words.get(i).letterCount <= maxLetterCount) {
                words.get(i).alreadyFound = false;
            }
        }
    }

    Word getUnusedWordWithinLetterCountRange(int minLetterCount, int maxLetterCount) {
        Word word = null;
        for (int count = 0; count < words.size(); count++) {
            word = words.get(ThreadLocalRandom.current().nextInt(words.size()));
            if (!word.alreadyFound && word.letterCount >= minLetterCount && word.letterCount <= maxLetterCount) {
                break;
            }
        }
        return word;
    }

    public static class GameEndedDialogFragment extends DialogFragment {
        @Override
        public void onCancel(DialogInterface dialog) {
            super.onCancel(dialog);
            getActivity().finish();
        }

        @Override
        public Dialog onCreateDialog(Bundle savedInstanceState) {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            LayoutInflater inflater = getActivity().getLayoutInflater();
            View dialogView = inflater.inflate(R.layout.dialog_game_end, null);
            builder.setView(dialogView)
                    .setPositiveButton("Play again", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            preventDoubleClick();
                            GameEndedDialogFragment.this.getDialog().dismiss();
                            getActivity().recreate();
                        }
                    })
                    .setNegativeButton("Exit", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            preventDoubleClick();
                            GameEndedDialogFragment.this.getDialog().cancel();
                        }
                    });
            final long end = currentTimeMillis();
            final long totalTimeSeconds = (end - start) / 1000;
            final long totalTimeMinutes = (totalTimeSeconds % 3600) / 60;
            TextView totalTimeTv = dialogView.findViewById(R.id.tv_total_time);
            @SuppressLint("DefaultLocale") String formattedTotalTime = String.format("%02d", totalTimeMinutes) + ":" + String.format("%02d", totalTimeSeconds);
            totalTimeTv.setText(formattedTotalTime);
            ImageView firstStar = dialogView.findViewById(R.id.iv_star_one);
            ImageView secondStar = dialogView.findViewById(R.id.iv_star_two);
            ImageView thirdStar = dialogView.findViewById(R.id.iv_star_three);
            firstStar.setVisibility(View.VISIBLE);
            if (totalTimeSeconds < 15) {
                secondStar.setVisibility(View.VISIBLE);
            }
            if (totalTimeSeconds < 8) {
                thirdStar.setVisibility(View.VISIBLE);
            }
            return builder.create();
        }
    }
}
